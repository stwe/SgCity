#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <ogl/resource/Texture.h>

namespace sg::ogl::buffer
{
    class Vao;
}

namespace sg::ogl::resource
{
    class ShaderProgram;
}

namespace sg::ogl::renderer
{
    class SpriteRenderer
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        SpriteRenderer();

        SpriteRenderer(const SpriteRenderer& t_other) = delete;
        SpriteRenderer(SpriteRenderer&& t_other) noexcept = delete;
        SpriteRenderer& operator=(const SpriteRenderer& t_other) = delete;
        SpriteRenderer& operator=(SpriteRenderer&& t_other) noexcept = delete;

        ~SpriteRenderer() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Init();
        void Render(
            const glm::mat4& t_modelMatrix,
            const glm::mat4& t_viewMatrix,
            const glm::mat4& t_projectionMatrix,
            const resource::Texture& t_texture
            ) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::unique_ptr<buffer::Vao> m_vao;
        std::unique_ptr<resource::ShaderProgram> m_shaderProgram;

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
