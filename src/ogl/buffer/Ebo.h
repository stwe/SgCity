// This file is part of the SgCity project.
//
// Copyright (c) 2022. stwe <https://github.com/stwe/SgCity>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#pragma once

#include <cstdint>

//-------------------------------------------------
// Ebo
//-------------------------------------------------

namespace sg::ogl::buffer
{
    /**
     * Represents an Indexbuffer.
     */
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
        // Bind / unbind
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

        void CleanUp() const;
    };
}
