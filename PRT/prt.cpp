#include "prt.h"
#include <opencv2\opencv.hpp>
using namespace cv;
// 43

bool Image::loadFromFile(const char* filename)
{
	//Mat mat = imread("spheremap.bmp");
	Mat mat = imread(filename);
	if (mat.empty()) {
		fprintf(stderr, "Error: load image failed.");
		return false;
	}
	namedWindow("image", CV_WINDOW_AUTOSIZE);
	imshow("image", mat);

	height = mat.rows;
	width = mat.cols;
	pixel[0] = new float[height * width];
	pixel[1] = new float[height * width];
	pixel[2] = new float[height * width];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vec3b intensity = mat.at<Vec3b>(i, j);
			pixel[0][i*width + j] = intensity.val[2] / 255.0f;
			pixel[1][i*width + j] = intensity.val[1] / 255.0f;
			pixel[2][i*width + j] = intensity.val[0] / 255.0f;
		}
	}
	return true;
}


void GenerateSamples(Sampler* sampler, int N)
{
	Sample* samples = new Sample [N*N];
	sampler->samples = samples;
	sampler->number_of_samples = N*N;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			float a = ((float)i + rand()/RAND_MAX) / (float) N;
			float b = ((float)j + rand()/RAND_MAX) / (float) N;
			float theta = 2*acos(sqrt(1-a));
			float phi = 2*PI*b;
			float x = sin(theta)*cos(phi);
			float y = sin(theta)*sin(phi);
			float z = cos(theta);
			int k = i*N + j;
			//sampler->samples[k].spherical_coord.theta = theta;
			//sampler->samples[k].spherical_coord.phi = phi;
			sampler->samples[k].cartesian_coord.x = x;
			sampler->samples[k].cartesian_coord.y = y;
			sampler->samples[k].cartesian_coord.z = z;
			sampler->samples[k].sh_functions = NULL;
		}
	}
}

//// obsolete
//float DoubleFactorial(int n)
//{
//	float result = 1.0f;
//	for ( ; n > 1; n -= 2)
//		result *= n;
//	return result;
//}
//
//float factorial(int n)
//{
//	float result = 1.0f;
//	for (; n > 1; n-- )
//		result *= n;
//	return result;
//}
//
////=================MATH====================
//// obsolete
//float Legendre(int l, int m, float x)
//{
//	float result = 0;
//	if (l == m+1)
//		result = x*(2*m + 1)*Legendre(m, m, x);
//	else if (l == m)
//		result = pow(-1, m)*DoubleFactorial(2*m-1)*pow((1-x*x), m/2);
//	else
//		result = (x*(2*l-1)*Legendre(l-1, m, x) - (l+m-1)*Legendre(l-2, m, x))/(l-m);
//	return(result);
//}
//
//// obsolete
//double K(const unsigned int l, const int m) {
//	const unsigned int cAM = abs(m);
//	double uval = l;
//	for (unsigned int k = l+cAM; k > (l-cAM); k--)
//		uval *= k;
//	return sqrt((2.0* l + 1.0) / (4*PI*uval));
//}
//float K(int l, int m)
//{
//	float num = (2*l+l) * factorial(1-abs(m));
//	float denom = 4 * PI * factorial(1+abs(m));
//	float result = sqrt(num/denom);
//	return result;
//}


//===================SH==================
// obsolete
//double SphericalHarmonic(int l, int m, float theta, float phi)
//{
//	double result = 0;
//	if (m > 0)
//		result = sqrt(2.0f) * K(l, m) * cos(m*phi) * Legendre(l, m, cos(theta));
//	else if (m < 0)
//		result = sqrt(2.0f) * K(l, m) * sin(-m*phi) * Legendre(1, -m, cos(theta));
//	return result;
//}

