#ifndef _UTILS_H_
#define _UTILS_H_

#include <assimp/cimport.h>		// C importer
#include <assimp/scene.h>		// collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h>				// memory management
#include <chrono>
#include "prt.h"
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

typedef std::chrono::high_resolution_clock Clock;

#endif