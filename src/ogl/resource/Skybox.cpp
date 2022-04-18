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

#include "Skybox.h"
#include "SgAssert.h"
#include "ResourceManager.h"
#include "SgException.h"
#include "stb_image.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Skybox::Skybox()
{
    Log::SG_LOG_DEBUG("[Skybox::Skybox()] Create Skybox.");

    LoadFaces();
    CreateBuffer();
}

sg::ogl::resource::Skybox::~Skybox()
{
    Log::SG_LOG_DEBUG("[Skybox::~Skybox()] Destruct Skybox.");

    CleanUp();
}

//-------------------------------------------------
// Bind
//-------------------------------------------------

void sg::ogl::resource::Skybox::Bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void sg::ogl::resource::Skybox::BindForReading() const
{
    glActiveTexture(GL_TEXTURE0);
    Bind();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::resource::Skybox::Render(const Window& t_window, const camera::Camera& t_camera) const
{
    OpenGL::SetEqualDepthFunction();

    const auto& shaderProgram{ ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/skybox") };
    shaderProgram.Bind();
    shaderProgram.SetUniform("cubeSampler", 0);
    shaderProgram.SetUniform("viewMatrix", glm::mat4(glm::mat3(t_camera.GetViewMatrix())));
    shaderProgram.SetUniform("projectionMatrix", t_window.GetProjectionMatrix());

    BindForReading();

    m_vao->Bind();
    m_vao->DrawPrimitives();
    m_vao->Unbind();

    ShaderProgram::Unbind();

    OpenGL::SetDefaultDepthFunction();
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::resource::Skybox::CreateId()
{
    glGenTextures(1, &id);
    SG_ASSERT(id, "[Skybox::CreateId()] Error while creating a new texture handle.")

    Log::SG_LOG_DEBUG("[Skybox::CreateId()] A new texture handle was created. The Id is {}.", id);
}

void sg::ogl::resource::Skybox::CreateBuffer()
{
    m_vao = std::make_unique<buffer::Vao>();
    m_vao->CreateStaticSkyboxVbo();
}

//-------------------------------------------------
// Load
//-------------------------------------------------

void sg::ogl::resource::Skybox::LoadFaces()
{
    CreateId();
    Bind();

    int width, height, channels;
    for (auto i{ 0u }; i < FACES.size(); ++i)
    {
        if (auto* data{ stbi_load(FACES[i].c_str(), &width, &height, &channels, 0) })
        {
            SG_ASSERT(width, "[Skybox::LoadFaces()] Invalid image format.")
            SG_ASSERT(height, "[Skybox::LoadFaces()] Invalid image format.")
            SG_ASSERT(channels == 3, "[Skybox::LoadFaces()] Invalid image format.")

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);

            Log::SG_LOG_DEBUG("[Skybox::LoadFaces()] Cubemap texture {} was successfully loaded.", FACES[i]);
        }
        else
        {
            throw SG_EXCEPTION("[Skybox::LoadFaces()] Cubemap texture failed to load at path: " + FACES[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::Skybox::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Skybox::CleanUp()] Clean up Cubemap texture Id {}.", id);

    if (id)
    {
        glDeleteTextures(1, &id);
    }
}
