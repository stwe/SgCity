#include "SpriteRenderer.h"
#include "Log.h"
#include "ogl/OpenGL.h"
#include "ogl/buffer/Vao.h"
#include "ogl/resource/ShaderProgram.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::renderer::SpriteRenderer::SpriteRenderer()
{
    Log::SG_LOG_DEBUG("[SpriteRenderer::SpriteRenderer()] Create SpriteRenderer.");

    Init();
}

sg::ogl::renderer::SpriteRenderer::~SpriteRenderer() noexcept
{
    Log::SG_LOG_DEBUG("[SpriteRenderer::~SpriteRenderer()] Destruct SpriteRenderer.");

    CleanUp();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::renderer::SpriteRenderer::Init()
{
    m_vao = std::make_unique<buffer::Vao>();
    m_vao->Add2DQuadVbo();

    m_shaderProgram = std::make_unique<resource::ShaderProgram>("/home/steffen/CLionProjects/SgCity/resources/shader/sprite");
    m_shaderProgram->Load();
}

void sg::ogl::renderer::SpriteRenderer::Render(const glm::mat4& t_modelMatrix,
                                               const glm::mat4& t_viewMatrix,
                                               const glm::mat4& t_projectionMatrix,
                                               const resource::Texture& t_texture) const
{
    OpenGL::EnableAlphaBlending();

    m_vao->Bind();
    m_shaderProgram->Bind();

    m_shaderProgram->SetUniform("model", t_modelMatrix);
    m_shaderProgram->SetUniform("view", t_viewMatrix);
    m_shaderProgram->SetUniform("projection", t_projectionMatrix);

    t_texture.BindForReading(GL_TEXTURE0);
    m_shaderProgram->SetUniform("diffuseMap", 0);

    m_vao->DrawPrimitives();

    t_texture.Unbind();
    m_shaderProgram->Unbind();
    m_vao->Unbind();

    OpenGL::DisableBlending();
}

void sg::ogl::renderer::SpriteRenderer::Render(const glm::mat4& t_modelMatrix,
                                               const glm::mat4& t_viewMatrix,
                                               const glm::mat4& t_projectionMatrix) const
{
    OpenGL::EnableAlphaBlending();

    m_vao->Bind();
    m_shaderProgram->Bind();

    m_shaderProgram->SetUniform("model", t_modelMatrix);
    m_shaderProgram->SetUniform("view", t_viewMatrix);
    m_shaderProgram->SetUniform("projection", t_projectionMatrix);

    m_vao->DrawPrimitives();

    m_shaderProgram->Unbind();
    m_vao->Unbind();

    OpenGL::DisableBlending();
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::renderer::SpriteRenderer::CleanUp()
{

}
