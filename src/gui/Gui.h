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

#include <memory>

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::ogl::buffer
{
    class Vao;
}

//-------------------------------------------------
// Gui
//-------------------------------------------------

namespace sg::gui
{
    /**
     * Represents a Gui.
     */
    class Gui
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Gui();

        Gui(const Gui& t_other) = delete;
        Gui(Gui&& t_other) noexcept = delete;
        Gui& operator=(const Gui& t_other) = delete;
        Gui& operator=(Gui&& t_other) noexcept = delete;

        ~Gui() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Render(float t_x, float t_y, float t_scaleX, float t_scaleY, uint32_t t_id) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::unique_ptr<ogl::buffer::Vao> m_vao;
    };
}