//
void PrecomputeSHFunctions(Sampler* sampler, int bands)
{
	if (bands < 3) {
		bands = 3;
		cout << "[WARNING] bands must be >=3 and <= 10. Set bands = 3.\n";
	} else if (bands > 10) {
		bands = 10;
		cout << "[WARNING] bands must be >=3 and <= 10\n. Set bands = 10\n";
	}
	SHEvalFunc SHEval[] = {SHEval3, SHEval3, SHEval3, SHEval3, SHEval4, SHEval5, SHEval6, SHEval7, SHEval8, SHEval9, SHEval10};
	for (int i = 0; i < sampler->number_of_samples; i++)
	{
		float* sh_functions = new float[bands*bands];
		sampler->samples[i].sh_functions = sh_functions;
		float x = sampler->samples[i].cartesian_coord.x;
		float y = sampler->samples[i].cartesian_coord.y;
		float z = sampler->samples[i].cartesian_coord.z;
		SHEval[bands](x, y, z, sh_functions);
		// old way  stack overflow!!!
		//float theta = sampler->samples[i].spherical_coord.theta;
		//float phi = sampler->samples[i].spherical_coord.phi;
		//for (int l = 0; l < bands; l++)
		//	for (int m = -l; m <= l; m++)
		//	{
		//		int j = l*(l+1) + m;
		//		sh_functions[j] = SphericalHarmonic(l, m, theta, phi);
		//	}
	}
}

// 6 Lighting
// out: color
void LightProbeAccess(Color* color, Image* image, Vector3& direction)
{
	float d = sqrt(direction.x*direction.x + direction.y*direction.y);
	float r = (d == 0) ? 0.0f : (1.0f/PI/2.0f) * acos(direction.z) / d;
	float tex_coord [2];
	tex_coord[0] = 0.5f + direction.x * r;
	tex_coord[1] = 0.5f + direction.y * r;
	int pixel_coord [2];
	pixel_coord[0] = int(tex_coord[0] * image->width);
	pixel_coord[1] = int(tex_coord[1] * image->height);
	int pixel_index = pixel_coord[1]*image->width + pixel_coord[0];
	color->r = image->pixel[0][pixel_index];
	color->g = image->pixel[1][pixel_index];
	color->b = image->pixel[2][pixel_index];
}


// out: color coeffs
void ProjectLightFunction(Color* coeffs, Sampler* sampler, Image* light, int bands)
{
	if (bands < 3) {
		bands = 3;
		cout << "[WARNING] bands must be >=3 and <= 10. Set bands = 3.\n";
	} else if (bands > 10) {
		bands = 10;
		cout << "[WARNING] bands must be >=3 and <= 10. Set bands = 10\n";
	}

	for (int i = 0; i < bands*bands; i++) {
		coeffs[i].r = 0.0f;
		coeffs[i].g = 0.0f;
		coeffs[i].b = 0.0f;
	}
	for (int i = 0; i < sampler->number_of_samples; i++) {
		Vector3& direction = sampler->samples[i].cartesian_coord;
		for (int j = 0; j < bands*bands; j++) {
			Color color;
			LightProbeAccess(&color, light, direction);
			float sh_function = sampler->samples[i].sh_functions[j];
			coeffs[j].r += (color.r * sh_function);
			coeffs[j].g += (color.g * sh_function);
			coeffs[j].b += (color.b * sh_function);
			//Color directionLight(0.01f, 0.01f, 0.01f);	// ฒนนโ
			//if (direction.z > 0) {
			//	coeffs[j] += directionLight;
			//}
		}
	}
	float weight = 4.0f*PI;
	float scale = weight / sampler->number_of_samples;
	for (int i = 0; i < bands*bands; i++) {
		coeffs[i].r *= scale;
		coeffs[i].g *= scale;
		coeffs[i].b *= scale;
	}
}


// out: codffs
// in:  sampler
//		model
//		bands
void ProjectUnshadowed(Color** coeffs, Sampler* sampler, CAssimpModel* model, int bands)
{
	for (int i = 0; i < model->vertices.size(); i++) {
		for (int j = 0; j < bands*bands; j++) {
			coeffs[i][j].r = 0.0f;
			coeffs[i][j].g = 0.0f;
			coeffs[i][j].b = 0.0f;
		}
	}

	for (int i = 0; i < model->vertices.size(); i++) {
		for (int j = 0; j < sampler->number_of_samples; j++) {
			Sample& sample = sampler->samples[j];
			float cosine_term = glm::dot(model->vertices[i].m_normal, sample.cartesian_coord);
			//if (cosine_term < 0.f) cosine_term = 0.f;
			for (int k = 0; k < bands*bands; k++) {
				float sh_function = sample.sh_functions[k];
				//int materia_idx = model->material[i];
				//Color& albedo = model->albedo[materia_idx];
				//coeffs[i][k].r += (albedo.r * sh_function * cosine_term);
				//coeffs[i][k].g += (albedo.g * sh_function * cosine_term);
				//coeffs[i][k].b += (albedo.b * sh_function * cosine_term);
				coeffs[i][k].r += (1.f * sh_function * cosine_term);
				coeffs[i][k].g += (1.f * sh_function * cosine_term);
				coeffs[i][k].b += (1.f * sh_function * cosine_term);
			}
		}
	}

	float weight = 4.0f*PI;
	float scale = weight / sampler->number_of_samples;
	for (int i = 0; i < model->vertices.size(); i++) {
		for (int j = 0; j < bands*bands; j++) {
			coeffs[i][j] *= scale;
		}
	}
}


