#include "dgl.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <cglm/cglm.h>

#include "shader.h"
#include "texture.h"
#include "vao.h"

#define TOWERS_ALIVE_L(a) (a & 3)
#define TOWERS_ALIVE_M(a) (a & 3)
#define TOWERS_ALIVE_R(a) ((a << 2) & 3)

#define TOWER_ALIVE_L(a) (a & (1 >> 3))
#define TOWER_ALIVE_M(a) (a & (1 >> 4))
#define TOWER_ALIVE_R(a) (a & (1 >> 5))

#define ATTACK_ALIVE(a) (a & 6)

#define ATTACK_DISTANCE(a) (a & 7)
#define ATTACK_DIRECTION(a) ((a << 3) & 7)

#define ATTACK_DIRECTION_LL 0
#define ATTACK_DIRECTION_ML 1
#define ATTACK_DIRECTION_MM 2
#define ATTACK_DIRECTION_MR 3
#define ATTACK_DIRECTION_RR 4

typedef struct {
	// 0-1 = towers alive left
	// 2-3 = towers alive right
	char sideLaneStates;
	// 0-1 = towers alive mid
	// 2   = left tower alive
	// 3   = mid tower alive
	// 4   = right tower alive
	char mainState;
	// 0-2 = distance from ancient
	// 3-5 = direction enum
	// 6   = alive
	char attacks[2];
} PlayerState;

typedef struct {
	PlayerState players[4];
	GLuint playerTextures[4];
} BoardState;

BoardState* makeBoard() {
	BoardState* ret = malloc(sizeof(BoardState));
	for (int i = 0; i < 4; ++i) {
		ret->playerTextures[i] = 0;
		ret->players[i].sideLaneStates = 0xFF;
		ret->players[i].mainState = 0xFF;
		ret->players[i].attacks[0] = 0;
		ret->players[i].attacks[1] = 0;
	}
	return ret;
}

GLuint makeGraphLinesVAO() {
	float graphLineVerts[24] = {
		-0.5, -0.5, 0.0,
		-0.5, 0.5 , 0.0,
		0.5 , -0.5, 0.0,
		0.5 , 0.5 , 0.0,

		-0.3, -0.3, 0.0,
		-0.3, 0.3 , 0.0,
		0.3 , -0.3, 0.0,
		0.3 , 0.3 , 0.0,
	};

	int graphLineIndices[20] = {
		0, 1,
		1, 3,
		3, 2,
		2, 0,

		0, 3,
		1, 2,

		4, 5,
		5, 7,
		7, 6,
		6, 4,
	};
	return makeVAO(graphLineVerts, 24, graphLineIndices, 20);
}

GLuint makeNodeVAO() {
	GLfloat verts[12] = {
		0.0   , 0.015 , 0.0,
		0.015 , 0.0   , 0.0,
		0.0   , -0.015, 0.0,
		-0.015, 0.0   , 0.0,
	};
	GLuint indices[6] = {
		0, 1, 2,
		3, 0, 2,
	};
	return makeVAO(verts, 12, indices, 6);
}

void setNodePositions(mat4* poss) {
	//0-2 = left
	//3-5 = right
	//6-7 = mid
	//8-10 = mid lrm
	
	mat4 identity = GLM_MAT4_IDENTITY_INIT;
	
	for (int i = 0; i < 11; ++i) {
		glm_mat4_copy(identity, poss[i]);
	}

	float rot = 0.5;
	for (int i = 11; i < 44; i += 11) {
		for (int j = 0; j < 11; ++j) {
			glm_rotate_z(identity, M_PI * rot, poss[i + j]);
		}
		rot += 0.5;
	}

	for (int i = 0; i < 44; i += 11) {
		{
			vec3 scratch = {0.5, 0.4, 0.0};
			glm_translate(poss[0 + i], scratch);
		}
		{
			vec3 scratch = {0.5, 0.3, 0.0};
			glm_translate(poss[1 + i], scratch);
		}
		{
			vec3 scratch = {0.5, 0.15, 0.0};
			glm_translate(poss[2 + i], scratch);
		}
		{
			vec3 scratch = {0.4, 0.5, 0.0};
			glm_translate(poss[3 + i], scratch);
		}
		{
			vec3 scratch = {0.3, 0.5, 0.0};
			glm_translate(poss[4 + i], scratch);
		}
		{
			vec3 scratch = {0.15, 0.5, 0.0};
			glm_translate(poss[5 + i], scratch);
		}
		{
			vec3 scratch = {0.4, 0.4, 0.0};
			glm_translate(poss[6 + i], scratch);
		}
		{
			vec3 scratch = {0.3, 0.3, 0.0};
			glm_translate(poss[7 + i], scratch);
		}
		{
			vec3 scratch = {0.3, 0.15, 0.0};
			glm_translate(poss[8 + i], scratch);
		}
		{
			vec3 scratch = {0.15, 0.3, 0.0};
			glm_translate(poss[9 + i], scratch);
		}
		{
			vec3 scratch = {0.15, 0.15, 0.0};
			glm_translate(poss[10 + i], scratch);
		}
	}
}

