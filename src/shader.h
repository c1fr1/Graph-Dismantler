#pragma once

#include "GL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLuint createShader(const char* path, GLenum type);

GLuint createShaderProgram(const char* path);
