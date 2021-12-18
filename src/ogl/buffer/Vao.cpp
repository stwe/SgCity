#include "Vao.h"
#include "Assert.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vao::Vao()
{
    Log::SG_LOG_DEBUG("[Vao::Vao()] Create Vao.");

    CreateId();
}

sg::ogl::buffer::Vao::~Vao() noexcept
{
    Log::SG_LOG_DEBUG("[Vao::~Vao()] Destruct Vao.");

    CleanUp();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Vao::Bind() const
{
    glBindVertexArray(id);
}

void sg::ogl::buffer::Vao::Unbind()
{
    glBindVertexArray(0);
}

//-------------------------------------------------
// Create Vbo
//-------------------------------------------------

void sg::ogl::buffer::Vao::CreateEmptyDynamicVbo(const uint32_t t_size, const int32_t t_drawCount)
{
    Bind();

    m_vbo = std::make_unique<Vbo>();
    m_vbo->Bind();

    glBufferData(GL_ARRAY_BUFFER, t_size, nullptr, GL_DYNAMIC_DRAW);
    Vbo::Unbind();

    // enable location 0 (position)
    m_vbo->AddFloatAttribute(0, 3, 11, 0);

    // enable location 1 (uv)
    m_vbo->AddFloatAttribute(1, 2, 11, 3);

    // enable location 2 (idColor)
    m_vbo->AddFloatAttribute(2, 3, 11, 5);

    // enable location 3 (normal)
    m_vbo->AddFloatAttribute(3, 3, 11, 8);

    Unbind();

    if (t_drawCount > 0)
    {
        drawCount = t_drawCount;
    }
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode, const int32_t t_first) const
{
    glDrawArrays(t_drawMode, t_first, drawCount);
}

void sg::ogl::buffer::Vao::DrawPrimitives(const uint32_t t_drawMode) const
{
    DrawPrimitives(t_drawMode, 0);
}

void sg::ogl::buffer::Vao::DrawPrimitives() const
{
    DrawPrimitives(GL_TRIANGLES);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Vao::CreateId()
{
    glGenVertexArrays(1, &id);
    SG_ASSERT(id, "[Vao::CreateId()] Error while creating a new Vao.")

    Log::SG_LOG_DEBUG("[Vao::CreateId()] A new Vao was created. The Id is {}.", id);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Vao::CleanUp()
{
    Log::SG_LOG_DEBUG("[Vao::CleanUp()] Clean up Vao Id {}.", id);

    glDisableVertexAttribArray(0);

    Unbind();

    if (id)
    {
        glDeleteVertexArrays(1, &id);
    }

    Log::SG_LOG_DEBUG("[Vao::CleanUp()] Vao Id {} was deleted.", id);
}
