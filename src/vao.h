#pragma once

#include "dgl.h"

GLuint makeRectangleVAO(float x, float y, float width, float height);

GLuint makeVAO(GLfloat* vertices, int vertCount, GLuint* indices, int indexCount);
