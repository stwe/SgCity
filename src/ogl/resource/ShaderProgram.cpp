#include "ShaderProgram.h"
#include "OpenGL.h"
#include "Assert.h"
#include "SgException.h"
#include "ResourceUtil.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::ShaderProgram::ShaderProgram(std::string t_path)
    : m_path{ std::move(t_path) }
{
    Log::SG_LOG_DEBUG("[ShaderProgram::ShaderProgram()] Create ShaderProgram.");
}

sg::ogl::resource::ShaderProgram::~ShaderProgram() noexcept
{
    Log::SG_LOG_DEBUG("[ShaderProgram::~ShaderProgram()] Destruct ShaderProgram.");

    CleanUp();
}

//-------------------------------------------------
// Load
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::Load()
{
    CreateId();

    AddVertexShader(ResourceUtil::ReadShaderFile(m_path + "/Vertex.vert"));
    AddFragmentShader(ResourceUtil::ReadShaderFile(m_path + "/Fragment.frag"));

    //LinkAndValidateProgram();
    //AddFoundUniforms();
}

//-------------------------------------------------
// Bind / unbind
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::Bind() const
{
    glUseProgram(id);
}

void sg::ogl::resource::ShaderProgram::Unbind() const
{
    glUseProgram(0);
}

//-------------------------------------------------
// Create
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::CreateId()
{
    id = glCreateProgram();
    SG_ASSERT(id, "[ShaderProgram::CreateId()] Error while creating a new Shader Program.");

    Log::SG_LOG_DEBUG("[ShaderProgram::CreateId()] A new Shader Program was created. The Id is {}.", id);
}

//-------------------------------------------------
// Add shader types
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::AddVertexShader(const std::string& t_shaderCode)
{
    m_vertexShaderId = AddShader(t_shaderCode, GL_VERTEX_SHADER);
    Log::SG_LOG_DEBUG("[ShaderProgram::AddVertexShader()] A new vertex shader was added. The Id is {}.", m_vertexShaderId);
}

void sg::ogl::resource::ShaderProgram::AddFragmentShader(const std::string& t_shaderCode)
{
    m_fragmentShaderId = AddShader(t_shaderCode, GL_FRAGMENT_SHADER);
    Log::SG_LOG_DEBUG("[ShaderProgram::AddFragmentShader()] A new fragment shader was added. The Id is {}.", m_fragmentShaderId);
}

//-------------------------------------------------
// Shader
//-------------------------------------------------

uint32_t sg::ogl::resource::ShaderProgram::CreateShaderObject(int32_t t_shaderType)
{
    const auto shaderId{ glCreateShader(t_shaderType) };
    SG_ASSERT(shaderId, "[ShaderProgram::CreateShaderObject()] Shader object creation has failed.");

    return shaderId;
}

void sg::ogl::resource::ShaderProgram::CompileShader(uint32_t t_shaderId, const std::string& t_shaderCode)
{
    auto shaderSrc{ t_shaderCode.c_str() };
    glShaderSource(t_shaderId, 1, &shaderSrc, nullptr);
    glCompileShader(t_shaderId);
}

void sg::ogl::resource::ShaderProgram::CheckCompileStatus(uint32_t t_shaderId)
{
    auto isCompiled{ GL_FALSE };
    glGetShaderiv(t_shaderId, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        auto maxLength{ 0 };
        glGetShaderiv(t_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> errorLog(maxLength);
        glGetShaderInfoLog(t_shaderId, maxLength, &maxLength, &errorLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(t_shaderId);

        std::string log;
        for (const auto& value : errorLog)
        {
            log.push_back(value);
        }

        throw SG_EXCEPTION("[ShaderProgram::CheckCompileStatus()] Error while compiling shader program. Log: " + log);
    }
}

void sg::ogl::resource::ShaderProgram::FindUniforms(const std::string& t_shaderCode)
{

}

uint32_t sg::ogl::resource::ShaderProgram::AddShader(const std::string& t_shaderCode, int32_t t_shaderType)
{
    const auto shaderId{ CreateShaderObject(t_shaderType) };
    CompileShader(shaderId, t_shaderCode);
    CheckCompileStatus(shaderId);
    glAttachShader(id, shaderId);

    FindUniforms(t_shaderCode);

    return shaderId;
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::ShaderProgram::CleanUp() const
{
    Log::SG_LOG_DEBUG("[ShaderProgram::CleanUp()] Clean up Shader Program Id {}.", id);

    Unbind();

    if (m_vertexShaderId)
    {
        glDeleteShader(m_vertexShaderId);
        Log::SG_LOG_DEBUG("[ShaderProgram::CleanUp()] Vertex shader Id {} was deleted.", m_vertexShaderId);
    }

    if (m_fragmentShaderId)
    {
        glDeleteShader(m_fragmentShaderId);
        Log::SG_LOG_DEBUG("[ShaderProgram::CleanUp()] Fragment shader Id {} was deleted.", m_fragmentShaderId);
    }

    if (id)
    {
        glDeleteProgram(id);
        Log::SG_LOG_DEBUG("[ShaderProgram::CleanUp()] Shader Program Id {} was deleted.", id);
    }
}
