#version 330 core
 
uniform vec4 color1 = vec4(0.0, 0.0, 1.0, 1.0);
uniform vec4 color2 = vec4(1.0, 1.0, 0.5, 1.0);
in vec2 texCoordV;
//in fragVert;
 
out vec4 FragColor;
uniform sampler2D skybox;
 
void main() {
 
   FragColor = texture(skybox, texCoordV);
}