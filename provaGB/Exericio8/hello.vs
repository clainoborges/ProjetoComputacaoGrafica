#version 450

layout (location = 0) in vec3 position; //Define o atributo de entrada "position" com o layout de localiza��o 0
layout (location = 1) in vec2 texc; //Define o atributo de entrada "texc" com o layout de localiza��o 1
layout (location = 2) in vec3 normal; //Define o atributo de entrada "normal" com o layout de localiza��o 2

out vec3 finalColor; //Define a vari�vel de sa�da "finalColor"
out vec3 fragPos; // Define a vari�vel de sa�da "fragPos", que representa a posi��o do v�rtice no espa�o 3D
out vec2 texCoord; //Define a vari�vel de sa�da "texCoord", que representa as coordenadas de textura do v�rtice
out vec3 scaledNormal; //Define a vari�vel de sa�da "scaledNormal", que representa a normal do v�rtice 

uniform mat4 projection; //Define a matriz de proje��o como um uniforme. 
uniform mat4 model; //Define a matriz de modelo como um uniforme
uniform mat4 view; //Define a matriz de visualiza��o como um uniforme.

void main()
{
	//A posi��o do v�rtice � transformada pela matriz de proje��o, matriz de visualiza��o e matriz de modelo, e o resultado
	//� atribu�do � vari�vel pr�-definida gl_Position. Isso define a posi��o final do v�rtice.

	gl_Position = projection * view  * model * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0));
	texCoord = vec2(texc.x, 1-texc.y);
	scaledNormal = normal;
}