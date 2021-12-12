#pragma once

#include <cstdint>
#include <vector>
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
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Add Vbo
        //-------------------------------------------------

        /**
         * Adds a new Vbo object. This Vao is not bound beforehand.
         *
         * @return Vbo
         */
        Vbo& AddVbo();

        /**
         * Binds this Vao and creates an empty Vbo.
         * Allocate memory and not fill it.
         *
         * @param t_size Specifies the size in bytes of the buffer object's new data store.
         * @param t_drawCount The number of vertices to draw.
         *
         * @return Vbo
         */
        Vbo& AddEmptyVbo(uint32_t t_size, int32_t t_drawCount);

        /**
         * Binds this Vao and adds a new Vbo object and copies vertices
         * representing a 2D quad into the buffer's memory.
         */
        void Add2DQuadVbo();

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
         * The Vbo objects assigned to the Vao.
         */
        std::vector<std::unique_ptr<Vbo>> m_vbos;

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
