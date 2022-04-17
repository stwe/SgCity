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

        /**
         * A model consists of one or more VAOs.
         */
        std::vector<std::shared_ptr<Mesh>> meshes;

        /**
         * The bounding sphere.
         */
        camera::Sphere sphere;

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

        /**
         * Renders the model.
         *
         * @param t_window The current window to get the projection matrix.
         * @param t_camera The camera to get the view matrix.
         * @param t_position The position of the model.
         * @param t_rotation The rotation of the model.
         * @param t_scale The scale of the model.
         */
        void Render(
            const Window& t_window,
            const camera::Camera& t_camera,
            const glm::vec3& t_position,
            const glm::vec3& t_rotation,
            const glm::vec3& t_scale
        ) const;

        /**
         * Renders the model.
         *
         * @param t_window The current window to get the projection matrix.
         * @param t_camera The camera to get the view matrix.
         * @param t_position The position of the model.
         */
        void Render(
            const Window& t_window,
            const camera::Camera& t_camera,
            const glm::vec3& t_position
            ) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::string m_fullFilePath;
        std::string m_directory;

        glm::vec3 m_minAabb{ glm::vec3(std::numeric_limits<float>::max()) };
        glm::vec3 m_maxAabb{ glm::vec3(std::numeric_limits<float>::min()) };

        //-------------------------------------------------
        // Load
        //-------------------------------------------------

        void LoadFromFile(unsigned int t_pFlags);
        void ProcessNode(const aiNode* t_node, const aiScene* t_scene);
        std::unique_ptr<Mesh> ProcessMesh(const aiMesh* t_mesh, const aiScene* t_scene);
        std::vector<uint32_t> LoadMaterialTextures(const aiMaterial* t_mat, aiTextureType t_type) const;

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
