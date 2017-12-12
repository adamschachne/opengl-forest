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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
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
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;
	if (Window::targetObject == 3 && Window::currentLight == this) {
		//modelview *= glm::inverse(glm::lookAt(Window::lightPosition, Window::lightDirection + Window::lightPosition, glm::vec3(0, 1.0f, 0)));
		//modelview *= glm::mat4(1, 0, 0, 0,
		//						0, 1, 0, 0,
		//						0, 0, 1, 0,
		//						0, 0, -1, 1);
	}

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
	
		// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
		glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), mat.ambient.x, mat.ambient.y, mat.ambient.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), mat.diffuse.x, mat.diffuse.y, mat.diffuse.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), mat.specular.x, mat.specular.y, mat.specular.z);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), mat.shininess);
		glUniform1i(glGetUniformLocation(shaderProgram, "material.object_mode"), Window::colorMode);
	//light
	//if (Window::targetObject == 0) {
		glUniform3f(glGetUniformLocation(shaderProgram, "light.light_pos"), Window::lightPosition.x, Window::lightPosition.y, Window::lightPosition.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "light.light_dir"), Window::lightDirection.x, Window::lightDirection.y, Window::lightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "light.light_color"), Window::lightcolor.x, Window::lightcolor.y, Window::lightcolor.z);
		glUniform1i(glGetUniformLocation(shaderProgram, "light.light_mode"), Window::lightMode);
		glUniform1f(glGetUniformLocation(shaderProgram, "light.exponent"), Window::lightExponent);
		glUniform1f(glGetUniformLocation(shaderProgram, "light.light_field"), Window::lightField);
	//}
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}