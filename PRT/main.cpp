#include "GLWindow.h"
#include "global.h"
#include "prt.h"
#include "utils.h"
#include "CAssimpModel.h"
#include "ShaderProgram.h"

// PRT
Color* light;
Color** coeffs;
//CAssimpModel model;
const int bands = 4; 
const int numSamples = 10;


int main()
{
	GLWindow window(480, 480);
	CAssimpModel model;
	//model.LoadModelFromFile("models/happy_vrip_res2.obj");
	model.LoadModelFromFile("models/bunny.obj");
	//model.LoadModelFromFile("models/head_sad.x");

#if 1
	// load resources
	srand(time(NULL));
	cout << "Loading resources\n";
	cout << "   Loading Light probe...\n";
	Image img;			// image for light probe				
	img.loadFromFile("grace_probe.jpg");
	cout << "  Loading Model...\n";
	//model.LoadModelFromFile("models/bunny.obj");
	//scene.loadMeshFromFile("models/Head_Sad.x");			// get mesh
	cout << "     # of vertices  = " << model.vertices.size() << endl;
	cout << "     # of triangles = " << model.indices.size() / 3 << endl;
	//Color white(1, 1, 1);
	//Color black(0, 0, 0);
	//Color grey(0.5f, 0.5f, 0.5f);
	//scene.albedo = &white;
	//for (int i = 0; i < scene.number_of_vertices; i++)
	//	scene.material[i] = 0;
	
	light = new Color[bands*bands];
	coeffs = new Color*[model.vertices.size()];
	for (int i = 0; i < model.vertices.size(); i++)
		coeffs[i] = new Color[bands*bands];

	cout << "Preprocess\n";
	auto startTime = Clock::now();

	Sampler sampler;
	cout << "  Generate Samples...\n";
	GenerateSamples(&sampler, numSamples);

	cout << "  Precompute SH Functions...\n";
	PrecomputeSHFunctions(&sampler, bands);

	cout << "  Project Light Function...\n";
	ProjectLightFunction(light, &sampler, &img, bands);	// get light

	cout << "  Project Unshadowed...\n";
	//ProjectUnshadowed(coeffs, &sampler, &model, bands);	// get coeffs
	ProjectShadowed(coeffs, &sampler, &model, bands);

	auto endTime = Clock::now();
	auto ms = duration_cast<milliseconds>(endTime-startTime).count();
	cout << "Preproc time: " << ms << " ms" << '\n';

#endif
	// PART 2 Runtime
	
	computeColor(&model, light, coeffs, bands);
	window.compileShader("shader/unShadowed.vert", "shader/unShadowed.frag");
	window.prog.printActiveAttribs();
	window.prog.printActiveUniforms();
	model.meshEntry.bindBuffer(model.vertices, model.indices);
	window.setUniform();
	window.renderModel(&model);
	//window.renderPRT(&model, light, coeffs, bands);
	return 0;
}