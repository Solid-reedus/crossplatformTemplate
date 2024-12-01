#pragma once

#include <string>
#include <vector>
#include "mesh.h"
//#include <mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

//#include "shader.h"
#include "baseShader.h"
#include "baseRenderable.h"

//class Shader;
//struct Camera;


class Model
{
	public:
	Model(std::string const& p_path);

	// might chnage Instance to a mat4 since it doesnt change the instace vals
	void Draw(BaseShader* p_shader, Camera& p_cameraRef);

	#if defined(WINDOWS_BUILD) || defined(LINUX_BUILD)
	
	private:
	bool LoadModel(std::string const& p_path);
	
	void processNode(aiNode* p_node, const aiScene* p_scene);
	Mesh processMesh(aiMesh* p_mesh, const aiScene* p_scene);
	
	std::vector<Texture> loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type, std::string p_typeName);
	unsigned int TextureFromFile(const char* p_path, const std::string& p_directory, bool p_gamma = false);


	std::vector<Mesh> m_meshes;
	std::string m_directory;
	std::vector<Texture> m_textures_loaded;
	#endif // WINDOWS_BUILD

};