void setNodeColors(vec3* nodes) {
	for (int i = 0; i < 11; ++i) {
		nodes[i][0] = 1.0;
		nodes[i][1] = 0.0;
		nodes[i][2] = 0.0;
	}
	for (int i = 0; i < 11; ++i) {
		nodes[11 + i][0] = 0.0;
		nodes[11 + i][1] = 1.0;
		nodes[11 + i][2] = 0.0;
	}
	for (int i = 0; i < 11; ++i) {
		nodes[22 + i][0] = 0.0;
		nodes[22 + i][1] = 0.0;
		nodes[22 + i][2] = 1.0;
	}
	for (int i = 0; i < 11; ++i) {
		nodes[33 + i][0] = 1.0;
		nodes[33 + i][1] = 1.0;
		nodes[33 + i][2] = 0.0;
	}
}

int main(int argc, char** argv) {
	//create window
	DGLContext* context = dglCreateContext();
	DGLWindow* window = dglCreateWindow(context, 800, 800, "DGL test");
	printf("debug: OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
	glClearColor(0.1, 0.0, 0.0, 1.0);

	//create openGL objects
	GLuint colorShader = createShaderProgram("src/shaders/color/");
	GLuint textureShader = createShaderProgram("src/shaders/texture/");
	GLuint nodeShader = createShaderProgram("src/shaders/test/");

	GLuint pfpVAO = makeRectangleVAO(0.0, -1.0, 1.0, 1.0);
	GLuint graphLinesVAO = makeGraphLinesVAO();
	GLuint nodeVAO = makeNodeVAO();

	GLuint tex = makeTexture("res/pfp.png");

	dglPrintErrors();
	
	//set up matrices
	mat4 pfpMat = GLM_MAT4_IDENTITY_INIT;
	glm_translate_x(pfpMat, -1.0);
	glm_translate_y(pfpMat, 1.0);
	glm_scale_uni(pfpMat, 0.1);

	mat4 nodePositions[44];
	setNodePositions(nodePositions);
	vec3 nodeColors[44];
	setNodeColors(nodeColors);

	//find uniform positions
	GLuint pfpMatPos = glGetUniformLocation(textureShader, "transformation");
	GLuint colorPos = glGetUniformLocation(colorShader, "inColor");
	GLuint nodeTransformPos = glGetUniformLocation(nodeShader, "transforms");
	GLuint nodeColorPos = glGetUniformLocation(nodeShader, "colors");

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
	
		//render pfp
		glUseProgram(textureShader);
		glUniformMatrix4fv(pfpMatPos, 1, GL_FALSE, (GLfloat*)pfpMat);
		bindTextureToPosition(tex, 0);
		glBindVertexArray(pfpVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//render graph
		glUseProgram(colorShader);
		glUniform3f(colorPos, 1.0, 1.0, 1.0);
		glBindVertexArray(graphLinesVAO);
		glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT, 0);
		
		//render nodes
		glUseProgram(nodeShader);
		glUniformMatrix4fv(nodeTransformPos, 44, GL_FALSE, (GLfloat*)nodePositions);
		glUniform3fv(nodeColorPos, 44, (GLfloat*) nodeColors);
		glBindVertexArray(nodeVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 44);

		glfwSwapBuffers(window);
		dglPrintErrors();
		sleep(1);
		dglUpdateKeys();
		glfwPollEvents();
		if (dglGetKeyCodeState(GLFW_KEY_ESCAPE) == 1) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glDeleteShader(textureShader);

	glfwTerminate();
	
	return 0;
}

/*
 * add input functionality
 * organise net interaction
 */
