#include "dgl.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <poll.h>

#include <cglm/cglm.h>

#include "socket.h"

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

#define MAX_PACKET_SIZE 65535
//65535

typedef struct {
	// 0-1 = towers alive left
	// 2-3 = towers alive right
	char sideLaneStates;
	// 0-1 = towers alive mid
	// 2   = left tower alive
	// 3   = right tower alive
	// 4   = mid tower alive
	char mainState;
	// 0-2 = distance from ancient
	// 3-5 = direction enum
	// 6   = alive
	char attacks[2];
} PlayerState;

typedef struct {
	PlayerState players[4];
	int playersTurn;
	char scores[4];
	int userIndex;
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

void initConnection(int* sockp, int* sessionp) {
	*sessionp = initSocket();
	*sockp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket < 0) {
		fprintf(stderr, "error: failed to create socket (%d)\n", socket);
		exit(-1);
	}
	struct sockaddr_in sad = {
		AF_INET,
		htons(4731),
		inet_addr("209.180.172.173")
	};
	int connection = connect(*sockp, (struct sockaddr*) &sad, sizeof(sad));
	if (connection < 0) {
		fprintf(stderr, "error: failed to connect\n");
		exit(-1);
	}
}

void readAllBytes(int fd, unsigned char* buffer, int bytesToRead) {
	while (bytesToRead > 0) {
		int bytesRead = read(fd, buffer, bytesToRead);
		buffer += bytesRead;
		bytesToRead -= bytesRead;
	}
}

void handleConnection(BoardState* board, int fd) {
	struct pollfd pollfd[1];
	pollfd[0].fd = fd;
	pollfd[0].events = POLLIN | POLLHUP; 
	poll(pollfd, 1, 2);
	if (pollfd[0].revents & POLLIN) {
		static byte buffer[MAX_PACKET_SIZE];
		int bytesRead = read(fd, buffer, MAX_PACKET_SIZE);

		byte* data = buffer;
		while (bytesRead > 0) {
			byte packetType = data[0];
			if (packetType == 0) {
				//new connection

				int pfpPlayer = data[1];
				int pfpWidth = data[2] << 8;
				pfpWidth += data[3];
				int pfpHeight = data[4] << 8;
				pfpHeight += data[5];
				
				printf("%x%xx%x%x\n", data[2], data[3], data[4], data[5]);

				printf("debug: new connection at index %d, pfp is %dx%d\n", pfpPlayer, pfpWidth, pfpHeight);
				int pfpSize = pfpWidth * pfpHeight * 3;
				byte* newPfpData = malloc(pfpSize);

				if (pfpSize + 6 <= bytesRead) {
					memcpy(newPfpData, data + 6, pfpSize - 6);//invalid read
					printf("debug: full pfp sent (%d bytes)\n", bytesRead);
					data += pfpSize;
					bytesRead -= pfpSize;
				} else {
					memcpy(newPfpData, data + 6, bytesRead - 6);
					readAllBytes(fd, newPfpData + bytesRead - 6, pfpSize - bytesRead + 6);
					bytesRead = 0;
				}
				board->playerTextures[pfpPlayer] = makeTextureFromData(newPfpData, pfpWidth, pfpHeight);
				free(newPfpData);
			} else if (packetType == 1) {
				//new disconnection
				glDeleteTextures(1, board->playerTextures + data[1]);
				board->playerTextures[data[1]] = 0;
				data += 2;
				bytesRead -= 2;
			} else if (packetType == 2) {
				//update board
				for (int i = 0;i < 4; ++i) {
					board->players[i].sideLaneStates = data[4 * i + 1];
					board->players[i].mainState = data[4 * i + 2];
					board->players[i].attacks[0] = data[4 * i + 3];
					board->players[i].attacks[1] = data[4 * i + 4];
				}
				board->playersTurn = data[17];
				memcpy(board->scores, data + 18, 4);
				board->userIndex = data[22];
				bytesRead -= 23;
				data += 23;
			} else {
				fprintf(stderr, "error: invalid packet from server\n");
				//data[bytesRead] = 0;
				//printf("packet: [%s]\n", data);
				bytesRead = 0;
			}
		}
	} else if (pollfd[0].revents & POLLHUP) {
		fprintf(stderr, "error: lost connection to server\n");
		exit(-1);
	}
}

