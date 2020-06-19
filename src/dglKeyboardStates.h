#pragma once

#include "dgl.h"

//pressed is there for one frame, as is released, before going to the next state
#define DGL_KEY_PRESSED 	3
#define DGL_KEY_DOWN		2
#define DGL_KEY_RELEASED	1
#define DGL_KEY_UP			0

void dglPrepareKeys();

void dglKeyCallback(GLFWwindow*, int, int, int, int);

void dglMouseButtonCallback(GLFWwindow*, int, int, int);

int dglGetKeyState(char key);

int dglGetKeyCodeState(int key);

int dglGetMouseButtonState(int button); 

void dglUpdateKeys();
