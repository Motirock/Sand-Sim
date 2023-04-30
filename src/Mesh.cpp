#include "Mesh.h"

Mesh::Mesh() {
	std::cout << "USED DEFAULT CONSTRUCTOR FOR MESH\n";
	vao = VAO();
    vao.bind();
    VBO vbo = VBO(vertices);
    IBO ibo = IBO(indices);
    //Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(7 * sizeof(float)));
	vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(10 * sizeof(float)));
	//Unbind all to prevent accidentally modifying them
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	vao.bind();
	//Generates Vertex Buffer Object and links it to vertices
	VBO vbo(vertices);
	//Generates Element Buffer Object and links it to indices
	IBO ibo(indices);
	//Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	//Unbind all to prevent accidentally modifying them
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
}

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
	std::cout << "CREATING SIMPLE MESH\n";
	Mesh::vertices = vertices;
	Mesh::indices = indices;

	vao.bind();
	//Generates Vertex Buffer Object and links it to vertices
	VBO vbo(vertices);
	//Generates Element Buffer Object and links it to indices
	IBO ibo(indices);
	//Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	//Unbind all to prevent accidentally modifying them
	vao.unbind();
	vbo.unbind();
	ibo.unbind();
}

void Mesh::update(Camera& camera) {

}

void Mesh::draw(Shader& shader, Camera& camera) {
	//Bind shader to be able to access uniforms
	shader.activateShader();
	vao.bind();

	//Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].textureUnit(shader, (type + num).c_str(), i);
		textures[i].bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::drawSimple(Shader& shader, Camera& camera) {
	//Generates Vertex Buffer Object and links it to vertices
	VBO vbo = VBO(vertices);
	//Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);

	//Bind shader to be able to access uniforms
	shader.activateShader();
	vao.bind();

	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	
}