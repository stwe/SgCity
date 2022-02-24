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
#include <string>

//-------------------------------------------------
// Texture
//-------------------------------------------------

namespace sg::ogl::resource
{
    class Texture
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        uint32_t id{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        Texture() = delete;
        Texture(std::string t_path, bool t_loadVerticalFlipped);
        explicit Texture(std::string t_path);

        Texture(const Texture& t_other) = delete;
        Texture(Texture&& t_other) noexcept = delete;
        Texture& operator=(const Texture& t_other) = delete;
        Texture& operator=(Texture&& t_other) noexcept = delete;

        ~Texture() noexcept;

        //-------------------------------------------------
        // Bind / unbind
        //-------------------------------------------------

        void Bind() const;
        static void Unbind();
        void BindForReading(uint32_t t_textureUnit) const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        std::string m_path;
        bool m_loadVerticalFlipped{ false };
        int m_width{ 0 };
        int m_height{ 0 };
        int m_format{ 0 };
        int m_channels{ 0 };

        //-------------------------------------------------
        // Create
        //-------------------------------------------------

        void CreateId();

        //-------------------------------------------------
        // Helper
        //-------------------------------------------------

        void LoadFromFile();

        //-------------------------------------------------
        // Clean up
        //-------------------------------------------------

        void CleanUp() const;
    };
}
