#include "Mesh.h"

//Inicializar os atributos da classe Mesh com os valores fornecidos.
void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, GLuint textureID, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis)
{
	this->VAO = VAO; //um identificador OpenGL para o Vertex Array Object usado para renderizar o modelo.
	this->nVertices = nVertices; //o n�mero de v�rtices do modelo.
	this->shader = shader; //um ponteiro para um objeto Shader que ser� usado para renderizar o modelo.
	this->position = position; //a posi��o inicial do modelo no espa�o 3D
	this->scale = scale; //a escala do modelo em cada eixo.
	this->angle = angle; //o �ngulo inicial de rota��o do modelo.
	this->axis = axis; //o eixo de rota��o do modelo.
	this->textureID = textureID; //um identificador OpenGL para a textura do modelo.
}

//Atualiza a matriz de modelo da malha (usada para transforma��es) e a envia para o shader.
void Mesh::update()
{
	//Cria uma matriz de modelo model inicializada como uma matriz identidade
	glm::mat4 model = glm::mat4(1);
	
	model = glm::translate(model, position); //Aplica uma transla��o na matriz de modelo usando o vetor position.
	model = glm::rotate(model, glm::radians(angle), axis); //Aplica uma rota��o na matriz de modelo usando o �ngulo angle e o eixo axis.
	model = glm::scale(model, scale); //Aplica uma escala na matriz de modelo usando o vetor scale.

	//Define a matriz de modelo no shader usando o m�todo setMat4 do objeto shader, passando o nome da vari�vel uniforme "model".
	shader->setMat4("model", glm::value_ptr(model));
}

//Realiza o processo de desenho da malha na tela.
void Mesh::draw()
{
	//Ativa a textura 
	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, textureID);//Associa a textura fornecida (textureID) ao tipo GL_TEXTURE_2D usando glBindTexture
	glBindVertexArray(VAO); //Associa o Vertex Array Object(VAO) usando glBindVertexArray.

	glDrawArrays(GL_TRIANGLES, 0, nVertices); //Desenha a malha utilizando a fun��o glDrawArrays com o modo de desenho GL_TRIANGLES, come�ando do �ndice 0 e desenhando nVertices v�rtices.

	glBindVertexArray(0); //Desassocia o Vertex Array Object usando glBindVertexArray com o valor 0.
	glBindTexture(GL_TEXTURE_2D, 0); //Desassocia a textura do tipo GL_TEXTURE_2D usando glBindTexture com o valor 0.
}

// Atualiza a posi��o da malha no espa�o 3D. Recebe como par�metro um novo vetor position e atribui esse valor ao 
//atributo position da classe
void Mesh::updatePosition(glm::vec3 position) {
	this->position = position;
}
//Realiza uma rota��o adicional na malha com base em um fator de velocidade. Recebe a velocidade e o eixo da rota��o.
void Mesh::rotate(int speed, glm::vec3 axis) {
	glm::mat4 model = glm::mat4(1); //Cria uma matriz de modelo model inicializada como uma matriz identidade
	model = glm::translate(model, position); //Aplica uma transla��o na matriz de modelo usando o vetor position.
	model = glm::rotate(model, (GLfloat)glfwGetTime() / speed, axis); //Aplica uma rota��o na matriz de modelo usando o tempo atual dividido pela velocidade speed e o eixo axis.
	model = glm::scale(model, scale); //Aplica uma escala na matriz de modelo usando o vetor scale.
	shader->setMat4("model", glm::value_ptr(model)); //Define a matriz de modelo no shader usando o m�todo setMat4 do objeto shader, passando o nome da vari�vel uniforme "model
}
