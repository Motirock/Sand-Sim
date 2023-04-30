#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include <vector>

#include "VAO.h"
#include "IBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:
	// Initializes the mesh
    Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
	// Draws the mesh
	void draw(Shader& shader, Camera& camera);
	void drawSimple(Shader& shader, Camera& camera);
	void update(Camera& camera);

    std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO vao;
};

#endif