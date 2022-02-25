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

#pragma once

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cstdint>
#include <memory>
#include <vector>
#include "ogl/Window.h"
#include "ogl/camera/Camera.h"

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

//-------------------------------------------------
// Model
//-------------------------------------------------

namespace sg::ogl::resource
{
    /**
     * Forward declaration class Mesh.
     */
    class Mesh;

    /**
     * Represents the Model.
     */
    class Model
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::vector<std::shared_ptr<Mesh>> meshes;

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Model() = delete;

        /**
         * Constructs a new Model object.
         *
         * @param t_fullFilePath The full file path to the model file.
         * @param t_pFlags Post processing flags.
         */
        explicit Model(
            std::string t_fullFilePath,
            unsigned int t_pFlags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals
            );

        Model(const Model& t_other) = delete;
        Model(Model&& t_other) noexcept = delete;
        Model& operator=(const Model& t_other) = delete;
        Model& operator=(Model&& t_other) noexcept = delete;

        ~Model() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(
            const ogl::Window& t_window,
            const ogl::camera::Camera& t_camera,
            const glm::vec3& t_position
            );

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::string m_fullFilePath;
        std::string m_directory;

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void LoadFromFile(unsigned int t_pFlags);
        void ProcessNode(aiNode* t_node, const aiScene* t_scene);
        std::unique_ptr<Mesh> ProcessMesh(aiMesh* t_mesh, const aiScene* t_scene) const;
        std::vector<uint32_t> LoadMaterialTextures(aiMaterial* t_mat, aiTextureType t_type) const;

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
