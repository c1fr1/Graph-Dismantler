#pragma once

#include "dgl.h"

unsigned char* loadTextureData(const char* path, int* width, int* height);

void freeTextureData(unsigned char*);

GLuint makeTexture(const char* path);

GLuint makeTextureFromData(unsigned char* data, int width, int height);

GLuint bindTextureToPosition(GLuint tex, int position);

