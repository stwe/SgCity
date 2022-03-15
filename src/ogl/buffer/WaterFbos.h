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
// WaterFbos
//-------------------------------------------------

namespace sg::ogl::buffer
{
    /**
     * Represents the Water Framebuffer objects.
     */
    class WaterFbos
    {
    public:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        // geometry which is above the water surface
        int32_t reflectionWidth{ 0 };
        int32_t reflectionHeight{ 0 };
        uint32_t reflectionFboId{ 0 };
        uint32_t reflectionColorTextureId{ 0 };
        uint32_t reflectionDepthRenderBufferId{ 0 }; // write-only

        // geometry which is under the water surface
        int32_t refractionWidth{ 0 };
        int32_t refractionHeight{ 0 };
        uint32_t refractionFboId{ 0 };
        uint32_t refractionColorTextureId{ 0 };
        uint32_t refractionDepthTextureId{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        WaterFbos() = delete;
        WaterFbos(int32_t t_width, int32_t t_height);

        WaterFbos(const WaterFbos& t_other) = delete;
        WaterFbos(WaterFbos&& t_other) noexcept = delete;
        WaterFbos& operator=(const WaterFbos& t_other) = delete;
        WaterFbos& operator=(WaterFbos&& t_other) noexcept = delete;

        ~WaterFbos() noexcept;

        //-------------------------------------------------
        // Fbo
        //-------------------------------------------------

        static uint32_t GenerateAndBindFbo();
        static void BindFbo(uint32_t t_fboId);
        static void UnbindFbo();

        //-------------------------------------------------
        // Fbo - Render Target
        //-------------------------------------------------

        static void BindAsRenderTarget(uint32_t t_fbo, int32_t t_width, int32_t t_height);

        void BindReflectionFboAsRenderTarget() const;
        void BindRefractionFboAsRenderTarget() const;

        void UnbindRenderTarget() const;

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        int32_t m_width{ 0 };
        int32_t m_height{ 0 };

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void InitReflectionFbo();
        void InitRefractionFbo();

        static uint32_t CreateColorTextureAttachment(int32_t t_width, int32_t t_height);
        static uint32_t CreateDepthTextureAttachment(int32_t t_width, int32_t t_height);
        static uint32_t CreateDepthRenderBufferAttachment(int32_t t_width, int32_t t_height);

        static void CheckFbo();

        //-------------------------------------------------
        // CleanUp
        //-------------------------------------------------

        void CleanUp() const;
    };
}
