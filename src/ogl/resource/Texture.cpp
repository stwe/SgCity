#include "Texture.h"
#include "Assert.h"
#include "SgException.h"
#include "ogl/OpenGL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Texture::Texture(std::string t_path)
    : m_path{ std::move(t_path) }
{
    Log::SG_LOG_DEBUG("[Texture::Texture()] Create Texture.");
}

sg::ogl::resource::Texture::Texture(std::string t_path, bool t_loadVerticalFlipped)
    : m_path{ std::move(t_path) }
    , m_loadVerticalFlipped{ t_loadVerticalFlipped }
{
    Log::SG_LOG_DEBUG("[Texture::Texture()] Create Texture.");
}

sg::ogl::resource::Texture::~Texture() noexcept
{
    Log::SG_LOG_DEBUG("[Texture::~Texture()] Destruct Texture.");

    CleanUp();
}

//-------------------------------------------------
// Load
//-------------------------------------------------

void sg::ogl::resource::Texture::Load()
{
    CreateId();
    LoadFromFile();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::resource::Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void sg::ogl::resource::Texture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void sg::ogl::resource::Texture::BindForReading(uint32_t t_textureUnit) const
{
    // make sure that the OpenGL constants are used here
    if (t_textureUnit < GL_TEXTURE0 || t_textureUnit > GL_TEXTURE15) {
        SG_ASSERT(false, "[Texture::BindForReading()] Invalid texture unit value.")
    }

    glActiveTexture(t_textureUnit);
    Bind();
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::resource::Texture::CreateId()
{
    glGenTextures(1, &id);
    SG_ASSERT(id, "[Texture::CreateId()] Error while creating a new texture handle.")

    Log::SG_LOG_DEBUG("[Texture::CreateId()] A new texture handle was created. The Id is {}.", id);
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::resource::Texture::LoadFromFile()
{
    stbi_set_flip_vertically_on_load(m_loadVerticalFlipped);

    auto* const image{ stbi_load(m_path.c_str(), &m_width, &m_height, &m_channels, 0) };
    if (image)
    {
        SG_ASSERT(m_width, "[Texture::LoadFromFile()] Invalid image format.")
        SG_ASSERT(m_height, "[Texture::LoadFromFile()] Invalid image format.")
        SG_ASSERT(m_channels, "[Texture::LoadFromFile()] Invalid image format.")

        if (m_channels == STBI_grey)
            m_format = GL_RED;
        else if (m_channels == STBI_rgb)
            m_format = GL_RGB;
        else if (m_channels == STBI_rgb_alpha)
            m_format = GL_RGBA;

        SG_ASSERT(m_format, "[Texture::LoadFromFile()] Invalid image format.")

        Bind();

        glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);

        Log::SG_LOG_DEBUG("[Texture::LoadFromFile()] Texture {} was successfully loaded.", m_path);
    }
    else
    {
        throw SG_EXCEPTION("[Texture::LoadFromFile()] Texture failed to load at path: " + m_path);
    }
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::Texture::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Texture::CleanUp()] Clean up Texture Id {}.", id);

    if (id)
    {
        glDeleteTextures(1, &id);
    }
}
