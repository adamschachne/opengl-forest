#version 330 core

in vec3 fragVert;
in vec3 fragNormal;
in vec2 TexCoords;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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


    vec3 norm = normalize(fragNormal);
    //color = vec4(0.2*norm.x, norm.y, norm.z, 1.0f);
	if (blur == 1) {
		const float blurSizeH = 1.0 / 300.0;
		const float blurSizeV = 1.0 / 200.0;
		vec4 sum = vec4(0.0);
		for (int x = -2; x <= 2; x++)
			for (int y = -2; y <= 2; y++)
							  /* framebuffer */
				sum += texture(texture_diffuse1, vec2(TexCoords.x + x * blurSizeH, TexCoords.y + y * blurSizeV)) / 25.0;
   
		//color = sum;
		color = vec4(vec3(depth), 1.0);
	} else {
		vec4 texcolor = texture(texture_diffuse1, TexCoords);
		//color = texture(texture_diffuse1, TexCoords);
		if (depth > 0.5 && depth < 0.7)
			color = vec4(vec3(depth), 1.0)*texcolor;
		else
			color = vec4(vec3(depth), 1.0);
	}
}