#include "Model.h"

void Model::Draw(Shader shader)
{
	for (auto mesh : meshes)
	{
		mesh.Draw(shader);
	}

}

void Model::loadModel(string path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	

	for (int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);

	if (node->mMeshes == nullptr)
	{
		cout << "mesh is null , name : " << node->mName.C_Str() << endl;
		return;
	}
	else
	{
		cout << "load mesh, name : " << node->mName.C_Str() << endl;
	}

	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;

	vector<unsigned int> indices;

	vector<Texture> textures;

	// 顶点

	for(int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Noraml.x = mesh->mNormals[i].x;
		vertex.Noraml.y = mesh->mNormals[i].y;
		vertex.Noraml.z = mesh->mNormals[i].z;

		// 检查网格是否有问题坐标
		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.TexCoords = glm::vec2(0, 0);		

		vertices.push_back(vertex);
	}

	// Face
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// 纹理
	if(mesh -> mMaterialIndex >= 0)
	{
		aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	Mesh m(vertices, indices,textures);

	return m;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;

	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);

		Texture texture;

		auto item = textures_loaded.find(str.C_Str());

		if(item != textures_loaded.end())
		{
			texture = item->second;
		}
		else
		{
			texture.id = TextureFromFile(str.C_Str(), directory);

			texture.type = typeName;

			texture.path = str.C_Str();

			textures_loaded[texture.path] = texture;
		}		

		textures.push_back(texture);
	}

	return textures;
}


unsigned Model::TextureFromFile(string s1, string s2)
{
	// 宽度、高度、颜色通道个数
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load((s2 + '\\'+s1).c_str() , &width, &height, &nrChannels, STBI_rgb);

	unsigned int texture1;

	// 第一个参数是生成纹理的数量
	glGenTextures(1, &texture1);

	//glActiveTexture(GL_TEXTURE0);

	// 绑定texture对象到GL_TEXTURE_2D，让后面对GL_TEXTURE_2D的指令都绑定到texture纹理上
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 纹理 重复方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 纹理缩小、放大时的过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		// 根据前面载入的图片生成纹理
		// 第二个参数 0 ： 多级渐远纹理级别
		// 第六个参数 0 ： 都是0，历史遗留问题
		// 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	// 释放图像
	stbi_image_free(data);

	return texture1;
}









