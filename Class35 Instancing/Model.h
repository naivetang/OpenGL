#pragma once

#include <vector>
#include "Mesh.h"


#include "stb_image.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

using namespace std;

class Model
{
public:

	Model() {};

	/*  函数   */
	Model(const char *path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
public:

	map<string, Texture> textures_loaded;

	/*  模型数据  */
	std::vector<Mesh> meshes;

	string directory;

	/*  函数   */
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(string s1, string s2);


};
