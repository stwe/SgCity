#include "Vbo.h"
#include "OpenGL.h"
#include "Assert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Vbo::Vbo()
{
    Log::SG_LOG_DEBUG("[Vbo::Vbo()] Create Vbo.");

    CreateId();
}

sg::ogl::buffer::Vbo::~Vbo() noexcept
{
    Log::SG_LOG_DEBUG("[Vbo::~Vbo()] Destruct Vbo.");

    CleanUp();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::buffer::Vbo::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void sg::ogl::buffer::Vbo::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-------------------------------------------------
// Store data by calling glBufferData
//-------------------------------------------------

void sg::ogl::buffer::Vbo::StoreMat4(const std::vector<glm::mat4>& t_matrices, uint32_t t_usage) const
{
    SG_ASSERT(t_usage == GL_STATIC_DRAW || t_usage == GL_DYNAMIC_DRAW, "[Vbo::StoreMat4()] Invalid usage given.")

    const auto floatCount{ 16 * static_cast<long>(t_matrices.size()) };

    Bind();
    glBufferData(GL_ARRAY_BUFFER, floatCount * static_cast<long>(sizeof(float)), t_matrices.data(), t_usage);
    Unbind();
}

void sg::ogl::buffer::Vbo::StoreMat4(const std::vector<glm::mat4>& t_matrices) const
{
    StoreMat4(t_matrices, GL_STATIC_DRAW);
}

void sg::ogl::buffer::Vbo::StoreFloat(const std::vector<float>& t_floats, uint32_t t_usage) const
{
    SG_ASSERT(t_usage == GL_STATIC_DRAW || t_usage == GL_DYNAMIC_DRAW, "[Vbo::StoreFloat()] Invalid usage given.")

    Bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(t_floats.size()) * static_cast<long>(sizeof(float)), t_floats.data(), t_usage);
    Unbind();
}

void sg::ogl::buffer::Vbo::StoreFloat(const std::vector<float>& t_floats) const
{
    StoreFloat(t_floats, GL_STATIC_DRAW);
}

//-------------------------------------------------
// Attributes
//-------------------------------------------------

void sg::ogl::buffer::Vbo::AddFloatAttribute(uint32_t t_index,
                                             int32_t t_nrOfFloatComponents,
                                             int32_t t_nrOfAllFloats,
                                             uint64_t t_startPoint,
                                             bool t_instancedRendering) const
{
    Bind();

    glEnableVertexAttribArray(t_index);
    glVertexAttribPointer(
        t_index,
        t_nrOfFloatComponents,
        GL_FLOAT,
        GL_FALSE,
        t_nrOfAllFloats * static_cast<int32_t>(sizeof(float)),
        reinterpret_cast<uintptr_t*>(t_startPoint * sizeof(float))
    );

    if (t_instancedRendering) {
        glVertexAttribDivisor(t_index, 1);
    }

    Unbind();
}

void sg::ogl::buffer::Vbo::AddFloatAttribute(uint32_t t_index,
                                             int32_t t_nrOfFloatComponents,
                                             int32_t t_nrOfAllFloats,
                                             uint64_t t_startPoint) const
{
    AddFloatAttribute(t_index, t_nrOfFloatComponents, t_nrOfAllFloats, t_startPoint, false);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Vbo::CleanUp()
{
    Log::SG_LOG_DEBUG("[Vbo::CleanUp()] Clean up Vbo Id {}.", id);

    Unbind();
    glDeleteBuffers(1, &id);

    Log::SG_LOG_DEBUG("[Vbo::CleanUp()] Vbo Id {} was deleted.", id);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Vbo::CreateId()
{
    glGenBuffers(1, &id);
    SG_ASSERT(id, "[Vbo::CreateId()] Error while creating a new Vbo.")

    Log::SG_LOG_DEBUG("[Vbo::CreateId()] A new Vbo was created. The Id is {}.", id);
}
