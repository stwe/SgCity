#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>

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
        // Store data by calling glBufferData
        //-------------------------------------------------

        void StoreMat4(const std::vector<glm::mat4>& t_matrices, uint32_t t_usage) const;
        void StoreMat4(const std::vector<glm::mat4>& t_matrices) const;

        void StoreFloat(const std::vector<float>& t_floats, uint32_t t_usage) const;
        void StoreFloat(const std::vector<float>& t_floats) const;

        //-------------------------------------------------
        // Attributes
        //-------------------------------------------------

        void AddFloatAttribute(
            uint32_t t_index,
            int32_t t_nrOfFloatComponents,
            int32_t t_nrOfAllFloats,
            uint64_t t_startPoint,
            bool t_instancedRendering
        ) const;

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
