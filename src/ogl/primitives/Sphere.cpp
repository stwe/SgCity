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

#include "Sphere.h"
#include "Log.h"
#include "ogl/Window.h"
#include "ogl/buffer/Vao.h"
#include "ogl/OpenGL.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::primitives::Sphere::Sphere(std::shared_ptr<Window> t_window, const float t_radius, const int t_slices, const int t_stacks)
    : m_window{ std::move(t_window) }
{
    Log::SG_LOG_DEBUG("[Sphere::Sphere()] Create Sphere.");

    m_sphereVao = std::make_unique<buffer::Vao>();
    m_sphereVao->CreateStaticSphereVbos(t_radius, t_slices, t_stacks);
}

sg::ogl::primitives::Sphere::~Sphere() noexcept
{
    Log::SG_LOG_DEBUG("[Sphere::~Sphere()] Destruct Sphere.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::primitives::Sphere::Render(
    const camera::Camera& t_camera,
    const glm::vec3& t_position,
    const glm::vec3& t_rotation,
    const glm::vec3& t_scale
) const
{
    m_sphereVao->Bind();

    const auto& shaderProgram{ resource::ResourceManager::LoadShaderProgram("E:/Dev/SgCity/resources/shader/sphere") };
    shaderProgram.Bind();
    shaderProgram.SetUniform("model", math::Transform::CreateModelMatrix(t_position, t_rotation, t_scale));
    shaderProgram.SetUniform("view", t_camera.GetViewMatrix());
    shaderProgram.SetUniform("projection", m_window->GetProjectionMatrix());

    OpenGL::EnableWireframeMode();
    m_sphereVao->DrawPrimitives();
    OpenGL::DisableWireframeMode();

    resource::ShaderProgram::Unbind();
    m_sphereVao->Unbind();
}
