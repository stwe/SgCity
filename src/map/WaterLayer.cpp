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

#include "WaterLayer.h"
#include "Application.h"
#include "Log.h"
#include "ogl/Window.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::WaterLayer::WaterLayer(std::shared_ptr<ogl::Window> t_window, const int t_tileCount)
    : Layer(std::move(t_window), t_tileCount)
{
    Log::SG_LOG_DEBUG("[WaterLayer::WaterLayer()] Create WaterLayer.");

    Init();
}

sg::map::WaterLayer::~WaterLayer() noexcept
{
    Log::SG_LOG_DEBUG("[WaterLayer::~WaterLayer()] Destruct WaterLayer.");
}

//-------------------------------------------------
// Override
//-------------------------------------------------

void sg::map::WaterLayer::Update()
{
    m_moveFactor += 0.006f * static_cast<float>(Application::FRAME_TIME);
    m_moveFactor = fmod(m_moveFactor, 1.0f);
}

void sg::map::WaterLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane) const
{
    ogl::OpenGL::EnableAlphaBlending();
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/water") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", window->GetProjectionMatrix());

    // todo: method in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->reflectionColorTextureId);
    shaderProgram.SetUniform("reflectionTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->refractionColorTextureId);
    shaderProgram.SetUniform("refractionTexture", 1);

    const auto& dudvTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/water/waterDUDV.png") };
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture.id);
    shaderProgram.SetUniform("dudvTexture", 2);

    const auto& normalTexture{ ogl::resource::ResourceManager::LoadTexture("E:/Dev/SgCity/resources/water/normal.png") };
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalTexture.id);
    shaderProgram.SetUniform("normalTexture", 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->refractionDepthTextureId);
    shaderProgram.SetUniform("depthTexture", 4);

    shaderProgram.SetUniform("cameraPosition", t_camera.position);
    shaderProgram.SetUniform("moveFactor", m_moveFactor);

    shaderProgram.SetUniform("lightPosition", glm::vec3(0.5, 1.0, 0.0));
    shaderProgram.SetUniform("lightColor", glm::vec3(0.8, 0.8, 0.8));

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
    ogl::OpenGL::DisableBlending();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::WaterLayer::Init()
{
    Log::SG_LOG_DEBUG("[WaterLayer::Init()] Initialize the WaterLayer.");

    m_waterFbos = std::make_unique<ogl::buffer::WaterFbos>(Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT);
    position = glm::vec3(tileCount / 2, -WATER_HEIGHT, tileCount / 2);
    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(static_cast<float>(tileCount) * 0.5f, 1.0f, static_cast<float>(tileCount) * 0.5f)
    );

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateStaticWaterVbo();

    Log::SG_LOG_DEBUG("[WaterLayer::Init()] The WaterLayer was successfully initialized.");
}
