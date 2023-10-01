// Shaders de OpenGL
#include "shaders.h"
#include <glad/glad.h>
#include <stdio.h>


int initShader(const char* vertexPath, const char* fragmentPath, XSHADER* shader)
{
	FILE* ptr;
	int err;
    unsigned char* shaderContents = malloc(512);

	if (shaderContents == NULL) return -1;

    ptr = fopen( vertexPath, "r");

    if (ptr == NULL)
	{
		printf("ERRO::SHADER::INITSHADER - Erro ao abrir vertex shader\n");
		if(ptr != NULL)
			fclose(ptr);

		free(shaderContents);
		return -1;
	}

	int index = 0;

	while (!feof(ptr))
	{
		shaderContents[index++] = fgetc(ptr);

		if(index > 500)
		{
			printf("ERRO::SHADER::INITSHADER - Nao foi possivel ler o arquivo de vertex shader\n");
			fclose(ptr);
			free(shaderContents);
			return -1;
		}

	}

	fclose(ptr);

	if(index > 0)
	{
		shaderContents[index-1] = '\0';
	}
	else 
	{
		printf("ERRO::SHADER::INITSHADER - Nao foi possivel ler o arquivo de vertex shader\n");
		free(shaderContents);
		return -1;
	}

	shaderContents[index] = '\0';

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &shaderContents, &index);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		printf(infoLog);
		printf("\n");
		free(shaderContents);
		return -1;
	}

    ptr = fopen(fragmentPath, "r");

    if (ptr == 0)
	{
		printf("ERRO::SHADER::INITSHADER - Erro ao abrir fragment shader\n");
		fclose(ptr);
		free(shaderContents);
		glDeleteShader(vertex);
		return -1;
	}

	index = 0;

	while (!feof(ptr))
	{
		shaderContents[index++] = fgetc(ptr);

		if (index > 500)
		{
			printf("ERRO::SHADER::INITSHADER - Nao foi possivel ler o arquivo de fragment shader\n");
			fclose(ptr);
			free(shaderContents);
			glDeleteShader(vertex);
			return -1;
		}

	}

	if(index > 0)
		shaderContents[index-1] = '\0';

	fclose(ptr);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &shaderContents, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
		printf(infoLog);
		printf("\n");
		free(shaderContents);
		glDeleteShader(vertex);
		return -1;
	}

	unsigned int ID;

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
		printf(infoLog);
		printf("\n");
		free(shaderContents);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return -1;
	}
	else shader->ID = ID;

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return 0;
}

void useShader(XSHADER shader)
{
	glUseProgram(shader.ID);
}

void setBool(const char* name, int value, XSHADER shader)
{
	glUniform1i(glGetUniformLocation(shader.ID, name), (int)value);
}
void setInt(const char* name, int value, XSHADER shader)
{
	glUniform1i(glGetUniformLocation(shader.ID, name), value);
}
void setFloat(const char* name, float value, XSHADER shader)
{
	glUniform1f(glGetUniformLocation(shader.ID, name), value);
}
