#include "GLWindow.h"
#include "prt.h"
#include "utils.h"

// PRT
Color light;
Color** coeffs;
Scene scene;
const int bands = 5;
const int numSamples = 5;


int main()
{

#if 0
	// load resources
	srand(time(NULL));
	cout << "Loading resources\n";
	cout << "   Loading Light probe...\n";
	Image img;			// image for light probe				
	img.loadFromFile("uffizi_probe.jpg");
	cout << "  Loading Model...\n";
	scene.loadMeshFromFile("models/Head_Sad.x");			// get mesh
	cout << "     # of vertices  = " << scene.number_of_vertices << endl;
	cout << "     # of triangles = " << scene.number_of_triangles << endl;
	Color white(1, 1, 1);
	Color black(0, 0, 0);
	Color grey(0.5f, 0.5f, 0.5f);
	scene.albedo = &white;
	for (int i = 0; i < scene.number_of_vertices; i++)
		scene.material[i] = 0;
	//fScale = scene.aabb.max.x - scene.aabb.min.x;
	//fScale = aisgl_max(scene.aabb.max.y - scene.aabb.min.y, fScale);
	//fScale = aisgl_max(scene.aabb.max.z - scene.aabb.min.z, fScale);
	//fScale = 1.f/fScale * 2;
	
	coeffs = new Color*[scene.number_of_vertices];
	for (int i = 0; i < scene.number_of_vertices; i++)
		coeffs[i] = new Color[bands*bands];

	cout << "Preprocess\n";
	auto startTime = Clock::now();

	Sampler sampler;
	cout << "  Generate Samples...\n";
	GenerateSamples(&sampler, numSamples);

	cout << "  Precompute SH Functions...\n";
	PrecomputeSHFunctions(&sampler, bands);

	cout << "  Project Light Function...\n";
	ProjectLightFunction(&light, &sampler, &img, bands);	// get light

	cout << "  Project Unshadowed...\n";
	ProjectUnshadowed(coeffs, &sampler, &scene, bands);	// get coeffs
	//ProjectShadowed(coeffs, &sampler, &scene, bands);

	auto endTime = Clock::now();
	auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	cout << "Preproc time: " << ms << " ms" << '\n';

#endif

	// PART 2 Runtime
	GLWindow window;
	window.renderGL();
}