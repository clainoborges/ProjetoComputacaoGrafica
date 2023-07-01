#version 450 core

in vec3 finalColor; // Cor final do fragmento, fornecida como entrada.
in vec3 scaledNormal; //Escala normalizada
in vec3 fragPos; //Posi��o do fragmento
in vec2 texCoord; //Coordenadas de textura do fragmento 

//Propriedades do material do objeto
uniform vec3 ka; //Coeficiente de reflex�o ambiente do material do objeto.
uniform float kd; //Coeficiente de reflex�o difusa do material do objeto.
uniform vec3 ks; // Coeficiente de reflex�o especular do material do objeto.
uniform float q; //Fator de intensidade especular do material do objeto.

//Propriedades da fonte de luz
uniform vec3 lightPos;
uniform vec3 lightColor;

//Posi��o da c�mera 
uniform vec3 cameraPos;

//Buffer de sa�da (color buffer)
out vec4 color;

//buffer de textura
uniform sampler2D colorBuffer;

void main()
{
    //C�lculo da parcela de ilumina��o ambiente
    vec3 ambient =  lightColor * ka;
    
    //C�lculo da parcela de ilumina��o difusa
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N, L),0.0);
    vec3 diffuse = diff * lightColor * kd;
    
    //C�lculo da parcela de ilumina��o especular
    vec3 R = reflect(-L,N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R,V),0.0),q);
    vec3 specular = spec * ks * lightColor;
    

    vec4 texColor = texture(colorBuffer,texCoord);

    //A cor final do fragmento � calculada como a soma da ilumina��o ambiente, ilumina��o difusa multiplicada pela cor da
    //textura e ilumina��o especular.
    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;

    color = vec4(result, 1.0f);
}