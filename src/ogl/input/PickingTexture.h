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
#include <vector>

namespace sg::ogl::input
{
    class PickingTexture
    {
    public:
        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        PickingTexture();

        PickingTexture(const PickingTexture& t_other) = delete;
        PickingTexture(PickingTexture&& t_other) noexcept = delete;
        PickingTexture& operator=(const PickingTexture& t_other) = delete;
        PickingTexture& operator=(PickingTexture&& t_other) noexcept = delete;

        ~PickingTexture() noexcept;

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init(int t_width, int t_height);

        //-------------------------------------------------
        // Write
        //-------------------------------------------------

        void EnableWriting() const;
        static void DisableWriting();

        //-------------------------------------------------
        // Read
        //-------------------------------------------------

        [[nodiscard]] int ReadMapIndex(int t_x, int t_y) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The width of the window.
         */
        int m_width{ 0 };

        /**
         * The height of the window.
         */
        int m_height{ 0 };

        /**
         * The handle of the Fbo.
         */
        uint32_t m_fboId{ 0 };

        /**
         * The picking texture handle.
         */
        uint32_t m_pickingTextureId{ 0 };

        /**
         * The depth texture handle.
         */
        uint32_t m_depthTextureId{ 0 };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp();
    };
}
