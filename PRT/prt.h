#ifndef _PRT_H_
#define _PRT_H_
#include "global.h"
#include "CAssimpModel.h"
#include "SHEval.h"
#include "AccelerationStructure.h"

void GenerateSamples(Sampler* sampler, int N);
void PrecomputeSHFunctions(Sampler* sampler, int bands);
void LightProbeAccess(Color* color, Image* image, Vector3& direction);
void ProjectLightFunction(Color* coeffs, Sampler* sampler, Image* light, int bands);
void ProjectUnshadowed(Color** coeffs, Sampler* sampler, CAssimpModel* model, int bands);
void ProjectShadowed(Color** coeffs, Sampler* sampler, CAssimpModel* model, int bands);

void computeColor(CAssimpModel* model, Color* light, Color** coeffs, int bands);

#endif