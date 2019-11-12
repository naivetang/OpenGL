#include "Mesh.h"

void Mesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;

	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ

		// ��ȡ������ţ�diffuse_textureN �е� N��
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setFloat(("material." + name + number).c_str(), i);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glActiveTexture(GL_TEXTURE0);

	// ��������
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
void Mesh::SetupMesh()
{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vertex::Position));//  (void*)(3 * sizeof(float)));//(void*)offsetof(Vertex, Vertex.Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vertex::Position) + sizeof(Vertex::Noraml))); //(void*)(6 * sizeof(float))); //offsetof(Vertex, TexCoords));
	
		glBindVertexArray(0);
	}
