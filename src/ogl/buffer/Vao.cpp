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
// Getter
//-------------------------------------------------

int32_t sg::ogl::buffer::Vao::GetDrawCount() const
{
    return m_drawCount;
}

//-------------------------------------------------
// Setter
//-------------------------------------------------

void sg::ogl::buffer::Vao::SetDrawCount(int32_t t_drawCount)
{
    m_drawCount = t_drawCount;
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Vao::Bind() const
{
    glBindVertexArray(id);
}

void sg::ogl::buffer::Vao::Unbind() const
{
    glBindVertexArray(0);
}

//-------------------------------------------------
// Add Vbo
//-------------------------------------------------

sg::ogl::buffer::Vbo& sg::ogl::buffer::Vao::AddVbo()
{
    return *m_vbos.emplace_back(std::make_unique<Vbo>());
}

void sg::ogl::buffer::Vao::Add2DQuadVbo()
{
    const float vertices[] =
    {
        // pos      // uv
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    Bind();

    auto& vbo{ AddVbo() };

    vbo.Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    vbo.Unbind();

    // enable location 0 (position)
    vbo.AddFloatAttribute(0, 2, 4, 0);

    // enable location 1 (uv)
    vbo.AddFloatAttribute(1, 2, 4, 2);

    Unbind();
    SetDrawCount(6);
}

//-------------------------------------------------
// Draw
//-------------------------------------------------

void sg::ogl::buffer::Vao::DrawPrimitives(uint32_t t_drawMode, int32_t t_first) const
{
    glDrawArrays(t_drawMode, t_first, m_drawCount);
}

void sg::ogl::buffer::Vao::DrawPrimitives(uint32_t t_drawMode) const
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

    for (auto& vbo : m_vbos)
    {
        vbo->CleanUp();
    }

    Unbind();
    glDeleteVertexArrays(1, &id);

    Log::SG_LOG_DEBUG("[Vao::CleanUp()] Vao Id {} was deleted.", id);
}
