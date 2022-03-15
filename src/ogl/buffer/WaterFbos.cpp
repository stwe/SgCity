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

#include "WaterFbos.h"
#include "SgAssert.h"
#include "SgException.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::WaterFbos::WaterFbos(const int32_t t_width, const int32_t t_height)
    : m_width{ t_width }
    , m_height{ t_height }
{
    Log::SG_LOG_DEBUG("[WaterFbos::WaterFbos()] Create WaterFbos.");

    SG_ASSERT(m_width, "[WaterFbos::WaterFbos()] Invalid width.")
    SG_ASSERT(m_height, "[WaterFbos::WaterFbos()] Invalid height.")

    reflectionWidth = m_width / 2;
    reflectionHeight = m_height / 2;

    refractionWidth = m_width;
    refractionHeight = m_height;

    InitReflectionFbo();
    InitRefractionFbo();
}

sg::ogl::buffer::WaterFbos::~WaterFbos() noexcept
{
    Log::SG_LOG_DEBUG("[WaterFbos::~WaterFbos()] Destruct WaterFbos.");

    CleanUp();
}

//-------------------------------------------------
// Fbo
//-------------------------------------------------

uint32_t sg::ogl::buffer::WaterFbos::GenerateAndBindFbo()
{
    // generate Fbo
    uint32_t fboId;
    glGenFramebuffers(1, &fboId);

    // bind Fbo
    BindFbo(fboId);

    // always render to color attachment 0
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    return fboId;
}

void sg::ogl::buffer::WaterFbos::BindFbo(const uint32_t t_fboId)
{
    SG_ASSERT(t_fboId, "[WaterFbos::BindFbo()] Invalid Fbo Id.")
    glBindFramebuffer(GL_FRAMEBUFFER, t_fboId);
}

void sg::ogl::buffer::WaterFbos::UnbindFbo()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-------------------------------------------------
// Fbo - Render Target
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::BindAsRenderTarget(const uint32_t t_fbo, const int32_t t_width, const int32_t t_height)
{
    glViewport(0, 0, t_width, t_height);
    glBindTexture(GL_TEXTURE_2D, 0);
    BindFbo(t_fbo);
}

void sg::ogl::buffer::WaterFbos::BindReflectionFboAsRenderTarget() const
{
    BindAsRenderTarget(reflectionFboId, reflectionWidth, reflectionHeight);
}

void sg::ogl::buffer::WaterFbos::BindRefractionFboAsRenderTarget() const
{
    BindAsRenderTarget(refractionFboId, refractionWidth, refractionHeight);
}

void sg::ogl::buffer::WaterFbos::UnbindRenderTarget() const
{
    UnbindFbo();
    glViewport(0, 0, m_width, m_height);
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::InitReflectionFbo()
{
    reflectionFboId = GenerateAndBindFbo();
    reflectionColorTextureId = CreateColorTextureAttachment(reflectionWidth, reflectionHeight);
    reflectionDepthRenderBufferId = CreateDepthRenderBufferAttachment(reflectionWidth, reflectionHeight);

    CheckFbo();

    UnbindFbo();

    Log::SG_LOG_DEBUG("[WaterFbos::InitReflectionFbo()] A new Reflection Fbo was created. Id: {}", reflectionFboId);
}

void sg::ogl::buffer::WaterFbos::InitRefractionFbo()
{
    refractionFboId = GenerateAndBindFbo();
    refractionColorTextureId = CreateColorTextureAttachment(refractionWidth, refractionHeight);
    refractionDepthTextureId = CreateDepthTextureAttachment(refractionWidth, refractionHeight);

    CheckFbo();

    UnbindFbo();

    Log::SG_LOG_DEBUG("[WaterFbos::InitRefractionFbo()] A new Refraction Fbo was created. Id: {}", refractionFboId);
}

uint32_t sg::ogl::buffer::WaterFbos::CreateColorTextureAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    SG_ASSERT(textureId, "[WaterFbos::CreateColorTextureAttachment()] Invalid texture id.")

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

    return textureId;
}

uint32_t sg::ogl::buffer::WaterFbos::CreateDepthTextureAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);

    SG_ASSERT(textureId, "[WaterFbos::CreateDepthTextureAttachment()] Invalid texture id.")

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, t_width, t_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureId, 0);

    return textureId;
}

uint32_t sg::ogl::buffer::WaterFbos::CreateDepthRenderBufferAttachment(const int32_t t_width, const int32_t t_height)
{
    uint32_t depthRenderBufferId;
    glGenRenderbuffers(1, &depthRenderBufferId);

    SG_ASSERT(depthRenderBufferId, "[WaterFbos::CreateDepthRenderBufferAttachment()] Invalid depth render buffer id.")

    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t_width, t_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);

    return depthRenderBufferId;
}

void sg::ogl::buffer::WaterFbos::CheckFbo()
{
    const auto status{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        UnbindFbo();
        throw SG_EXCEPTION("[WaterFbos::CheckFbo()] Error while creating Fbo Attachments.");
    }
}

//-------------------------------------------------
// CleanUp
//-------------------------------------------------

void sg::ogl::buffer::WaterFbos::CleanUp() const
{
    Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Start the OpenGL clean up process for the WaterFbos objects.");

    if (reflectionFboId)
    {
        glDeleteFramebuffers(1, &reflectionFboId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Reflection Fbo was deleted. Id: {}", reflectionFboId);
    }

    if (reflectionColorTextureId)
    {
        glDeleteTextures(1, &reflectionColorTextureId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Reflection texture was deleted. Id: {}", reflectionColorTextureId);
    }

    if (reflectionDepthRenderBufferId)
    {
        glDeleteRenderbuffers(1, &reflectionDepthRenderBufferId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Depth render buffer was deleted. Id: {}", reflectionDepthRenderBufferId);
    }

    if (refractionFboId)
    {
        glDeleteFramebuffers(1, &refractionFboId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction Fbo was deleted. Id: {}", refractionFboId);
    }

    if (refractionColorTextureId)
    {
        glDeleteTextures(1, &refractionColorTextureId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction texture was deleted. Id: {}", refractionColorTextureId);
    }

    if (refractionDepthTextureId)
    {
        glDeleteTextures(1, &refractionDepthTextureId);
        Log::SG_LOG_DEBUG("[WaterFbos::CleanUp()] Refraction depth texture was deleted. Id: {}", refractionDepthTextureId);
    }
}
