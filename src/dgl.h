#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "dglKeyboardStates.h"

typedef struct {
	int libsReady;
} DGLContext;

typedef GLFWwindow DGLWindow;

DGLContext* dglCreateContext();

DGLWindow* dglCreateWindow(DGLContext*, int, int, const char*);

int dglPrintErrors();
