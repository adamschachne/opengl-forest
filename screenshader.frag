#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blur;
uniform sampler2D depthscene;
uniform int rendermode;

void main()
{             
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(blur, TexCoords).rgb;
	vec3 depthvec = texture(depthscene, TexCoords).rgb;

	if(rendermode == 2){//motion blur
		FragColor = vec4(bloomColor, 1.0);
	}
	else if (rendermode == 1){//DOF
		float depth = depthvec.x;
		float focusAmt = depthvec.y;
		//extract color


		//FragColor = vec4(dist, dist, dist, 1.0);
		FragColor = vec4(hdrColor*focusAmt + bloomColor*(1-focusAmt), 1.0);
	}
	else{//normal rendering of textures
		FragColor = vec4(hdrColor, 1.0);
	}

}