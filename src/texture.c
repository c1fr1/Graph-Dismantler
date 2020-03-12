#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint makeTexture(const char* path) {
	int width, height, channels;
	unsigned char* imageData = stbi_load(path, &width, &height, &channels, 0);

	GLuint ret = makeTextureFromData(imageData, width, height);

	imageData[width * height * 3] = 1;

	stbi_image_free(imageData);
	return ret;
}

GLuint makeTextureFromData(unsigned char* data, int width, int height) {
	GLuint ret;

	glGenTextures(1, &ret);
	glBindTexture(GL_TEXTURE_2D, ret);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return ret;
}

GLuint bindTextureToPosition(GLuint tex, int position) {
	glActiveTexture(GL_TEXTURE0 + position);
	glBindTexture(GL_TEXTURE_2D, tex);
}

