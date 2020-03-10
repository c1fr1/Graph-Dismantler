#pragma once

#include "dgl.h"

GLuint makeTexture(const char* path);

GLuint makeTextureFromData(unsigned char* data, int width, int height);

GLuint bindTextureToPosition(GLuint tex, int position);

