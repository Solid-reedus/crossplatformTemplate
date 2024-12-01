#include "model.h"

#ifdef LINUX_BUILD
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#endif // LINUX_BUILD

#ifdef WINDOWS_BUILD

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#endif // WINDOWS_BUILD


#include "string"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "baseShader.h"
#include "baseRenderable.h"


Model::Model(std::string const& p_path)
{
    #if defined(WINDOWS_BUILD) || defined(LINUX_BUILD)
    if (!LoadModel(p_path))
    {
        printf("error: model was unable to be loaded \n");
    }
    else
    {
        printf("model loaded \n");
    }
    #endif // WINDOWS_BUILD

}



void Model::Draw(BaseShader* p_shader, Camera& p_cameraRef)
{
    #if defined(WINDOWS_BUILD) || defined(LINUX_BUILD)
    for (Mesh& mesh : m_meshes)
    {
        mesh.Draw(p_shader);
    }
    #endif // WINDOWS_BUILD
}
#if defined(WINDOWS_BUILD) || defined(LINUX_BUILD)

bool Model::LoadModel(std::string const& p_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        printf("ERROR::ASSIMP::%s \n", importer.GetErrorString());
        return false;
    }
    // retrieve the directory path of the filepath
    m_directory = p_path.substr(0, p_path.find_last_of('/'));

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

    return true;
}

void Model::processNode(aiNode* p_node, const aiScene* p_scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < p_node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = p_scene->mMeshes[p_node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, p_scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < p_node->mNumChildren; i++)
    {
        processNode(p_node->mChildren[i], p_scene);
    }
}

Mesh Model::processMesh(aiMesh* p_mesh, const aiScene* p_scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < p_mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = p_mesh->mVertices[i].x;
        vector.y = p_mesh->mVertices[i].y;
        vector.z = p_mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (p_mesh->HasNormals())
        {
            vector.x = p_mesh->mNormals[i].x;
            vector.y = p_mesh->mNormals[i].y;
            vector.z = p_mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (p_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = p_mesh->mTextureCoords[0][i].x;
            vec.y = p_mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = p_mesh->mTangents[i].x;
            vector.y = p_mesh->mTangents[i].y;
            vector.z = p_mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = p_mesh->mBitangents[i].x;
            vector.y = p_mesh->mBitangents[i].y;
            vector.z = p_mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < p_mesh->mNumFaces; i++)
    {
        aiFace face = p_mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    // process materials
    aiMaterial* material = p_scene->mMaterials[p_mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* p_mat, aiTextureType p_type, std::string p_typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < p_mat->GetTextureCount(p_type); i++)
    {
        aiString str;
        p_mat->GetTexture(p_type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
        {
            if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_directory);
            texture.type = p_typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int Model::TextureFromFile(const char* p_path, const std::string& p_directory, bool p_gamma)
{
    std::string filename = std::string(p_path);
    filename = p_directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
        {
            format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
        }

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
        std::cout << "Texture failed to load at path: " << p_path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


#endif // WINDOWS_BUILD