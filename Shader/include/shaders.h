#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct XSHADER{
	unsigned int ID;
} XSHADER;

enum {
	SHADER_SUCCESS = 0,
	SHADER_ERROR = -1
};

extern const char* vertexShaderSource;
extern const char* fragmentShaderSource;

int initShader(const char* vertexPath, const char* fragmentPath, XSHADER *shader);
void useShader(XSHADER shader);
void setBool(const char* name, int value, XSHADER shader);
void setInt(const char* name, int value, XSHADER shader);
void setFloat(const char* name, float value, XSHADER shader);

#endif
