#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

uniform bool horizontal;
uniform int forward;
uniform int backward;
uniform int left;
uniform int right;
uniform int rendermode;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	bool moving = true;
	if(forward == 0 && backward == 0 && left == 0 && right == 0)
		moving = false;
	if(rendermode == 2){ //motion blur
		if(moving){
			vec2 center = vec2(720/2,720/2);
			//center = normalize(center);
			//vec2 normtex = normalize(TexCoords);
			vec2 lines = center - TexCoords;

			if(lines.x < 0){
			//	lines.x = -lines.x;
			}
			if(lines.y < 0){
			//	lines.y = -lines.y;
			}

			lines = normalize(lines);

			//center - TexCoords;
			vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
			vec3 result = texture(image, TexCoords).rgb * weight[0];

			if(right== 1 || left==1){
				for(int i = 1; i < 5; ++i)
				{
					result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
					result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
				}
			}

			else if(forward== 1 || backward == 1){
				for(int i = 1; i < 5; ++i)
				{
					result += texture(image, TexCoords  + vec2((tex_offset.x + lines.x) * i,(tex_offset.y + lines.y) * i)).rgb * weight[i];
					result += texture(image, TexCoords  - vec2((tex_offset.x + lines.x) * i, (tex_offset.y + lines.y) * i)).rgb * weight[i];
				}	
			}
	 
	 //forward + right
	 //result += texture(image, TexCoords + (lines * vec2(tex_offset.x * i, tex_offset.y * i))).rgb * weight[i];
	 //result += texture(image, TexCoords - (lines * vec2(tex_offset.x * i, tex_offset.y * i))).rgb * weight[i];
			FragColor = vec4(result, 1.0);
		}
		else{
			vec2 center = vec2(360/2,720/2);
			center = normalize(center);
			vec2 normtex = normalize(TexCoords);
			vec2 lines = center - normtex;

			if(lines.x < 0){
				lines.x = -lines.x;
			}
			if(lines.y < 0){
				lines.y = -lines.y;
			}

			lines = normalize(lines);
			vec3 result = texture(image, TexCoords).rgb;
			FragColor = vec4(result, 1.0);
		}
	}
	
	else{     //not motion blur
		vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
		vec3 result = texture(image, TexCoords).rgb * weight[0];
	//vec3 result = texture(image, TexCoords).rgb;

	 
		if(horizontal)
		{
			for(int i = 1; i < 5; ++i)
			{
			result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
			}
		}
		else
		{
			for(int i = 1; i < 5; ++i)
			{
				result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
				result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
			}
		}
	 
		FragColor = vec4(result, 1.0);
	 }
}