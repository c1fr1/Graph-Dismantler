#pragma once

#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>

#include "hackFont.h"

//lut = lookup table

int* makeFontLookupTable(Font*);

void getCharPosition(vec4, Font*, int*, char);

void getCharMat(mat4, Font*, int*, char, float*, float);

void setupUniforms(Font*, int*, mat4*, vec4*, char*, float, float, float);

/*
 * need to bind font details to the shader
 * mat4 of the position of the font 
 * vec4 containing the character position in the texture
 * (x_0, y_0, width, height)
 */
