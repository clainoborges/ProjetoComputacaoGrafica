/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;


bool rotateX=false, rotateY=false, rotateZ=false;

float moveX = 0.0f;
float moveY = 0.0f;
float moveZ = 0.0f;
float escala = 1.0f;

float moveX2 = 0.0f;
float moveY2 = 0.0f;
float moveZ2 = 0.0f;
float escala2 = 1.0f;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Cubo - Claino!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");
	Shader shader2 = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();
	GLuint VAO2 = setupGeometry();

	glUseProgram(shader.ID);
	glUseProgram(shader2.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glm::mat4 model2 = glm::mat4(1); //matriz identidade;
	GLint modelLoc2 = glGetUniformLocation(shader2.ID, "model");
	model2 = glm::rotate(model2, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc2, 1, FALSE, glm::value_ptr(model2));


	glEnable(GL_DEPTH_TEST);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1); 
		model2 = glm::mat4(1);
		model2 = glm::scale(model2, glm::vec3(-1.0));
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.6f, 0.0f));

		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			model2 = glm::rotate(model2, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			model2 = glm::rotate(model2, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			model2 = glm::rotate(model2, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		model = glm::translate(model, glm::vec3(moveX, moveY, moveZ));
		model = glm::scale(model, glm::vec3(escala));
		model2 = glm::translate(model2, glm::vec3(moveX2, moveY2, moveZ2));
		model2 = glm::scale(model2, glm::vec3(escala2));

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		glUniformMatrix4fv(modelLoc2, 1, FALSE, glm::value_ptr(model2));
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		moveZ -= 0.1f;
		moveZ2 -= 0.1f;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		moveX -= 0.1f;
		moveX2 -= 0.1f;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		moveZ += 0.1f;
		moveZ2 += 0.1f;
	}
	
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		moveX += 0.1f;
		moveX2 += 0.1f;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		moveY += 0.1f;
		moveY2 += 0.1f;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		moveY -= 0.1f;
		moveY2 -= 0.1f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		escala -= 0.1f;
		escala2 -= 0.1f;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		escala += 0.1f;
		escala2 += 0.1f;
	}
}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {


		  //Base da pir�mide: 2 tri�ngulos
		  //x    y    z    r    g    b

		 
		  -0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //Amarelo
		  -0.5, -0.5,  0.5, 0.0, 1.0, 1.0, //ciano
		  -0.5, 0.5, 0.5, 1.0, 0.0, 1.0,  //Rosa F�chsia     

		   0.5, 0.5, -0.5, 1.0, 1.0, 0.0, //Amarelo
		   -0.5, -0.5, -0.5, 0.0, 1.0, 1.0, //ciano
		   -0.5, 0.5, -0.5, 1.0, 0.0, 1.0, //Rosa F�chsia
		   

		   //
		   0.5, -0.5, 0.5, 1.0, 0.0, 1.0, //Rosa F�chsia  
		   -0.5,  -0.5,  -0.5, 0.0, 1.0, 1.0, //ciano
		   0.5, -0.5, -0.5, 0.0, 1.0, 0.0, //Verde

			0.5, 0.5, -0.5, 1.0, 0.0, 1.0, //Rosa F�chsia  
			0.5,  -0.5,  -0.5, 0.0, 1.0, 1.0, //ciano
			-0.5, -0.5, -0.5, 0.0, 1.0, 0.0, //Verde

			-0.5, -0.5, -0.5, 1.0, 1.0, 0.0, //Amarelo
			-0.5,  0.5,  0.5, 1.0, 0.0, 0.0, //Vermelho
			-0.5, 0.5, -0.5, 0.0, 1.0, 0.0, //Verde

			0.5, -0.5, 0.5, 1.0, 1.0, 0.0, //Amarelo
			-0.5,  -0.5,  0.5, 1.0, 0.0, 0.0, //Vermelho
			-0.5, -0.5, -0.5, 0.0, 1.0, 0.0, //Verde

			-0.5, 0.5, 0.5, 0.0, 0.0, 1.0, //Azul
			-0.5,  -0.5,  0.5, 0.0, 1.0, 0.0, //Verde
			0.5, -0.5, 0.5, 1.0, 1.0, 0.0, //Amarelo

			0.5, 0.5, 0.5, 0.0, 0.0, 1.0, //Azul
			0.5,  -0.5,  -0.5, 0.0, 1.0, 0.0, //Verde
			0.5, 0.5, -0.5, 1.0, 1.0, 0.0, //Amarelo

			0.5, -0.5, -0.5, 1.0, 0.0, 0.0, //Vermelho
			0.5,  0.5,  0.5, 0.0, 0.0, 1.0, //Azul
			0.5, -0.5, 0.5, 0.0, 1.0, 1.0, //ciano

			0.5, 0.5, 0.5, 1.0, 0.0, 0.0, //Vermelho
			0.5,  0.5, -0.5, 0.0, 0.0, 1.0, //Azul
			-0.5, 0.5, -0.5, 0.0, 1.0, 1.0, //ciano

			0.5, 0.5, 0.5, 0.0, 1.0, 0.0, //Verde
		   -0.5,  0.5,  -0.5, 0.0, 0.0, 1.0, //Azul
		   -0.5, 0.5, 0.5, 0.0, 0.0, 0.0, //Preto

			0.5, 0.5, 0.5, 0.0, 1.0, 0.0, //Verde
			-0.5,  0.5,  0.5, 0.0, 0.0, 1.0, //Azul
			0.5, -0.5, 0.5, 0.0, 0.0, 0.0, //Preto

	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
