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

void main()

{
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
   
		color = sum;
	} else {
		color = texture(texture_diffuse1, TexCoords);
	}
}