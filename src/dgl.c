#include "dgl.h"

DGLContext* dglCreateContext() {
	if (!glfwInit()) {
		fprintf(stderr, "error: failed to init GFLW");
		return 0;
	}
	DGLContext* ret = malloc(sizeof(DGLContext));
	ret->libsReady = 0;
	return ret;
}

DGLWindow* dglCreateWindow(DGLContext* context, int width, int height, const char* title) {
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!context->libsReady) {
		context->libsReady = gladLoadGL();
	}
	dglPrepareKeys();
	glfwSetKeyCallback(window, dglKeyCallback);
	glfwSetMouseButtonCallback(window, dglMouseButtonCallback);
	return window;
}

void dglGetCursorPosition(DGLWindow* window, float* xp, float* yp) {
	double x, y;
	int w, h;
	glfwGetCursorPos(window, &x, &y);
	glfwGetWindowSize(window, &w, &h);
	*xp = (float) x / (float) w;
	*yp = (float) y / (float) h;
	*xp *= 2;
	*yp *= 2;
	*xp -= 1;
	*yp -= 1;
}

int dglPrintErrors() {
	int ret = 0;
	GLenum error = glGetError();
	while (error) {
		if (error == GL_INVALID_ENUM) {
			fprintf(stderr, "error: GL_INVALID_ENUM\n");
		} else if (error == GL_INVALID_VALUE) {
			fprintf(stderr, "error: GL_INVALID_VALUE\n");
		} else if (error == GL_INVALID_OPERATION) {
			fprintf(stderr, "error: GL_INVALID_OPERATION\n");
		} else if (error == GL_INVALID_ENUM) {
			fprintf(stderr, "error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
		} else if (error == GL_INVALID_ENUM) {
			fprintf(stderr, "error: GL_OUT_OF_MEMORY\n");
		} else if (error == GL_INVALID_ENUM) {
			fprintf(stderr, "error: GL_STACK_UNDERFLOW\n");
		} else if (error == GL_INVALID_ENUM) {
			fprintf(stderr, "error: GL_STACK_OVERFLOW\n");
		} else {
			fprintf(stderr, "error: unknown GL error %x\n", error);
		}
		error = glGetError();
		++ret;
	}
	return ret;
}
