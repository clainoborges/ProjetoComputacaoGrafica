#version 450

layout (location = 0) in vec3 position; //Define o atributo de entrada "position" com o layout de localização 0
layout (location = 1) in vec2 texc; //Define o atributo de entrada "texc" com o layout de localização 1
layout (location = 2) in vec3 normal; //Define o atributo de entrada "normal" com o layout de localização 2

out vec3 finalColor; //Define a variável de saída "finalColor"
out vec3 fragPos; // Define a variável de saída "fragPos", que representa a posição do vértice no espaço 3D
out vec2 texCoord; //Define a variável de saída "texCoord", que representa as coordenadas de textura do vértice
out vec3 scaledNormal; //Define a variável de saída "scaledNormal", que representa a normal do vértice 

uniform mat4 projection; //Define a matriz de projeção como um uniforme. 
uniform mat4 model; //Define a matriz de modelo como um uniforme
uniform mat4 view; //Define a matriz de visualização como um uniforme.

void main()
{
	//A posição do vértice é transformada pela matriz de projeção, matriz de visualização e matriz de modelo, e o resultado
	//é atribuído à variável pré-definida gl_Position. Isso define a posição final do vértice.

	gl_Position = projection * view  * model * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0));
	texCoord = vec2(texc.x, 1-texc.y);
	scaledNormal = normal;
}