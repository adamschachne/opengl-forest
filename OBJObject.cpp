#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	scaleVal = 1.0f;
	initialScaleVal = 1.0f;
	angle = 0.0f;
	offset = glm::vec3(0.0f, 0.0f, 0.0f);
	orbitDeg = 0.0f;
	orbitAmt = 0.0f;
	y_min = 0;
	y_max = 0;
	/*glm::mat4 rotation(1);
	glm::mat4 orbitMat(1);
	glm::mat4 translateMat(1);*/
	parse(filepath);	
	/*scale(1.0f);
	center();	*/
	reset();

	mat.ambient = glm::vec3(0.41f, 0.31f, 0.41f);
	mat.specular = glm::vec3(0.92f, 0.89f, 0.93f);
	mat.diffuse = glm::vec3(0.79f, 0.92f, 0.98f);
	mat.shininess = 0.7f;

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	std::vector<Vertex> verts;

	for (int i = 0; i < indices.size(); i++) {
		verts.push_back({
			mappedVertices[i],
			mappedNormals[i],
			mappedTextureCoordinates[i]
		});
	}
	int oldSize = indices.size();
	std::vector<unsigned int> newArray;
	for (int i = 0; i < oldSize; i++) {
		newArray.push_back(i);
	}
	


	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mappedVertices.size() * sizeof(glm::vec3), mappedVertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, mappedTextureCoordinates.size() * sizeof(glm::vec2), mappedTextureCoordinates.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(GLfloat),
		(GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newArray.size() * sizeof(unsigned int), newArray.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

OBJObject::~OBJObject() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

std::vector<std::string> OBJObject::split(std::string &s, char delim) {
	using namespace std;
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

void OBJObject::update()
{
	//spin(3.0f);
}

void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;

	spinMat = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	toWorld = toWorld*spinMat;
}

void OBJObject::orbit(float deg)
{
	this->orbitMat = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
	toWorld = orbitMat*toWorld;
}

void OBJObject::translate(float x, float y, float z) {
	offset += glm::vec3(x, y, z);
	translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	toWorld = translateMat*toWorld;
}


void OBJObject::scale(float s) {
	scaleVal *= s;
	//print();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(s, s, s));
	toWorld = scaleMat * toWorld;
}

void OBJObject::reset() {
	toWorld = glm::mat4(1);
	scaleVal = 1.0f;
	glm::mat4 centerMatrix = glm::translate(glm::mat4(1), -mid);
	toWorld = centerMatrix*toWorld;

	//glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(initialScaleVal, initialScaleVal, initialScaleVal));
	//toWorld = scaleMat*toWorld;

	//glm::mat4 transMatrix = glm::translate(glm::mat4(1.0), { (xMin + xMax) / -2, (yMin + yMax) / -2, (zMin + zMax) / -2 });
	//toWorld = transMatrix * toWorld;
}

void OBJObject::parse(const char *filepath) 
{
	// parse the OBJ file
	using namespace std;
	ifstream file(filepath, ifstream::in);
	string str;
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	vector<string> toks;
	vector<string> toksSlash;
	int lines = 0;
	float x_min, x_max;
	float z_min, z_max;

	x_min = y_min = z_min = 100.0f;
	x_max = y_max = z_max = -100.0f;

	while (getline(file, str)) {
		lines++;
		if (str[0] != 'v' && str[0] != 'f') {
			continue;
		}
		if (str[0] == 'f') {
			toks = split(str, ' ');
			//std::cout << toks[1] << toks[2] << toks[3] << std::endl;
			for (int i = 1; i < 4; i++) {
				toksSlash = split(toks[i], '/');
				//std::cout << toksSlash[0] << toksSlash[1] << toksSlash[2] << std::endl;
				for (int j = 0; j < 3; j++) {
					if (j == 0) {
						//std::cout << (unsigned int)strtof(toksSlash[j].c_str(), 0) - 1 << std::endl;
						indices.push_back((unsigned int)strtof(toksSlash[j].c_str(), 0) - 1);
					}
					if (j == 1) {
						textureIndices.push_back((unsigned int)strtof(toksSlash[j].c_str(), 0) - 1);
					}
					if (j == 2) {
						normalIndices.push_back((unsigned int)strtof(toksSlash[j].c_str(), 0) - 1);
					}
				}
			}
		}
		else { // str[0] == 'v'
			toks = split(str, ' ');
			std::string::size_type sz;     // alias of size_t
			int i = 0;
			for (auto num = ++toks.begin(); num != toks.end(); ++num, ++i) {
				//cout << *num << ", ";
				if (i == 0) {
					x = strtof((*num).c_str(), 0);
				}
				else if (i == 1) {
					y = strtof((*num).c_str(), 0);
				}
				else if (i == 2) {
					z = strtof((*num).c_str(), 0);
				}
				else if (i == 3) {
					r = strtof((*num).c_str(), 0);
				}
				else if (i == 4) {
					g = strtof((*num).c_str(), 0);
				}
				else if (i == 5) {
					b = strtof((*num).c_str(), 0);
				}
			}

			if (str[1] == 'n') {
				normals.push_back(glm::vec3(x, y, z));
			}
			else if (str[0] == 'v') {
				if (str[1] == 't') { // vt
					textureCoordinates.push_back(glm::vec2(x, y));
				}
				else if (str[1] == 'n') { // vn
				}
				else { // vertex				
					if (x > x_max) {
						x_max = x;
					}
					if (y > y_max) {
						y_max = y;
					}
					if (z > z_max) {
						z_max = z;
					}
					if (x < x_min) {
						x_min = x;
					}
					if (y < y_min) {
						y_min = y;
					}
					if (z < z_min) {
						z_min = z;
					}
					vertices.push_back(glm::vec3(x, y, z));
				}
			}
		}
	}
	mid = glm::vec3((x_max + x_min) / 2, (y_max + y_min)/2, (z_max + z_min)/2);

	float bigLength = x_max - x_min;
	if (bigLength < y_max - y_min) {
		bigLength = y_max - y_min;
	}
	if (bigLength < z_max - z_min) {
		bigLength = z_max - z_min;
	}
	initialScaleVal = 2/(bigLength);
	cout << "loaded " << lines << " " << filepath << endl;
	cout << "number of indices = " << indices.size() << endl;
	cout << "number of vertices = " << vertices.size() << endl;

	for (int i = 0; i < indices.size(); i++) {
		mappedVertices.push_back(vertices[indices[i]]);
		mappedNormals.push_back(normals[normalIndices[i]]);
		mappedTextureCoordinates.push_back(textureCoordinates[textureIndices[i]]);
	}

}

void OBJObject::print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << toWorld[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void OBJObject::draw(GLuint shaderProgram)
{

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse1"), 1);
	glActiveTexture(GL_TEXTURE1);

	glBindTexture(GL_TEXTURE_2D, 2);
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}
void OBJObject::drawScreen(GLuint shaderProgram, unsigned int * texturebuffer)
{
	glBindVertexArray(quadVAO);
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 2;
	//texturebuffer[0] contains the unblurred scene
	//texturebuffer[1] contains the blurred scene
	//glBindTexture(GL_TEXTURE_2D, texturebuffer[0]);
	//for (unsigned int i = 0; i < amount; i++)
	//{
		//glUniform1i(glGetUniformLocation(shaderProgram, "i"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturebuffer[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	//}
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texturebuffer);
}

void OBJObject::copyVAO(GLuint shaderProgram, unsigned int * texturebuffer)
{
	glBindVertexArray(quadVAO);
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 2;
	//texturebuffer[0] contains the unblurred scene
	//texturebuffer[1] contains the blurred scene
	//glBindTexture(GL_TEXTURE_2D, texturebuffer[0]);
	//for (unsigned int i = 0; i < amount; i++)
	//{
	//glUniform1i(glGetUniformLocation(shaderProgram, "i"), i);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturebuffer[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//}
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texturebuffer);
}