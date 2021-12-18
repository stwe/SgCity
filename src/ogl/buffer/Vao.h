#pragma once

#include <cstdint>
#include <memory>
#include "Vbo.h"

namespace sg::ogl::buffer
{
    /**
     * Represents a Vertex Array Object.
     */
    class Vao
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The handle of the Vao.
         */
        uint32_t id{ 0 };

        /**
         * Number of vertices to draw.
         */
        int32_t drawCount{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Vao();

        Vao(const Vao& t_other) = delete;
        Vao(Vao&& t_other) noexcept = delete;
        Vao& operator=(const Vao& t_other) = delete;
        Vao& operator=(Vao&& t_other) noexcept = delete;

        ~Vao() noexcept;

        //-------------------------------------------------
        // Getter
        //-------------------------------------------------

        Vbo& GetVbo() { return *m_vbo; }

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Create Vbo
        //-------------------------------------------------

        /**
         * Binds this Vao and creates an empty dynamic Vbo.
         * Allocate memory and *not* fill it.
         *
         * @param t_size Specifies the size in bytes of the buffer object's new data store.
         * @param t_drawCount Sets the number of vertices to be render if the value is greater than zero.
         */
        void CreateEmptyDynamicVbo(uint32_t t_size, int32_t t_drawCount = 0);

        //-------------------------------------------------
        // Draw
        //-------------------------------------------------

        void DrawPrimitives(uint32_t t_drawMode, int32_t t_first) const;
        void DrawPrimitives(uint32_t t_drawMode) const;
        void DrawPrimitives() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * A Vbo object.
         */
        std::unique_ptr<Vbo> m_vbo;

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
