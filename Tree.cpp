#include "Tree.h"
#include "Window.h"


Tree::Tree(const char *filepath)
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

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)1);

	glBindVertexArray(0);


	cubemapTexture = loadTexture(faces2);
	treeshader = LoadShaders("../shader.vert", "../shader.frag");
	glUseProgram(treeshader);
	glUniform1i(glGetUniformLocation(treeshader, "skybox"), 1);
}

Tree::~Tree() {
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

std::vector<std::string> Tree::split(std::string &s, char delim) {
	using namespace std;
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

void Tree::update()
{
	//spin(3.0f);
}


void Tree::translate(float x, float y, float z) {
	offset += glm::vec3(x, y, z);
	translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	toWorld = translateMat*toWorld;
}


void Tree::scale(float s) {
	scaleVal *= s;
	//print();
	glm::mat4 scaleMat = glm::scale(glm::mat4(1), glm::vec3(s, s, s));
	toWorld = scaleMat * toWorld;
}

void Tree::reset() {
	toWorld = glm::mat4(1);
	scaleVal = 1.0f;
	glm::mat4 centerMatrix = glm::translate(glm::mat4(1), -mid);
	toWorld = centerMatrix*toWorld;
}

void Tree::parse(const char *filepath)
{
	// parse the OBJ file
	using namespace std;
	ifstream file(filepath, ifstream::in);
	string str;
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	vector<string> toks;
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
		
		// Populate the face indices, vertices, and normals vectors with the OBJ Object data


		if (str[1] == 't') {

			// normal
			/*float x1 = glm::normalize(glm::vec3(x, y, z)).x;
			float y1 = glm::normalize(glm::vec3(x, y, z)).y;
			float z1 = glm::normalize(glm::vec3(x, y, z)).z;
			normals.push_back(glm::vec3((x1+1)/2, (y1+1)/2, (z1+1)/2));*/
			normals.push_back(glm::vec2(x, y));
			//vertices.push_back(pair<char, glm::vec3>('n', glm::vec3(x, y, z)));
		}
		else if (str[0] == 'v') {
			if (str[1] == 'n') {
			}
			else {
				// vertex
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
				//std::cout << glm::normalize(glm::vec3(x, y, z)).x << " " << glm::normalize(glm::vec3(x, y, z)).y << " " << glm::normalize(glm::vec3(x, y, z)).z << std::endl;
				//vertices.push_back(pair<char, glm::vec3>('v', glm::vec3(x, y, z)));
			}
		}
		else if (str[0] == 'f') {
			//std::cout << "x: " << x << "y: " << y << "z: " << z << "r: " << r << "g: " << g << "b: " << b << std::endl;
			indices.push_back((unsigned int)x - 1);
			indices.push_back((unsigned int)y - 1);
			indices.push_back((unsigned int)z - 1);
		}
	}
	mid = glm::vec3((x_max + x_min) / 2, (y_max + y_min) / 2, (z_max + z_min) / 2);

	float bigLength = x_max - x_min;
	if (bigLength < y_max - y_min) {
		bigLength = y_max - y_min;
	}
	if (bigLength < z_max - z_min) {
		bigLength = z_max - z_min;
	}
	initialScaleVal = 2 / (bigLength);
	std::cout << "loaded " << lines << " " << filepath << std::endl;
	std::cout << "number of indices = " << indices.size() << std::endl;
	std::cout << "number of vertices = " << vertices.size() << std::endl;
	std::cout << "number of numbers = " << normals.size() << std::endl;
}

void Tree::print() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << toWorld[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Tree::draw(GLuint shaderProgram)
{
	glUseProgram(treeshader);
	//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(treeshader, "projection");
	uModelview = glGetUniformLocation(treeshader, "modelview");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);

	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubemapTexture);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	//glDepthFunc(GL_LESS); // set depth function back to default
}

unsigned int Tree::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	std::cout << "tree tex: " << textureID << std::endl;
	return textureID;
}

unsigned int Tree::loadMap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	std::cout << "tree tex: " << textureID << std::endl;
	return textureID;
}