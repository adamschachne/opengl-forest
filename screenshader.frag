#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blur;
uniform sampler2D depthscene;
uniform bool toblur;

void main()
{             
   // const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;  
	
	vec3 depthvec = texture(depthscene, TexCoords).rgb;
	float depth = depthvec.x;
	float focusAmt = depthvec.y;
	//extract color

    vec3 bloomColor = texture(blur, TexCoords).rgb;
    //if(toblur)

	//FragColor = vec4(dist, dist, dist, 1.0);

	FragColor = vec4(hdrColor*focusAmt + bloomColor*(1-focusAmt), 1.0);
	//FragColor = vec4(focusAmt, focusAmt, focusAmt, 1.0);

    // tone mapping
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
   // result = pow(result, vec3(1.0 / gamma));

  // vec3 result = hdrColor; // CHANGE THIS LATER TO CHOOSE BLENDING AMT DEPENDING ON DEPTH/XY
  // if ((result.x - 0.00004) < 0.1)
	// result = vec3(1.0,0.3,0.4);
	//if (depth < 0.5 && depth > 0.3)
	//	result = vec3(0.0,0.0,1.0);
}