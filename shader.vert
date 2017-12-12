#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 modelview;

out float sampleExtraOutput;
out vec3 fragVert;
out vec3 fragNormal;
out vec2 TexCoords;

void main()
{
	fragVert = vec3(modelview * vec4(position, 1.0f));
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position, 1.0f);
	//texCoordV = texCoord;
	fragNormal = normal;
	TexCoords = aTexCoords;   
}
