#version 330 core

in vec3 fragVert;
in vec3 fragNormal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform vec3 colorIn;
uniform uint id;

void main()

{
    vec3 norm = normalize(fragNormal);
    color = vec4(0.2*norm.x, norm.y, norm.z, 1.0f);
}