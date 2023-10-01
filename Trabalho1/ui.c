#include "ui.h"
#include "shaders.h"
#include <glad/glad.h>
#include <FreeImage.h>
#include "graphics.h"

#define GUI_SHADER_PATH_FRAGMENT "../resource/shaders/GUI_FragmentShader.fs"
#define GUI_SHADER_PATH_VERTEX "../resource/shaders/GUI_VertexShader.vs"

// Vetor de vertices das linhas da UI
// 4 vertices {x, y} 
// 4 cores {R, G, B} 
// 4 coordenadas de textura {x,y}
// total por quadrado = 4 * 7 vertices+cores+tex coords {x,y,r,g,b,xtex,ytex} = 28 floats
float vertices[280];
unsigned int numIcons = 0;

XSHADER uiShader;

unsigned int uiVBO, uiVAO;

// 10 quadrados
unsigned int uiEBO;

// 10 quadrados com 2 triangulos cada = 6 vertices cada
unsigned int indicesUISquares[60];

unsigned int textures[10];


int UI_addIcon(const char* icon_path)
{
	if (numIcons > 9)
	{
		printf("ERROR::UI::ADDICON - Numero maximo de icones alcan�ado\n");
		return -1;
	}

	numIcons++;

	FIBITMAP* bitmap = FreeImage_Load(
		FreeImage_GetFileType(icon_path, 0),
		icon_path, 0);

	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	unsigned int width = FreeImage_GetWidth(pImage);
	unsigned int height = FreeImage_GetHeight(pImage); 

	glGenTextures(1, &(textures[numIcons - 1]));
	glBindTexture(GL_TEXTURE_2D, textures[numIcons - 1]);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	glGenerateMipmap(GL_TEXTURE_2D);
	
	//free(data);
	FreeImage_Unload(bitmap);

	return 0;
}

int initUI()
{

	for (int i = 0; i < 10; i++)
	{
		// Gera os vertices dos icones
		indicesUISquares[i * 6] = i * 4;//numIcons * 2;
		indicesUISquares[i * 6 + 1] = i * 4 + 2;//numIcons * 2 + 2;
		indicesUISquares[i * 6 + 2] = i * 4 + 1;//numIcons * 2 + 1;
		indicesUISquares[i * 6 + 3] = i * 4 + 2;//numIcons * 2 + 2;
		indicesUISquares[i * 6 + 4] = i * 4 + 3;//numIcons * 2 + 3;
		indicesUISquares[i * 6 + 5] = i * 4 + 1;//numIcons * 2 + 1;

		vertices[i * 28] = 0.9f; // x
		vertices[i * 28 + 1] = 1.0f - 0.1f * i * 2; // y
		vertices[i * 28 + 2] = 0.6f; // r
		vertices[i * 28 + 3] = 0.6f; // g
		vertices[i * 28 + 4] = 0.6f; // b
		vertices[i * 28 + 5] = 0.0f; // x tex
		vertices[i * 28 + 6] = 1.0f; // y tex

		vertices[i * 28 + 7] = 1.0f; // x
		vertices[i * 28 + 8] = 1.0f - 0.1f * i * 2; // y
		vertices[i * 28 + 9] = 0.6f; // r
		vertices[i * 28 + 10] = 0.6f; // g
		vertices[i * 28 + 11] = 0.6f; // b
		vertices[i * 28 + 12] = 1.0f; // x tex
		vertices[i * 28 + 13] = 1.0f; // y tex

		vertices[i * 28 + 14] = 0.9f; // x
		vertices[i * 28 + 15] = 1.0f - 0.1f * (i + 1) * 2; // y
		vertices[i * 28 + 16] = 0.6f; // r
		vertices[i * 28 + 17] = 0.6f; // g
		vertices[i * 28 + 18] = 0.6f; // b
		vertices[i * 28 + 19] = 0.0f; // x tex
		vertices[i * 28 + 20] = 0.0f; // y tex

		vertices[i * 28 + 21] = 1.0f; // x
		vertices[i * 28 + 22] = 1.0f - 0.1f * (i + 1) * 2; // y
		vertices[i * 28 + 23] = 0.6f; // r
		vertices[i * 28 + 24] = 0.6f; // g
		vertices[i * 28 + 25] = 0.6f; // b
		vertices[i * 28 + 26] = 1.0f; // x tex
		vertices[i * 28 + 27] = 0.0f; // y tex
	}

	glGenBuffers(1, &uiVBO);

	glGenVertexArrays(1, &uiVAO);

	glBindVertexArray(uiVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &uiEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesUISquares), indicesUISquares, GL_STATIC_DRAW);


	// Seta os atributos dos vertices - posicao
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Seta os atributos dos vertices - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	if (initShader(GUI_SHADER_PATH_VERTEX, GUI_SHADER_PATH_FRAGMENT, &uiShader) != SHADER_SUCCESS)
	{
		printf("ERROR::GUI::INIT - Erro de inicializa��o de shader\n");
		//free(vertices);
		return -1;
	}

	

	return 0;
}

void drawUI()
{
	if(!numIcons)
	{
		return;
	}
	useShader(uiShader);
	
	glBindVertexArray(uiVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (unsigned int i = 0; i < numIcons; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*i*6));
	}
	glBindVertexArray(0);
}