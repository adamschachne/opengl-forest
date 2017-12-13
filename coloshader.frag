#version 330 core
layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BlurColor;
//out vec4 FragColor;

in vec3 fragVert;
in vec3 fragNormal;
in vec2 TexCoords;


uniform vec3 colorIn;
uniform uint id;
uniform int blur = 0;
uniform sampler2D texture_diffuse1;

float near = 0.1; 
float far  = 100.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{        

	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	   // FragColor = vec4(vec3(depth), 1.0);
    FragColor = texture(texture_diffuse1, TexCoords);

	//depth testing
   // if (depth > 0.5 && depth < 0.7)
	//	BlurColor = vec4(vec3(1.0,0.1,0.1), 1.0);
	//else
	//	BlurColor = vec4(vec3(depth), 1.0);

	//FragColor = vec4(vec3(0.7,0.1,0.4), 1.0);

}