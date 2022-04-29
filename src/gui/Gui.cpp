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

#include "Gui.h"
#include "Log.h"
#include "Application.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/math/Transform.h"
#include "ogl/resource/ResourceManager.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::gui::Gui::Gui()
{
    Log::SG_LOG_DEBUG("[Gui::Gui()] Create Gui.");

    m_vao = std::make_unique<ogl::buffer::Vao>();
    m_vao->CreateStaticGuiVbo();
}

sg::gui::Gui::~Gui() noexcept
{
    Log::SG_LOG_DEBUG("[Gui::~Gui()] Destruct Gui.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::gui::Gui::Render(const float t_x, const float t_y, const float t_scaleX, const float t_scaleY, const uint32_t t_id) const
{
    ogl::OpenGL::EnableAlphaBlending();
    ogl::OpenGL::DisableDepthTesting();

    const auto& shaderProgram{ ogl::resource::ResourceManager::LoadShaderProgram(Application::RESOURCES_PATH + "shader/gui") };
    shaderProgram.Bind();

    const auto modelMatrix = ogl::math::Transform::CreateModelMatrix(
        glm::vec3(t_x, t_y, 0.0f),
        glm::vec3(0.0f),
        glm::vec3(t_scaleX, t_scaleY, 1.0f)
    );

    shaderProgram.SetUniform("transformationMatrix", modelMatrix);

    // todo: method in texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t_id);
    shaderProgram.SetUniform("guiTexture", 0);

    m_vao->Bind();
    m_vao->DrawPrimitives(GL_TRIANGLE_STRIP);
    m_vao->Unbind();

    ogl::resource::ShaderProgram::Unbind();

    ogl::OpenGL::EnableDepthTesting();
    ogl::OpenGL::DisableBlending();
}
