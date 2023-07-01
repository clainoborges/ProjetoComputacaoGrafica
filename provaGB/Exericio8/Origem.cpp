/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstdio>

 // GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Mesh.h"
#include "Camera.h"
#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"


// Protótipos das funções
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode); //função de callback para eventos de teclado.
void mouse_callback(GLFWwindow* window, double xpos, double ypos); //função de callback para eventos de mouse.
int setupGeometry(); //função para configurar a geometria.
int loadTexture(string path); //função para carregar uma textura.
void loadOBJ(string path); //função para carregar um arquivo OBJ.
void loadMTL(string path); //função para carregar um arquivo MTL.

vector<glm::vec3> generateControlPointsSet(const std::string& input); // função para gerar um conjunto de pontos de controle.
std::vector<glm::vec3> createCurve(float radius, int points, float speed = 1.0); //função para criar uma curva.

// Constantes
const GLuint WIDTH = 800, HEIGHT = 600;

//Flags para indicar a rotação da bola.
bool rotateX = false, rotateY = false, rotateZ = false;
vector<GLfloat> positions;
vector<GLfloat> textureCoords;
vector<GLfloat> normals;
vector<GLfloat> ka;
vector<GLfloat> ks;
float ns;

//Caminho do arquivo MTL e caminho da textura.
string mtlFilePath = "";
string texturePath = "";

//objetos do tipo Mesh para representar a bola e o objeto trump.
Mesh bola;
Mesh trump;

//variáveis para controlar a posição e o movimento da bola.
float bolaY = 0.0f;
float bolaX = 0.0f;
float moveBola = 0.2f;
float maxMove = 9.0f;

float escalaBola = 2.5f;
float radius = 4.0f;
float velocidadeTrump = 1.0f;
int points = 200;
float escalaTrump = 0.9f;

bool bolaSelecionada = false;
 
