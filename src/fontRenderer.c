#include "fontRenderer.h"

//lut = lookup table

int* makeFontLookupTable(Font* font) {
	char maxChar = 0;
	for (int i = 0; i < font->characterCount; ++i) {
		if (font->characters[i].codePoint > maxChar) {
			maxChar = font->characters[i].codePoint;
		}
	}
	maxChar += 1;
	int* lookupTable = malloc(maxChar * sizeof(int));
	for (int i = 0; i < font->characterCount; ++i) {
		lookupTable[font->characters[i].codePoint] = i;
	}
	return lookupTable;
}

void getCharPosition(vec4 targ, Font* f, int* lut, char c) {
	Character cObj = f->characters[lut[c]];
	targ[0] = (float) cObj.x / (float) f->width;
	targ[1] = (float) cObj.y / (float) f->height;
	targ[2] = (float) cObj.width / (float) f->width;
	targ[3] = (float) cObj.height / (float) f->height;
}

void getCharMat(mat4 targ, Font* f, int* lut, char c, float* x, float y) { 
	Character cObj = f->characters[lut[c]];
	glm_mat4_identity(targ);
	vec3 scale;
	scale[0] = (float) cObj.width;
	scale[1] = (float) cObj.height;
	scale[2] = 1.0;

	vec3 translate;
	translate[0] = *x + (float) cObj.originX;
	translate[1] = y + (float) cObj.originY;
	translate[2] = 0;

	glm_translate(targ, translate);
	glm_scale(targ, scale);
	*x += cObj.advance;
}

void setupUniforms(Font* f, int* lut, mat4* mats, vec4* vecs, char* text, float x, float y, float scale) {
	int i;
	vec3 scalev;
	scalev[0] = scale;
	scalev[1] = scale;
	scalev[2] = 1;
	while (text[i]) {
		getCharMat(mats[i], f, lut, text[i], &x, y);
		glm_scale(mats[i], scalev);
		getCharPosition(vecs[i], f, lut, text[i]);
		++i;
	}
}

/*
 * need to bind font details to the shader
 * mat4 of the position of the font 
 * vec4 containing the character position in the texture
 * (x_0, y_0, width, height)
 */
