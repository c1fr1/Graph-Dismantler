#include "vao.h"

GLuint makeRectangleVAO(float x, float y, float width, float height) {
	GLfloat vertices[12] = {
		x        , y         , 0.0,
		x        , y + height, 0.0,
		x + width, y         , 0.0,
		x + width, y + height, 0.0,
	};
	GLfloat textureCoordinates[8] = {
		0.0, 1.0,
		0.0, 0.0,
		1.0, 1.0,
		1.0, 0.0,
	};
	GLuint indices[6] = {
		0, 1, 2,
		2, 1, 3,
	};

	GLuint buffers[3];
	GLuint ret;

	glGenVertexArrays(1, &ret);
	glBindVertexArray(ret);

	glGenBuffers(3, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);//position buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), textureCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	return ret;
}

GLuint makeVAO(GLfloat* vertices, int vertCount, GLuint* indices, int indexCount) {
	GLuint buffers[2];
	GLuint ret;

	glGenVertexArrays(1, &ret);
	glBindVertexArray(ret);

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

	return ret;
}
