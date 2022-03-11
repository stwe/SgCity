// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <assimp/Importer.hpp>
#include <ogl/math/Transform.h>
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "SgAssert.h"
#include "SgException.h"
#include "ogl/buffer/Vao.h"
#include "ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_fullFilePath, const unsigned int t_pFlags)
    : m_fullFilePath{ std::move(t_fullFilePath) }
{
    Log::SG_LOG_DEBUG("[Model::Model()] Create Model.");

    m_directory = m_fullFilePath.substr(0, m_fullFilePath.find_last_of('/'));

    LoadFromFile(t_pFlags);
}

sg::ogl::resource::Model::~Model() noexcept
{
    Log::SG_LOG_DEBUG("[Model::~Model()] Destruct Model.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::resource::Model::Render(
    const Window& t_window,
    const camera::Camera& t_camera,
    const glm::vec3& t_position
    ) const
{
    OpenGL::EnableAlphaBlending();

    const auto& shaderProgram{ ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/model") };
    shaderProgram.Bind();

    // todo: tmp code
    glm::vec3 p{ t_position };
    p.y += 1.0f;

    const auto modelMatrix{ math::Transform::CreateModelMatrix(
    p,
    glm::vec3(0.0f, 0.0f, 180.0f),
    glm::vec3(1.0f)
    ) };

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    for (const auto& mesh : meshes)
    {
        mesh->vao->Bind();

        shaderProgram.SetUniform("diffuseColor", mesh->defaultMaterial->kd);

        shaderProgram.SetUniform("hasDiffuseMap", mesh->defaultMaterial->HasDiffuseMap());
        if (mesh->defaultMaterial->HasDiffuseMap())
        {
            shaderProgram.SetUniform("diffuseMap", 0);

            // todo: create a static function in Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh->defaultMaterial->mapKd);
        }

        mesh->vao->DrawPrimitives();
        mesh->vao->Unbind();
    }

    ShaderProgram::Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Load
//-------------------------------------------------

void sg::ogl::resource::Model::LoadFromFile(const unsigned int t_pFlags)
{
    Assimp::Importer importer;

    Log::SG_LOG_DEBUG("[Model::LoadFromFile()] Start loading model file at: {}", m_fullFilePath);

    const auto* scene{ importer.ReadFile(m_fullFilePath, t_pFlags) };
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw SG_EXCEPTION("[Model::LoadFromFile()] ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }

    ProcessNode(scene->mRootNode, scene);

    Log::SG_LOG_DEBUG("[Model::LoadFromFile()] Model file at {} successfully loaded.", m_fullFilePath);
}

void sg::ogl::resource::Model::ProcessNode(const aiNode* t_node, const aiScene* t_scene)
{
    // Process each mesh located at the current node.
    for (auto i{ 0u }; i < t_node->mNumMeshes; ++i)
    {
        const auto* mesh{ t_scene->mMeshes[t_node->mMeshes[i]] };
        meshes.push_back(ProcessMesh(mesh, t_scene));
    }

    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes.
    for (auto i{ 0u }; i < t_node->mNumChildren; ++i)
    {
        ProcessNode(t_node->mChildren[i], t_scene);
    }
}

std::unique_ptr<sg::ogl::resource::Mesh> sg::ogl::resource::Model::ProcessMesh(const aiMesh* t_mesh, const aiScene* t_scene) const
{
    // Data to fill.
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    // Prevent duplicate warnings.
    auto missingUv{ false };
    auto missingTangent{ false };
    auto missingBiTangent{ false };

    // Walk through each of the mesh's vertices.
    for (auto i{ 0u }; i < t_mesh->mNumVertices; ++i)
    {
        // push position (3 floats)
        vertices.push_back(t_mesh->mVertices[i].x);
        vertices.push_back(t_mesh->mVertices[i].y);
        vertices.push_back(t_mesh->mVertices[i].z);

        // push normal (3 floats)
        //vertices.push_back(t_mesh->mNormals[i].x);
        //vertices.push_back(t_mesh->mNormals[i].y);
        //vertices.push_back(t_mesh->mNormals[i].z);

        // texture coordinates (2 floats)
        if (t_mesh->mTextureCoords[0])
        {
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertices.push_back(t_mesh->mTextureCoords[0][i].x);
            vertices.push_back(t_mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingUv)
            {
                missingUv = true;
                Log::SG_LOG_WARN("[Model::ProcessMesh()] Missing texture coords. Set default values (0, 0).");
            }
        }

        // tangent (3 floats)
        /*
        if (t_mesh->mTangents)
        {
            vertices.push_back(t_mesh->mTangents[i].x);
            vertices.push_back(t_mesh->mTangents[i].y);
            vertices.push_back(t_mesh->mTangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingTangent)
            {
                missingTangent = true;
                Log::SG_LOG_WARN("[Model::ProcessMesh()] Missing tangent coords. Set default values (0, 0, 0).");
            }
        }
        */

        // bitangent (3 floats)
        /*
        if (t_mesh->mBitangents)
        {
            vertices.push_back(t_mesh->mBitangents[i].x);
            vertices.push_back(t_mesh->mBitangents[i].y);
            vertices.push_back(t_mesh->mBitangents[i].z);
        }
        else
        {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            if (!missingBiTangent)
            {
                missingBiTangent = true;
                Log::SG_LOG_WARN("[Model::ProcessMesh()] Missing bitangent coords. Set default values (0, 0, 0).");
            }
        }
        */
    }

    // Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (auto i{ 0u }; i < t_mesh->mNumFaces; ++i)
    {
        const auto face{ t_mesh->mFaces[i] };

        // Retrieve all indices of the face and store them in the indices vector.
        for (auto j{ 0u }; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process materials.
    auto* aiMeshMaterial{ t_scene->mMaterials[t_mesh->mMaterialIndex] };

    // Create a unique_ptr Material instance.
    auto materialUniquePtr{ std::make_unique<Material>() };
    SG_ASSERT(materialUniquePtr, "[Model::ProcessMesh()] Null pointer.")

    // Set material name.
    aiString name;
    aiMeshMaterial->Get(AI_MATKEY_NAME, name);
    materialUniquePtr->newmtl = name.C_Str();

    // Set material colors.
    aiColor3D color;
    aiMeshMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
    materialUniquePtr->ka = glm::vec3(color.r, color.g, color.b);
    aiMeshMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    materialUniquePtr->kd = glm::vec3(color.r, color.g, color.b);
    aiMeshMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
    materialUniquePtr->ks = glm::vec3(color.r, color.g, color.b);

    // Set the material shininess.
    float shininess;
    aiMeshMaterial->Get(AI_MATKEY_SHININESS, shininess);
    materialUniquePtr->ns = shininess;

    // Load textures.
    Log::SG_LOG_DEBUG("[Model::ProcessMesh()] Loading textures for the model: {}", m_fullFilePath);

    const auto ambientMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_AMBIENT) };
    const auto diffuseMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_DIFFUSE) };
    const auto specularMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_SPECULAR) };
    const auto bumpMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_HEIGHT) };
    const auto normalMaps{ LoadMaterialTextures(aiMeshMaterial, aiTextureType_NORMALS) };

    // Always use the first texture Id.
    if (!ambientMaps.empty())
    {
        materialUniquePtr->mapKa = ambientMaps[0];
    }

    if (!diffuseMaps.empty())
    {
        materialUniquePtr->mapKd = diffuseMaps[0];
    }

    if (!specularMaps.empty())
    {
        materialUniquePtr->mapKs = specularMaps[0];
    }

    if (!bumpMaps.empty())
    {
        materialUniquePtr->mapBump = bumpMaps[0];
    }

    if (!normalMaps.empty())
    {
        materialUniquePtr->mapKn = normalMaps[0];
    }

    // Create a unique_ptr Mesh instance.
    auto meshUniquePtr{ std::make_unique<Mesh>() };
    SG_ASSERT(meshUniquePtr, "[Model::ProcessMesh()] Null pointer.")

    // Add Vbo.
    meshUniquePtr->vao->CreateModelVertexDataVbo(vertices, static_cast<int32_t>(t_mesh->mNumVertices));

    // Add Ebo.
    meshUniquePtr->vao->CreateModelIndexBuffer(indices);

    // Each mesh has a default material. Set the material properties as default.
    meshUniquePtr->defaultMaterial = std::move(materialUniquePtr);

    // Return a mesh object created from the extracted mesh data.
    return meshUniquePtr;
}

std::vector<uint32_t> sg::ogl::resource::Model::LoadMaterialTextures(const aiMaterial* t_mat, const aiTextureType t_type) const
{
    std::vector<uint32_t> textures;

    for (auto i{ 0u }; i < t_mat->GetTextureCount(t_type); ++i)
    {
        aiString str;
        const auto result{ t_mat->GetTexture(t_type, i, &str) };
        if (result == aiReturn_FAILURE)
        {
            throw SG_EXCEPTION("[Model::LoadMaterialTextures()] Error while loading material texture.");
        }

        const auto& texture{ ResourceManager::LoadTexture(m_directory + "/" + str.C_Str()) };
        textures.push_back(texture.id);
    }

    return textures;
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::Model::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Model::CleanUp()] Clean up Model.");

    // ...
}
