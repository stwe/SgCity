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

#include "Model.h"
#include "SgException.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ResourceManager.h"
#include "ogl/math/Transform.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_path)
    : m_path{ std::move(t_path) }
{
    Log::SG_LOG_DEBUG("[Model::Model()] Create Model.");

    Init();
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
    if (m_vaos.empty())
    {
        return;
    }

    ogl::OpenGL::EnableAlphaBlending();

    m_vaos[0]->Bind(); // todo [0]

    auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("/home/steffen/CLionProjects/SgCity/resources/shader/model") };
    shaderProgram.Bind();

    // todo
    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        t_position,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.25f)
    ) };

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    const auto& texture{ ogl::resource::ResourceManager::LoadTexture("/home/steffen/CLionProjects/SgCity/resources/texture/building.png", true) };
    texture.BindForReading(GL_TEXTURE0);
    shaderProgram.SetUniform("diffuseMap", 0);

    m_vaos[0]->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    m_vaos[0]->Unbind();

    ogl::OpenGL::DisableBlending();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::resource::Model::Init()
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "";

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(m_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            throw SG_EXCEPTION("[Model::Init()] Error: " + reader.Error());
        }
    }

    if (!reader.Warning().empty())
    {
        Log::SG_LOG_WARN("[Model::Init()] Warning: {}.", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> vertices;

    // Loop over shapes
    for (auto s{ 0u }; s < shapes.size(); ++s)
    {
        // Loop over faces(polygon)
        auto index_offset{ 0u };
        for (auto f{ 0u }; f < shapes[s].mesh.num_face_vertices.size(); ++f)
        {
            auto fv{ size_t(shapes[s].mesh.num_face_vertices[f]) };

            // Loop over vertices in the face.
            for (auto v{ 0u }; v < fv; ++v)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

                    vertices.push_back(tx);
                    vertices.push_back(ty);
                }
            }

            index_offset += fv;

            // per-face material
            //shapes[s].mesh.material_ids[f];
        }

        auto vao{ std::make_unique<ogl::buffer::Vao>() };
        vao->CreateStaticVbo(vertices, 12 * 3); // todo: drawCount
        m_vaos.push_back(std::move(vao));
    }
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::Model::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Model::CleanUp()] Clean up Model.");

    // ...
}
