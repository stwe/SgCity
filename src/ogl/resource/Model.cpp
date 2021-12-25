#include "Model.h"
#include "SgException.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/resource/Texture.h"
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

void sg::ogl::resource::Model::Render(const Window& t_window, const camera::Camera& t_camera) const
{
    if (m_vaos.empty())
    {
        return;
    }

    ogl::OpenGL::EnableAlphaBlending();

    m_vaos[0]->Bind(); // todo [0]
    m_modelShaderProgram->Bind();

    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        glm::vec3(14.0f, 0.0f, 9.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f)
    ) };

    m_modelShaderProgram->SetUniform("model", modelMatrix);
    m_modelShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_modelShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());

    m_modelTexture->BindForReading(GL_TEXTURE0);
    m_modelShaderProgram->SetUniform("diffuseMap", 0);

    m_vaos[0]->DrawPrimitives();

    m_modelShaderProgram->Unbind();
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
        vao->CreateStaticVbo(vertices, 4 * 4); // todo: drawCount
        m_vaos.push_back(std::move(vao));

        m_modelShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/model");
        m_modelShaderProgram->Load();

        m_modelTexture = std::make_unique<ogl::resource::Texture>("/home/steffen/CLionProjects/SgCity/resources/model/Tree_01/billboard0.png");
        m_modelTexture->Load();
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