Camera camera;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criacao da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Grau B", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da funcao de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d funcoes da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	// Obtendo as informacoes de versao
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("hello.vs", "hello.fs");

	//Arquivos OBJ e MTL BOLA
	loadOBJ("bola.obj");
	loadMTL(mtlFilePath);

	// Setando VAO e VBO com textura
	GLuint textureBola = loadTexture(texturePath);
	GLuint VAObola = setupGeometry();

	glUseProgram(shader.ID);

	bola.initialize(VAObola, positions.size() / 3, &shader, textureBola, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(escalaBola));

	//Arquivos OBJ e MTL Trump
	loadOBJ("Trump.obj");
	loadMTL(mtlFilePath);

	// Setando VAO e VBO com textura
	GLuint textureTrump = loadTexture(texturePath);
	GLuint VAOtrump = setupGeometry();

	glUseProgram(shader.ID);

	trump.initialize(VAOtrump, positions.size() / 3, &shader, textureTrump, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(escalaTrump));

	camera.initialize(&shader, width, height, 0.05f,0.0f,-90.0f,glm::vec3(0.0f,0.0f, -1.0f), glm::vec3(1.5f, 0.0f, 15.0f));

	// Propriedades de material
	shader.setVec3("ka", ka[0], ka[1], ka[2]);
	shader.setFloat("kd", 0.5);
	shader.setVec3("ks", ks[0], ks[1], ks[2]);
	shader.setFloat("q", ns);

	// Definindo a fonte de luz pontual
	shader.setVec3("lightPos", -5.0f, 200.0f, 5.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	std::vector<glm::vec3> controlPoints = createCurve(radius, points, velocidadeTrump);

	Bezier bezier;
	bezier.setControlPoints(controlPoints);
	bezier.setShader(&shader);
	bezier.generateCurve(100);
	int nbCurvePoints = bezier.getNbCurvePoints();
	int i = 0;

	// Limpa o buffer de cor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funcoes de callback correspondentes

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		camera.update();

		bola.updatePosition(glm::vec3(bolaX, bolaY, 0.0f));
		bola.update();
		if (rotateX) {
			bola.rotate(1, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (rotateY) {
			bola.rotate(1, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (rotateZ) {
			bola.rotate(1, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		bola.draw();

		glm::vec3 pointOnCurve = bezier.getPointOnCurve(i);
		trump.updatePosition(pointOnCurve);
		trump.update();
		trump.rotate(1, glm::vec3(1.0f, 0.0f, 1.0f));
		trump.draw();

		//Curvas
		i = (i + 1) % nbCurvePoints;

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAObola);
	glDeleteVertexArrays(1, &VAOtrump);
	glfwTerminate();
	return 0;
}

//É chamada sempre que uma tecla do teclado é pressionada
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	camera.move(window, key, action);

	//"R" redefine a posição da bola
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		bolaSelecionada = !bolaSelecionada;

		if (!bolaSelecionada) {
			rotateX = false;
			rotateY = false;
			rotateZ = false;
			bolaY = 0.0f;
			bolaX = 0.0f;
		}
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = bolaSelecionada;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = bolaSelecionada;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = bolaSelecionada;
	}

	if (bolaSelecionada && action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
		case GLFW_KEY_UP:
			if (bolaY < maxMove) {
				bolaY += moveBola; // Translação para cima
			}
			else {
				bolaY = 0.0f;
			}
			break;
		case GLFW_KEY_DOWN:
			if (bolaY > -maxMove) {
				bolaY -= moveBola; // Translação para baixo
			}
			else {
				bolaY = 0.0f;
			}
			break;
		case GLFW_KEY_LEFT:
			if (bolaX > -maxMove) {
				bolaX -= moveBola; // Translação para a esquerda
			}
			else {
				bolaX = 0.0f;
			}
			break;
		case GLFW_KEY_RIGHT:
			if (bolaX < maxMove) {
				bolaX += moveBola; // Translação para a direita
			}
			else {
				bolaX = 0.0f;
			}
			break;
		}
	}
}

//É chamada sempre que o mouse é movido. Ela calcula a diferença entre a posição atual do mouse e a posição anterior para determinar a direção do movimento da bola.

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.rotate(window, xpos, ypos);
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	GLuint VAO, VBO[3];

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	glBindVertexArray(VAO);

	//As informações são armazenadas nos vetores globais positions, texCoords e normals.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), textureCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	return VAO;
}

//carrega um arquivo OBJ e extrai as informações da malha.
void loadOBJ(string path)
{
	vector<glm::vec3> vertexIndices;
	vector<glm::vec2> textureIndices;
	vector<glm::vec3> normalIndices;

	ifstream file(path);
	if (!file.is_open())
	{
		cerr << "Failed to open file: " << path << endl;
		return;
	}


	string line;
	//Lê o arquivo linha por linha e analisa cada linha para obter as informações de vértices, coordenadas de textura e normais.
	while (getline(file, line))
	{
		istringstream iss(line);
		string prefix;
		iss >> prefix;

		if (prefix == "mtllib")
		{
			iss >> mtlFilePath;
		}
		else if (prefix == "v")
		{
			float x, y, z;
			iss >> x >> y >> z;
			vertexIndices.push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "vt")
		{
			float u, v;
			iss >> u >> v;
			textureIndices.push_back(glm::vec2(u, v));
		}
		else if (prefix == "vn")
		{
			float x, y, z;
			iss >> x >> y >> z;
			normalIndices.push_back(glm::vec3(x, y, z));
		}
		else if (prefix == "f")
		{
			string v1, v2, v3;
			iss >> v1 >> v2 >> v3;

			glm::ivec3 vIndices, tIndices, nIndices;
			istringstream(v1.substr(0, v1.find('/'))) >> vIndices.x;
			istringstream(v1.substr(v1.find('/') + 1, v1.rfind('/') - v1.find('/') - 1)) >> tIndices.x;
			istringstream(v1.substr(v1.rfind('/') + 1)) >> nIndices.x;
			istringstream(v2.substr(0, v2.find('/'))) >> vIndices.y;
			istringstream(v2.substr(v2.find('/') + 1, v2.rfind('/') - v2.find('/') - 1)) >> tIndices.y;
			istringstream(v2.substr(v2.rfind('/') + 1)) >> nIndices.y;
			istringstream(v3.substr(0, v3.find('/'))) >> vIndices.z;
			istringstream(v3.substr(v3.find('/') + 1, v3.rfind('/') - v3.find('/') - 1)) >> tIndices.z;
			istringstream(v3.substr(v3.rfind('/') + 1)) >> nIndices.z;

			for (int i = 0; i < 3; i++)
			{
				const glm::vec3& vertex = vertexIndices[vIndices[i] - 1];
				const glm::vec2& texture = textureIndices[tIndices[i] - 1];
				const glm::vec3& normal = normalIndices[nIndices[i] - 1];

				positions.push_back(vertex.x);
				positions.push_back(vertex.y);
				positions.push_back(vertex.z);
				textureCoords.push_back(texture.x);
				textureCoords.push_back(texture.y);
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
			}
		}
	}

	file.close();
}

//Carrega um arquivo MTL e extrai as informações de material.
void loadMTL(string path)
{
	string line, readValue;
	ifstream mtlFile(path);

	while (!mtlFile.eof())
	{
		//lê o arquivo linha por linha e analisa cada linha para obter as informações de cor ambiente, cor especular e 
		//valor especular.
		getline(mtlFile, line);

		istringstream iss(line);

		//As informações são armazenadas nos vetores ka, ks e ns. Retorna verdadeiro se o carregamento for bem-sucedido e 
		//falso caso contrário.
		if (line.find("map_Kd") == 0)
		{
			iss >> readValue >> texturePath;
		}
		else if (line.find("Ka") == 0)
		{
			float ka1, ka2, ka3;
			iss >> readValue >> ka1 >> ka2 >> ka3;
			ka.push_back(ka1);
			ka.push_back(ka2);
			ka.push_back(ka3);
		}
		else if (line.find("Ks") == 0)
		{
			float ks1, ks2, ks3;
			iss >> readValue >> ks1 >> ks2 >> ks3;
			ks.push_back(ks1);
			ks.push_back(ks2);
			ks.push_back(ks3);
		}
		else if (line.find("Ns") == 0)
		{
			iss >> readValue >> ns;
		}
	}
	mtlFile.close();
}

//Carrega uma imagem e cria uma textura OpenGL com base nos dados da imagem.
int loadTexture(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Ajusta os par�metros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Carregamento da imagem
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data); //Usa a biblioteca STB Image para carregar a imagem.
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID; //retorna o identificador da textura gerada.
}

//Gera um conjunto de pontos de controle para a criação de uma curva. Utiliza a classe Hermite, Bezier e CatmullRom para 
//gerar os pontos de controle.
std::vector<glm::vec3> generateControlPointsSet(const std::string& input) {
	std::vector<glm::vec3> controlPoints;
	std::istringstream iss(input);
	float x, y, z;

	while (iss >> x >> y >> z) {
		controlPoints.push_back(glm::vec3(x, y, z));
	}
	//Retorna um vetor de pontos de controle
	return controlPoints;
}


//Cria uma curva a partir de um conjunto de pontos de controle. Utiliza a classe Hermite, Bezier e CatmullRom para criar a curva.
std::vector<glm::vec3> createCurve(float radius, int points, float speed)
{
	std::vector<glm::vec3> curvePoints;

	float angleIncrement = (2.0f * glm::pi<float>() / points) * speed;
	float currentAngle = 0.0f;

	for (int i = 0; i < points; i++)
	{
		float x = radius * std::cos(currentAngle);
		float y = 0.0f;
		float z = radius * std::sin(currentAngle);

		curvePoints.push_back(glm::vec3(x, y, z));

		currentAngle += angleIncrement;
	}
	//retorna a curva gerada
	return curvePoints;
}

