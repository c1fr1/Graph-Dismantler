#include "dglKeyboardStates.h"

char KEY_STATES[GLFW_KEY_LAST + 1];

void dglPrepareKeys() {
	for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
		KEY_STATES[i] = 0;
	}
}

void dglKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key > 0 && key <= GLFW_KEY_LAST) {
		if (action == GLFW_PRESS) {
			KEY_STATES[key] = DGL_KEY_PRESSED;
		} else if (action == GLFW_RELEASE) {
			KEY_STATES[key] = DGL_KEY_RELEASED;
		}
	}
}

int dglGetKeyState(char key) {
	return KEY_STATES[key];
}

int dglGetKeyCodeState(int key) {
	return KEY_STATES[key];
}

void dglUpdateKeys() {
	for (int i = 0; i <= GLFW_KEY_LAST; ++i) {
		if (KEY_STATES[i] & 1) {
			KEY_STATES[i] -= 1;
		}
	}
}
