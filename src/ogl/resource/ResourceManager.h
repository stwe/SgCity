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

#include <map>
#include <memory>
#include <tuple>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Model.h"

//-------------------------------------------------
// ResourceManager
//-------------------------------------------------

namespace sg::ogl::resource
{
    class ResourceManager
    {
    public:

        inline static std::map<std::string, std::unique_ptr<Texture>> textures;
        inline static std::map<std::string, std::unique_ptr<ShaderProgram>> shaderPrograms;
        inline static std::map<std::string, std::unique_ptr<Model>> models;

        template<typename... Args>
        static const Texture& LoadTexture(Args&&... args)
        {
            auto t{ std::make_tuple(args...) };
            if (textures.count(std::get<0>(t)) == 0)
            {
                textures.emplace(std::get<0>(t), std::make_unique<ogl::resource::Texture>(std::forward<Args>(args)...));
            }

            return *textures.at(std::get<0>(t));
        }

        static ShaderProgram& LoadShaderProgram(const std::string& t_path);
        static Model& LoadModel(const std::string& t_path);

    protected:

    private:
        ResourceManager() = default;
    };
}
