#pragma once

#include <cstdint>

namespace sg::ogl::buffer
{
    class Ebo
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The handle of the Ebo.
         */
        uint32_t id{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Ebo();

        Ebo(const Ebo& t_other) = delete;
        Ebo(Ebo&& t_other) noexcept = delete;
        Ebo& operator=(const Ebo& t_other) = delete;
        Ebo& operator=(Ebo&& t_other) noexcept = delete;

        ~Ebo() noexcept;

        //-------------------------------------------------
        // Bind
        //-------------------------------------------------

        void Bind() const;

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