bool RayIntersectsTriangle(Vector3& p, Vector3& d, Vector3& v0, Vector3& v1, Vector3& v2)
{
	Vector3 e1 = v1 - v0;
	Vector3 e2 = v2 - v0;
	Vector3 h = glm::cross(d, e2);
	float a = glm::dot(e1, h);
	if (a > -0.00001f && a < 0.00001f)
		return false;

	float f = 1.0f / a;
	Vector3 s = p - v0;
	float u = f * glm::dot(s, h);
	if (u < 0.0f || u > 1.0f)
		return false;

	Vector3 q = glm::cross(s, e1);
	float v = f * glm::dot(d, q);
	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = f * glm::dot(e2, q);
	if (t < 0.0f)
		return false;
	return true;
}

bool Visibility(CAssimpModel* model, int vertexidx, Vector3& direction, Grid* grid)
{
	bool visible = true;
	assert(vertexidx >= 0 && vertexidx <  model->vertices.size());
	Vector3& p = model->vertices[vertexidx].m_pos;
	Ray ray(p, direction);
	if (grid->intersect(ray)) {
		//cout << "1\n";
		return false;
	}
	return true;
	//return grid->intersect(ray);
	//return visible;
}

void ProjectShadowed(Color** coeffs, Sampler* sampler, CAssimpModel* model, int bands)
{
	Grid gridAccel(model);

	for (int i = 0; i < model->vertices.size(); i++)
		for (int j = 0; j < bands*bands; j++)
			coeffs[i][j].r = coeffs[i][j].g = coeffs[i][j].b = 0.0f;

	for (int i = 0; i < model->vertices.size(); i++)
	{
		//printf("%d\n", j);
		for (int j = 0; j < sampler->number_of_samples; j++)
		{
			Sample& sample = sampler->samples[j];
			if (Visibility(model, i, sample.cartesian_coord, &gridAccel))
			{
				float cosine_term = glm::dot(model->vertices[i].m_normal, sample.cartesian_coord);
				for (int k = 0; k < bands*bands; k++)
				{
					float sh_function = sample.sh_functions[k];
					//int materia_idx = model->vertices[i].m_tex[i];
					//Color& albedo = model->albedo[materia_idx];
					//coeffs[i][k].r += (albedo.r * sh_function * cosine_term);
					//coeffs[i][k].g += (albedo.g * sh_function * cosine_term);
					//coeffs[i][k].b += (albedo.b * sh_function * cosine_term);
					coeffs[i][k].r += (1.f * sh_function * cosine_term);
					coeffs[i][k].g += (1.f * sh_function * cosine_term);
					coeffs[i][k].b += (1.f * sh_function * cosine_term);
				}
			}
		}
	}
	float weight = 4.0f*PI;
	float scale = weight / sampler->number_of_samples;
	for (int i = 0; i < model->vertices.size(); i++) {
		for (int j = 0; j < bands*bands; j++) {
			coeffs[i][j] *= scale;
		}
	}

}

void computeColor(CAssimpModel* model, Color* light, Color** coeffs, int bands)
{
	for (int i = 0; i < model->vertices.size(); i++) {
		Color& c = model->vertices[i].m_color;
		for (int j = 0; j < bands*bands; j++) {
			c.r += light[j].r * coeffs[i][j].r;
			c.g += light[j].g * coeffs[i][j].g;
			c.b += light[j].b * coeffs[i][j].b;
		}
	}
}