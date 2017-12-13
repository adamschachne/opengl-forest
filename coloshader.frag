#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BlurColor;
layout (location = 2) out vec4 depthColor;
//out vec4 FragColor;

in vec3 fragVert;
in vec3 fragNormal;
in vec2 TexCoords;


uniform vec3 colorIn;
uniform uint id;
uniform int blur = 0;
uniform sampler2D texture_diffuse1;
uniform float winwidth;
uniform float winheight;
uniform float focaldistance;
uniform float focallength;

float near = 0.1; 
float far  = 200.0;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{        

	float blurAmt;
	vec2 center = vec2(winwidth / 2, winheight / 2);
	float dist = distance(center,vec2(gl_FragCoord.x,gl_FragCoord.y));

	vec4 texcolo = texture(texture_diffuse1, TexCoords);
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	   // FragColor = vec4(vec3(depth), 1.0);

//	int newx = int(texcolo.x*100);
//	texcolo.x = float(newx);
	//texcolo.x = texcolo.x / 100.0f;
	//texcolo.x = 0.0005;
    FragColor = texcolo;
	depthColor = vec4(depth,depth,depth,1.0);

	BlurColor = texcolo;
	//depth testing - if it is in the focal range then make it black
    //if (depth > 0.3 && depth < 0.5){
	//if( dist < 200 ){
	//			BlurColor =  vec4(0, 0, 0, 1.0);
	//	}
	//}

	//FragColor = vec4(vec3(0.7,0.1,0.4), 1.0);
	//if color is black then add together
	//else (assume same), so just use one, but look at depth and shift amt of times, increasing for far depth from norm
}