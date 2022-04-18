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

#include "ResourceManager.h"
#include "Model.h"
#include "ogl/Window.h"

//-------------------------------------------------
// Shaders
//-------------------------------------------------

sg::ogl::resource::ShaderProgram& sg::ogl::resource::ResourceManager::LoadShaderProgram(const std::string& t_path)
{
    if (shaderPrograms.count(t_path) == 0)
    {
        shaderPrograms.emplace(t_path, std::make_unique<ShaderProgram>(t_path));
    }

    return *shaderPrograms.at(t_path);
}

//-------------------------------------------------
// Models
//-------------------------------------------------

std::shared_ptr<sg::ogl::resource::Model> sg::ogl::resource::ResourceManager::LoadModel(
    std::shared_ptr<Window> t_window,
    const std::string& t_path,
    unsigned int t_pFlags
)
{
    if (models.count(t_path) == 0)
    {
        models.emplace(t_path, std::make_unique<Model>(t_window, t_path, t_pFlags));
    }

    return models.at(t_path);
}
