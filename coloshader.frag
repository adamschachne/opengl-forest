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
uniform float focusDistance;
uniform float focusWidth;
uniform int debug;

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

	vec4 texcolo = texture(texture_diffuse1, TexCoords); // obtain sharp image mapping
	float depth = LinearizeDepth(gl_FragCoord.z) / far;

    FragColor = texcolo;
	BlurColor = texcolo; // this is going to get shaken up by blurshader a few times
	depthColor = vec4(depth,depth,depth,1.0);
	//depth testing - if it is in the focal range then make it black

	float sideSharpness = 1.0;
	sideSharpness -= (focusWidth)/(dist+focusWidth);

	sideSharpness = 1-sideSharpness;

	float focalDepth = focusDistance;
	float depthSharpness = 1.0;	
	if (depth > focalDepth) { // point deeper than our focus
		depthSharpness -= (depth - focalDepth)*5; // gradually less sharp	
	} else if (depth < focalDepth) { // point closer than our focus
		depthSharpness -= (focalDepth - depth)*5; // gradually less sharp
	}

	if (depthSharpness < 0) {
		depthSharpness = 0;
	}

	depthColor.y = (sideSharpness * depthSharpness);

	if (debug > 0) {
		if (debug == 1) {
			depthColor.y = sideSharpness;
		} else if (debug == 2) {
			depthColor.y = depthSharpness;
		} else if (debug == 3) {
			depthColor.y = (sideSharpness * depthSharpness);
		}
		FragColor = depthColor;
		BlurColor = depthColor; // this is going to get shaken up by blurshader a few times
	}
	//depthColor.y = depthSharpness;
	//depthColor.y = sideSharpness;

	//FragColor = vec4(vec3(0.7,0.1,0.4), 1.0);
	//if color is black then add together
	//else (assume same), so just use one, but look at depth and shift amt of times, increasing for far depth from norm
}