int main(int argc, char** argv) {
	//load pfp
	int pfpWidth, pfpHeight, pfpChannels;
	byte* pfpData = loadTextureData("res/pfp.png", &pfpWidth, &pfpHeight);
	printf("debug: loaded texture, %dx%dpx\n", pfpWidth, pfpHeight, pfpChannels);

	//setup game and connect
	BoardState* board = makeBoard();
	int fd, socket;
	initConnection(&fd, &socket);

	{
		int pfpSize = pfpWidth * pfpHeight * 3;
		byte* packet = malloc(4 + pfpSize);
		packet[0] = pfpWidth >> 8;
		packet[1] = pfpWidth & 0xFF;
		packet[2] = pfpHeight >> 8;
		packet[3] = pfpHeight & 0xFF;
		memcpy(packet + 4, pfpData, pfpSize);
		printf("debug: packet size = %d\n", 4 + pfpSize);
		//sendSegmented(fd, packet, 4 + pfpSize, 200);
		send(fd, packet, 4 + pfpSize, 0);
		free(packet);
	}
	freeTextureData(pfpData);

	{
		byte buffer[22];
		read(fd, buffer, 22);
		for (int i = 0; i < 4; ++i) {
			board->players[i].sideLaneStates = buffer[4 * i];
			board->players[i].mainState = buffer[4 * i + 1];
			board->players[i].attacks[0] = buffer[4 * i + 2];
			board->players[i].attacks[1] = buffer[4 * i + 3];
		}
		board->playersTurn = buffer[16];
		memcpy(board->scores, buffer + 17, 4);
		board->userIndex = buffer[21];
	}

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

	dglPrintErrors();
	
	//set up matrices
	mat4 pfpMats[4];
	mat4 nodePositions[44];
	vec3 nodeColors[44];
	
	for (int i = 0; i < 4; ++i) {
		vec3 pos = {-1.0, 1.0 - 0.3 * (float) i};
		glm_translate_make(pfpMats[i], pos);
		glm_scale_uni(pfpMats[i], 0.3);
	}

	setNodePositions(nodePositions);
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
		/*glUniformMatrix4fv(pfpMatPos, 1, GL_FALSE, (GLfloat*) pfpMats);
		bindTextureToPosition(tex, 0);
		glBindVertexArray(pfpVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
		
		glBindVertexArray(pfpVAO);
		for (int i = 0; i < 4; ++i) {
			if (board->playerTextures[i]) {
				glUniformMatrix4fv(pfpMatPos, 1, GL_FALSE, (GLfloat*) pfpMats[i]);
				bindTextureToPosition(board->playerTextures[i], 0);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		//render graph
		glUseProgram(colorShader);
		glUniform3f(colorPos, 1.0, 1.0, 1.0);
		glBindVertexArray(graphLinesVAO);
		glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT, 0);
		
		//render nodes
		glUseProgram(nodeShader);
		glUniformMatrix4fv(nodeTransformPos, 44, GL_FALSE, (GLfloat*) nodePositions);
		glUniform3fv(nodeColorPos, 44, (GLfloat*) nodeColors);
		glBindVertexArray(nodeVAO);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 44);

		glfwSwapBuffers(window);
		dglPrintErrors();
		handleConnection(board, fd);
		dglUpdateKeys();
		glfwPollEvents();
		if (dglGetKeyCodeState(GLFW_KEY_ESCAPE) == 1) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glDeleteShader(textureShader);

	glfwTerminate();
	
	closeSocket(fd);
	quitSocket(socket);

	return 0;
}

/*
 * add input functionality
 * organise net interaction
 */
