#include "Camera.h"

//responsável por inicializar a câmera. Recebe vários parâmetros, como o shader utilizado para renderização, a largura
//e altura da janela, a sensibilidade do mouse, a inclinação e o arremesso (pitch e yaw) da câmera, a direção frontal da câmera, a posição da câmera e a direção para cima
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

	//Matriz de view -- posição e orientação da câmera
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shader->setMat4("view", value_ptr(view));

	//Matriz de projeção perspectiva - definindo o volume de visualização (frustum)
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	shader->setMat4("projection", glm::value_ptr(projection));
}

//É chamada quando há movimento do mouse. Ela atualiza a direção frontal da câmera com base no deslocamento do mouse
//xpos e ypos são as coordenadas x e y atuais do cursor do mouse
void Camera::rotate(GLFWwindow* window, double xpos, double ypos)
{
	//calcula o deslocamento em relação à posição anterior do cursor (lastX e lastY)
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

	//Calcula a inclinação (pitch) e arremesso (yaw) incrementando os deslocamentos multiplicados pela sensibilidade
	pitch += offsety;
	yaw += offsetx;

	//A direção frontal da câmera é calculada com base na inclinação e no arremesso, e é normalizada para manter o comprimento do vetor
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

//Atualiza a posição e a orientação da câmera.
void Camera::update() {
	//cria uma nova matriz de visualização (view) usando glm::lookAt com a posição da câmera, a posição da câmera mais a direção frontal e a direção para cima. 
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	shader->setMat4("view", glm::value_ptr(view));

	//Atualizando o shader com a posição da câmera
	shader->setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
}

//É chamada quando uma tecla é pressionada. Ela atualiza a posição da câmera com base na tecla pressionada. 
void Camera::move(GLFWwindow* window, int key, int action)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	float cameraSpeed = 0.15;

	//A posição da câmera é incrementada pela direção frontal multiplicada pela velocidade da câmera
	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	//A posição da câmera é decrementada pela direção frontal multiplicada pela velocidade da câmera.
	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	//A posição da câmera é decrementada pela direção perpendicular ao plano definido pela direção frontal e para cima, normalizada e multiplicada pela velocidade da câmera.
	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
    //A posição da câmera é incrementada pela direção perpendicular ao plano definido pela direção frontal e para cima, normalizada e multiplicada pela velocidade da câmera.
	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}