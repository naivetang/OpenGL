#pragma once

#include <glm/detail/type_vec3.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <assimp/types.h>

using namespace  std;

struct Vertex
{
	glm::vec3 Position;

	glm::vec3 Noraml;

	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;

	std::string type;

	std::string path;   // ���Ǵ��������·������������������бȽ�
};

class Mesh
{
public:
	// ��������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) :vertices(vertices), indices(indices), textures(textures)
	{
		SetupMesh();
	}

	void Draw(Shader shader);


private:

	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};