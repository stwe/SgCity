#pragma once

#include <cstdint>

namespace sg::ogl::buffer
{
    /**
     * Represents a Vertex Buffer Object.
     */
    class Vbo
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The handle of the Vbo.
         */
        uint32_t id{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Vbo();

        Vbo(const Vbo& t_other) = delete;
        Vbo(Vbo&& t_other) noexcept = delete;
        Vbo& operator=(const Vbo& t_other) = delete;
        Vbo& operator=(Vbo&& t_other) noexcept = delete;

        ~Vbo() noexcept;

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Attributes
        //-------------------------------------------------

        /**
         * Specified how OpenGL should interpret the vertex data.
         *
         * @param t_index The location of the vertex attribute.
         * @param t_nrOfFloatComponents The size of the vertex attribute.
         * @param t_nrOfAllFloats The space between consecutive vertex attributes.
         * @param t_startPoint The offset of where the position data begins in the buffer.
         */
        void AddFloatAttribute(
            uint32_t t_index,
            int32_t t_nrOfFloatComponents,
            int32_t t_nrOfAllFloats,
            uint64_t t_startPoint
        ) const;

    protected:

    private:
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
