#include "Camera.h"

//respons�vel por inicializar a c�mera. Recebe v�rios par�metros, como o shader utilizado para renderiza��o, a largura
//e altura da janela, a sensibilidade do mouse, a inclina��o e o arremesso (pitch e yaw) da c�mera, a dire��o frontal da c�mera, a posi��o da c�mera e a dire��o para cima
void Camera::initialize(Shader* shader, int width, int height, float sensitivity, float pitch, float yaw, glm::vec3 cameraFront, glm::vec3 cameraPos, glm::vec3 cameraUp)
{
	firstMouse = true;
	this->shader = shader;
	this->sensitivity = sensitivity;
	this->pitch = pitch;
	this->yaw = yaw;
	this->cameraFront = cameraFront;
	this->cameraPos = cameraPos;
	this->cameraUp = cameraUp;

	//Matriz de view -- posi��o e orienta��o da c�mera
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shader->setMat4("view", value_ptr(view));

	//Matriz de proje��o perspectiva - definindo o volume de visualiza��o (frustum)
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	shader->setMat4("projection", glm::value_ptr(projection));
}

//� chamada quando h� movimento do mouse. Ela atualiza a dire��o frontal da c�mera com base no deslocamento do mouse
//xpos e ypos s�o as coordenadas x e y atuais do cursor do mouse
void Camera::rotate(GLFWwindow* window, double xpos, double ypos)
{
	//calcula o deslocamento em rela��o � posi��o anterior do cursor (lastX e lastY)
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float offsetx = xpos - lastX;
	float offsety = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	offsetx *= sensitivity;
	offsety *= sensitivity;

	//Calcula a inclina��o (pitch) e arremesso (yaw) incrementando os deslocamentos multiplicados pela sensibilidade
	pitch += offsety;
	yaw += offsetx;

	//A dire��o frontal da c�mera � calculada com base na inclina��o e no arremesso, e � normalizada para manter o comprimento do vetor
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//Atualiza a posi��o e a orienta��o da c�mera.
void Camera::update() {
	//cria uma nova matriz de visualiza��o (view) usando glm::lookAt com a posi��o da c�mera, a posi��o da c�mera mais a dire��o frontal e a dire��o para cima. 
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	shader->setMat4("view", glm::value_ptr(view));

	//Atualizando o shader com a posi��o da c�mera
	shader->setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
}

//� chamada quando uma tecla � pressionada. Ela atualiza a posi��o da c�mera com base na tecla pressionada. 
void Camera::move(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	float cameraSpeed = 0.15;

	//A posi��o da c�mera � incrementada pela dire��o frontal multiplicada pela velocidade da c�mera
	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	//A posi��o da c�mera � decrementada pela dire��o frontal multiplicada pela velocidade da c�mera.
	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	//A posi��o da c�mera � decrementada pela dire��o perpendicular ao plano definido pela dire��o frontal e para cima, normalizada e multiplicada pela velocidade da c�mera.
	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
    //A posi��o da c�mera � incrementada pela dire��o perpendicular ao plano definido pela dire��o frontal e para cima, normalizada e multiplicada pela velocidade da c�mera.
	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}