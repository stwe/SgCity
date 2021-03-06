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

#include <imgui.h>
#include "WaterLayer.h"
#include "Game.h"
#include "Log.h"
#include "Map.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::WaterLayer::WaterLayer(const int t_tileCount, std::shared_ptr<ogl::Window> t_window)
    : Layer(std::move(t_window))
    , m_tileCount{ t_tileCount }
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
    m_moveFactor += 0.024f * static_cast<float>(Game::FRAME_TIME);
    m_moveFactor = fmod(m_moveFactor, 1.0f);
}

void sg::map::WaterLayer::Render(const ogl::camera::Camera& t_camera, const glm::vec4& t_plane)
{
    ogl::OpenGL::EnableAlphaBlending();
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram(Game::RESOURCES_PATH + "shader/layer/water") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", window->GetProjectionMatrix());
    shaderProgram.SetUniform("cameraPosition", t_camera.position);
    shaderProgram.SetUniform("lightPosition", glm::vec3(0.5, 1.0, 0.0));

    // todo: method in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->reflectionColorTextureId);
    shaderProgram.SetUniform("reflectionTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->refractionColorTextureId);
    shaderProgram.SetUniform("refractionTexture", 1);

    const auto& dudvTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "water/dudv.png") };
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture.id);
    shaderProgram.SetUniform("dudvTexture", 2);

    const auto& normalTexture{ ogl::resource::ResourceManager::LoadTexture(Game::RESOURCES_PATH + "water/normal.png") };
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalTexture.id);
    shaderProgram.SetUniform("normalTexture", 3);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_waterFbos->refractionDepthTextureId);
    shaderProgram.SetUniform("depthTexture", 4);

    shaderProgram.SetUniform("moveFactor", m_moveFactor);
    shaderProgram.SetUniform("lightColor", glm::vec3(0.8, 0.8, 0.8));

    shaderProgram.SetUniform("near", window->nearPlane);
    shaderProgram.SetUniform("far", window->farPlane);

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
    ogl::OpenGL::DisableBlending();
}

void sg::map::WaterLayer::RenderImGui()
{
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::Text("Water Layer");
    ImGui::PopStyleColor();

    ImGui::Text("reflection");
    if (m_waterFbos->reflectionColorTextureId)
    {
        ImGui::Image(
            reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_waterFbos->reflectionColorTextureId)), // NOLINT(performance-no-int-to-ptr)
            ImVec2(128.0f, 128.0f),
            ImVec2(0.0f, 0.0f),
            ImVec2(1.0f, -1.0f)
        );
    }

    ImGui::Text("refraction");
    if (m_waterFbos->refractionColorTextureId)
    {
        ImGui::Image(
            reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_waterFbos->refractionColorTextureId)), // NOLINT(performance-no-int-to-ptr)
            ImVec2(128.0f, 128.0f),
            ImVec2(0.0f, 0.0f),
            ImVec2(1.0f, -1.0f)
        );
    }
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::WaterLayer::Init()
{
    Log::SG_LOG_DEBUG("[WaterLayer::Init()] Initialize the WaterLayer.");

    m_waterFbos = std::make_unique<ogl::buffer::WaterFbos>(window->GetWidth(), window->GetHeight());
    position = glm::vec3(m_tileCount / 2, -WATER_HEIGHT, m_tileCount / 2);
    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(static_cast<float>(m_tileCount) * 0.5f, 1.0f, static_cast<float>(m_tileCount) * 0.5f)
    );

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateStaticWaterVbo();

    Log::SG_LOG_DEBUG("[WaterLayer::Init()] The WaterLayer was successfully initialized.");
}
