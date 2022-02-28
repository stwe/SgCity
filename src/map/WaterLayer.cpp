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
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::map::WaterLayer::WaterLayer(const int t_tileCount)
    : Layer(t_tileCount)
{
    Log::SG_LOG_DEBUG("[WaterLayer::WaterLayer()] Create WaterLayer.");

    Init();
}

sg::map::WaterLayer::~WaterLayer() noexcept
{
    Log::SG_LOG_DEBUG("[WaterLayer::~WaterLayer()] Destruct WaterLayer.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::map::WaterLayer::Render(const sg::ogl::Window& t_window, const sg::ogl::camera::Camera& t_camera) const
{
    ogl::OpenGL::EnableFaceCulling();

    vao->Bind();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/water") };
    shaderProgram.Bind();

    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", t_window.GetProjectionMatrix());

    vao->DrawPrimitives();

    ogl::resource::ShaderProgram::Unbind();
    vao->Unbind();

    ogl::OpenGL::DisableFaceCulling();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::map::WaterLayer::Init()
{
    Log::SG_LOG_DEBUG("[WaterLayer::Init()] Initialize the WaterLayer.");

    position = glm::vec3(0.0f, -0.8f, 0.0f);

    modelMatrix = ogl::math::Transform::CreateModelMatrix(
        position,
        glm::vec3(0.0f),
        glm::vec3(static_cast<float>(tileCount))
    );

    vao = std::make_unique<ogl::buffer::Vao>();
    vao->CreateStaticWaterVbo();

    Log::SG_LOG_DEBUG("[WaterLayer::Init()] The WaterLayer was successfully initialized.");
}
