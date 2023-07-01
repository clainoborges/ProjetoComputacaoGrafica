#version 450 core

in vec3 finalColor; // Cor final do fragmento, fornecida como entrada.
in vec3 scaledNormal; //Escala normalizada
in vec3 fragPos; //Posição do fragmento
in vec2 texCoord; //Coordenadas de textura do fragmento 

//Propriedades do material do objeto
uniform vec3 ka; //Coeficiente de reflexão ambiente do material do objeto.
uniform float kd; //Coeficiente de reflexão difusa do material do objeto.
uniform vec3 ks; // Coeficiente de reflexão especular do material do objeto.
uniform float q; //Fator de intensidade especular do material do objeto.

//Propriedades da fonte de luz
uniform vec3 lightPos;
uniform vec3 lightColor;

//Posição da câmera 
uniform vec3 cameraPos;

//Buffer de saída (color buffer)
out vec4 color;

//buffer de textura
uniform sampler2D colorBuffer;

void main()
{
    //Cálculo da parcela de iluminação ambiente
    vec3 ambient =  lightColor * ka;
    
    //Cálculo da parcela de iluminação difusa
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N, L),0.0);
    vec3 diffuse = diff * lightColor * kd;
    
    //Cálculo da parcela de iluminação especular
    vec3 R = reflect(-L,N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R,V),0.0),q);
    vec3 specular = spec * ks * lightColor;
    

    vec4 texColor = texture(colorBuffer,texCoord);

    //A cor final do fragmento é calculada como a soma da iluminação ambiente, iluminação difusa multiplicada pela cor da
    //textura e iluminação especular.
    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;

    color = vec4(result, 1.0f);
}