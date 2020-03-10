#include "shader.h"

GLuint createShader(const char* path, GLenum type) {
	FILE* file = fopen(path, "r");
	GLchar* source;
	GLuint ret;

#ifdef DGL_DEBUG
	if (file == NULL) {
		fprintf(stderr, "error: shader does not exist in path %s\n", path);
		return 0;
	}
#endif

	fseek(file, 0, SEEK_END);
	int fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);;
	source = malloc(sizeof(char) * (fileLen + 1));
	fread(source, fileLen, sizeof(char), file);
	source[fileLen] = 0;
	fclose(file);

	const GLchar* srcptr = (const GLchar*)source;
	
	ret = glCreateShader(type);
	glShaderSource(ret, 1, &srcptr, NULL);
	glCompileShader(ret);
	free(source);
	return ret;
}

GLuint createShaderProgram(const char* path) {
	int pathLength = strlen(path);
	char* srcPath = malloc(sizeof(char) * (pathLength + 5));
	memcpy(srcPath, path, sizeof(char) * pathLength);
	srcPath[pathLength] = 'v';
	srcPath[pathLength + 1] = 'e';
	srcPath[pathLength + 2] = 'r';
	srcPath[pathLength + 3] = 't';
	srcPath[pathLength + 4] = '\0';

	GLuint vertShader = createShader(srcPath, GL_VERTEX_SHADER);

	srcPath[pathLength] = 'f';
	srcPath[pathLength + 1] = 'r';
	srcPath[pathLength + 2] = 'a';
	srcPath[pathLength + 3] = 'g';

	GLuint fragShader = createShader(srcPath, GL_FRAGMENT_SHADER);

	if (!(vertShader && fragShader)) {
		fprintf(stderr, "error: failed to create shaders\n");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (!program) {
		fprintf(stderr, "error: failed to create program");
		return 0;
	}
	
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

#ifdef DGL_DEBUG
	int linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		fprintf(stderr, "error: failed to link program\n");
		int len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		char* log = malloc(len);
		glGetProgramInfoLog(program, len, &len, log);
		printf("%s\n", log);
		free(log);
		exit(-1);
	}

#endif
	
	return program;
}
