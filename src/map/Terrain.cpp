#include <vector>
#include <cstdint>
#include "Terrain.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"
#include "ogl/math/Transform.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::Terrain::Terrain(int t_vertexCount)
    : m_vertexCount{ t_vertexCount }
{
    Log::SG_LOG_DEBUG("[Terrain::Terrain()] Create Terrain.");

    Init();
}

sg::map::Terrain::~Terrain() noexcept
{
    Log::SG_LOG_DEBUG("[Terrain::~Terrain()] Destruct Terrain.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::Terrain::Init()
{
    std::vector<float> vertices;
    for (auto z{ 0 }; z < m_vertexCount; ++z)
    {
        for (auto x{ 0 }; x < m_vertexCount; ++x)
        {
            // position (3 floats)
            vertices.push_back(static_cast<float>(x));
            vertices.push_back(0.0f);
            vertices.push_back(static_cast<float>(z));

            // uv (2 floats)
            vertices.push_back(static_cast<float>(x) / (static_cast<float>(m_vertexCount) - 1));
            vertices.push_back(static_cast<float>(z) / (static_cast<float>(m_vertexCount) - 1));

            auto i{ static_cast<int>(z) * 4 + static_cast<int>(x) };

            // convert "i", the integer mesh Id, into an RGB color
            int r = (i & 0x000000FF) >> 0;
            int g = (i & 0x0000FF00) >> 8;
            int b = (i & 0x00FF0000) >> 16;

            auto idColor{ glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f) };

            // color (3 floats)
            vertices.push_back(idColor.x * 5);
            vertices.push_back(idColor.y);
            vertices.push_back(idColor.z);
        }
    }

    std::vector<uint32_t> indices;
    for (auto gz{ 0 }; gz < m_vertexCount - 1; ++gz)
    {
        for (auto gx{ 0 }; gx < m_vertexCount - 1; ++gx)
        {
            /*
            const auto topLeft = (gz * count) + gx;
            const auto topRight = topLeft + 1;
            const auto bottomLeft = ((gz + 1) * count) + gx;
            const auto bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
            */

            const auto i0 = (gz * 4 ) + gx;
            indices.push_back(i0);
            indices.push_back(i0 + 4 + 1);
            indices.push_back(i0 + 1);
            indices.push_back(i0);
            indices.push_back(i0 + 4);
            indices.push_back(i0 + 4 + 1);
        }
    }

    m_terrainVao = std::make_unique<ogl::buffer::Vao>();
    m_terrainVao->AddFloatVbo(vertices);
    m_terrainVao->AddIndexBuffer(indices);

    m_terrainShaderProgram = std::make_unique<ogl::resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_terrainShaderProgram->Load();
}

void sg::map::Terrain::Render(const ogl::Window& t_window, const ogl::camera::Camera& t_camera)
{
    ogl::OpenGL::EnableAlphaBlending();

    m_terrainVao->Bind();
    m_terrainShaderProgram->Bind();

    auto modelMatrix{ ogl::math::Transform::CreateModelMatrix(
        glm::vec3(-2.0f, 0.0f, -2.0f),
        glm::vec3(0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    ) };

    m_terrainShaderProgram->SetUniform("model", modelMatrix);
    m_terrainShaderProgram->SetUniform("view", t_camera.GetViewMatrix());
    m_terrainShaderProgram->SetUniform("projection", t_window.GetProjectionMatrix());
    m_terrainShaderProgram->SetUniform("col", glm::vec3(0.1, 0.8, 0.1));

    //m_terrainVao->DrawPrimitives(GL_POINTS);
    m_terrainVao->DrawPrimitives();

    m_terrainShaderProgram->Unbind();
    m_terrainVao->Unbind();

    ogl::OpenGL::DisableBlending();
}
