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

#include "PickingTexture.h"
#include "SgException.h"
#include "Assert.h"
#include "ogl/OpenGL.h"
#include "ogl/resource/Texture.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::input::PickingTexture::PickingTexture()
{
    Log::SG_LOG_DEBUG("[PickingTexture::PickingTexture()] Create PickingTexture.");

    CreateId();
}

sg::ogl::input::PickingTexture::~PickingTexture() noexcept
{
    Log::SG_LOG_DEBUG("[PickingTexture::~PickingTexture()] Destruct PickingTexture.");

    CleanUp();
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::input::PickingTexture::Init(const int t_width, const int t_height)
{
    // store width && height
    m_width = t_width;
    m_height = t_height;

    // bind Fbo
    Bind();

    // create the texture object for the primitive information buffer
    glGenTextures(1, &m_pickingTextureId);
    glBindTexture(GL_TEXTURE_2D, m_pickingTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, t_width, t_height, 0, GL_RGB, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTextureId, 0);

    // create the texture object for the depth buffer
    glGenTextures(1, &m_depthTextureId);
    glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, t_width, t_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureId, 0);

    // disable reading to avoid problems with older GPUs
    glReadBuffer(GL_NONE);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // verify that the FBO is correct
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        throw SG_EXCEPTION("[PickingTexture::Init()] Error while creating Fbo attachments.");
    }

    // restore the default framebuffer
    resource::Texture::Unbind();
    Unbind();
}

//-------------------------------------------------
// Write
//-------------------------------------------------

void sg::ogl::input::PickingTexture::EnableWriting() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboId);
}

void sg::ogl::input::PickingTexture::DisableWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Read
//-------------------------------------------------

int sg::ogl::input::PickingTexture::ReadMapIndex(const int t_x, const int t_y) const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    unsigned char rgbaData[4];
    glReadPixels(t_x, m_height - t_y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    // convert the color back to an Id
    return rgbaData[0] +
           rgbaData[1] * 256 +
           rgbaData[2] * 256 * 256;
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::input::PickingTexture::CreateId()
{
    glGenFramebuffers(1, &m_fboId);
    SG_ASSERT(m_fboId, "[PickingTexture::CreateId()] Error while creating a new Fbo.")

    Log::SG_LOG_DEBUG("[PickingTexture::CreateId()] A new Fbo was created. The Id is {}.", m_fboId);
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::input::PickingTexture::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
}

void sg::ogl::input::PickingTexture::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::input::PickingTexture::CleanUp()
{
    Log::SG_LOG_DEBUG("[PickingTexture::CleanUp()] Clean up Fbo Id {}.", m_fboId);

    Unbind();

    if (m_fboId)
    {
        glDeleteFramebuffers(1, &m_fboId);
        Log::SG_LOG_DEBUG("[PickingTexture::CleanUp()] Fbo Id {} was deleted.", m_fboId);
    }

    if (m_pickingTextureId)
    {
        glDeleteTextures(1, &m_pickingTextureId);
        Log::SG_LOG_DEBUG("[PickingTexture::CleanUp()] Picking texture Id {} was deleted.", m_pickingTextureId);
    }

    if (m_depthTextureId)
    {
        glDeleteTextures(1, &m_depthTextureId);
        Log::SG_LOG_DEBUG("[PickingTexture::CleanUp()] Depth texture Id {} was deleted.", m_depthTextureId);
    }
}
