#include "Curve.h"

//Recebe um ponteiro para um objeto Shader e o armazena no shader da classe Curve
void Curve::setShader(Shader* shader)
{
	this->shader = shader;
	shader->Use();
}

//Desenhar a curva utilizando o shader configurado anteriormente.
void Curve::drawCurve(glm::vec4 color)
{
	//Define a cor final da curva no shader
	shader->setVec4("finalColor", color.r, color.g, color.b, color.a);

	glBindVertexArray(VAO);
	// Chamada de desenho - drawcall
	// CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
	glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
	//glDrawArrays(GL_POINTS, 0, curvePoints.size());
	glBindVertexArray(0);

}