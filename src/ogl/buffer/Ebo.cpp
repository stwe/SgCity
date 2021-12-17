#include "Ebo.h"
#include "Assert.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::buffer::Ebo::Ebo()
{
    Log::SG_LOG_DEBUG("[Ebo::Ebo()] Create Ebo.");

    CreateId();
}

sg::ogl::buffer::Ebo::~Ebo() noexcept
{
    Log::SG_LOG_DEBUG("[Ebo::~Ebo()] Destruct Ebo.");

    CleanUp();
}

//-------------------------------------------------
// Bind
//-------------------------------------------------

void sg::ogl::buffer::Ebo::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::buffer::Ebo::CreateId()
{
    glGenBuffers(1, &id);
    SG_ASSERT(id, "[Ebo::CreateId()] Error while creating a new Ebo.")

    Log::SG_LOG_DEBUG("[Ebo::CreateId()] A new Ebo was created. The Id is {}.", id);
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::buffer::Ebo::CleanUp()
{
    Log::SG_LOG_DEBUG("[Ebo::CleanUp()] Clean up Ebo Id {}.", id);

    if (id)
    {
        glDeleteBuffers(1, &id);
    }

    Log::SG_LOG_DEBUG("[Ebo::CleanUp()] Ebo Id {} was deleted.", id);
}
