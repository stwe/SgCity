#pragma once

#include <memory>
#include <vector>
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
         * Bufferlayout:
         * -------------
         * location 0 (position) 3 floats
         * location 1 (uv)       2 floats
         * location 2 (idColor)  3 floats
         * location 3 (normal)   3 floats
         *
         * @param t_size Specifies the size in bytes of the buffer object's new data store.
         * @param t_drawCount Sets the number of vertices to be render if the value is greater than zero.
         */
        void CreateEmptyDynamicVbo(uint32_t t_size, int32_t t_drawCount = 0);

        /**
         * Binds this Vao and creates a static Vbo. The given vertices data are copied into the Vbo.
         *
         * Bufferlayout:
         * -------------
         * location 0 (position) 3 floats
         * location 1 (uv)       2 floats
         *
         * @param t_vertices The vertices to copy.
         * @param t_drawCount Sets the number of vertices to be render if the value is greater than zero.
         */
        void CreateStaticVbo(const std::vector<float>& t_vertices, int32_t t_drawCount = 0);

        /**
         * Binds this Vao and creates a static Vbo.
         *
         * Bufferlayout
         * ------------
         * location 0 (position) 2 floats
         */
        void CreateStaticWaterVbo();

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
