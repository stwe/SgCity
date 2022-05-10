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

#include <algorithm>
#include <imgui.h>
#include "City.h"
#include "Application.h"
#include "SgAssert.h"
#include "map/Map.h"
#include "map/TerrainLayer.h"
#include "map/Tile.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::city::City::City(std::string t_name, std::shared_ptr<map::Map> t_map)
    : name{ std::move(t_name) }
    , m_map{ std::move(t_map) }
{
    Log::SG_LOG_DEBUG("[City::City()] Create City.");

    Init();
}

sg::city::City::~City() noexcept
{
    Log::SG_LOG_DEBUG("[City::~City()] Destruct City.");
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::city::City::Update()
{
    auto populationTotal{ 0.0f };

    currentTime += static_cast<float>(Application::FRAME_TIME);
    if (currentTime < TIME_PER_DAY)
    {
        return;
    }

    currentTime = 0.0f;
    day++;

    if (day % 30 == 0)
    {
        // set funds
    }

    for (auto& tile : m_map->terrainLayer->tiles)
    {
        if (tile->type == map::Tile::TileType::RESIDENTIAL)
        {
            Distribute(homelessPeople, tile, birthRate - deathRate);
            populationTotal += tile->curResidentsOrEmployees;
        }

        if (tile->type == map::Tile::TileType::COMMERCIAL)
        {
            Distribute(unemployedPeople, tile, 0.0f);
        }

        if (tile->type == map::Tile::TileType::INDUSTRIAL)
        {
            Distribute(unemployedPeople, tile, 0.0f);
        }

        // todo: tile update -> change variant
    }

    // adjust the number of homeless people for births and deaths
    homelessPeople += homelessPeople * (birthRate - deathRate);
    populationTotal += homelessPeople;

    // adjust the number of unemployed people for births and deaths
    auto newWorkers{ (populationTotal - population) * proportionCanWork };
    newWorkers *= newWorkers < 0 ? -1 : 1;
    unemployedPeople += newWorkers;
    employable += newWorkers;

    unemployedPeople = std::max(unemployedPeople, 0.0f);
    employable = std::max(employable, 0.0f);

    // update city population
    population = populationTotal;
}

void sg::city::City::RenderImGui()
{
    ImGui::Begin("City");

    ImGui::Text("Name: %s", name.c_str());
    ImGui::Text("Day: %d", day);
    ImGui::Text("Population: %d / Homeless: %d", static_cast<int>(population), static_cast<int>(homelessPeople));
    ImGui::Text("Employable: %d / Unemployed: %d", static_cast<int>(employable), static_cast<int>(unemployedPeople));
    ImGui::End();
}

//-------------------------------------------------
// Distribute
//-------------------------------------------------

float sg::city::City::Distribute(float& t_from, const std::shared_ptr<map::Tile>& t_toTile, const float t_rate)
{
    constexpr auto moveRate{ 4 };

    // when there are homeless or unemployed
    if (t_from > 0)
    {
        auto movingToTile{ t_toTile->maxResidentsOrEmployees - static_cast<int>(t_toTile->curResidentsOrEmployees) };

        // limit movingToTile to value of moveRate
        movingToTile = std::min(movingToTile, moveRate);
        SG_ASSERT(movingToTile >= 0 && movingToTile <= moveRate, "[City::Distribute()] Invalid moving value")

        // limit movingToTile to value of t_from
        if (static_cast<int>(t_from) - movingToTile < 0)
        {
            movingToTile = static_cast<int>(t_from);
            SG_ASSERT(movingToTile >= 0 && movingToTile <= moveRate, "[City::Distribute()] Invalid moving value")
        }

        // and now the homeless/unemployed go ...
        t_from -= static_cast<float>(movingToTile);

        // ... into the tile
        t_toTile->curResidentsOrEmployees += static_cast<float>(movingToTile);
    }

    // adjust the tile population for births and deaths
    // only affects residential tiles if non-zero rate is passed
    t_toTile->curResidentsOrEmployees += t_toTile->curResidentsOrEmployees * t_rate;

    // move residents that cannot be sustained by the tile into the pool
    const auto maxRes{ static_cast<float>(t_toTile->maxResidentsOrEmployees) };
    if (t_toTile->curResidentsOrEmployees > maxRes)
    {
        // should only affects residential tiles
        SG_ASSERT(t_toTile->type == map::Tile::TileType::RESIDENTIAL, "[City::Distribute()] Invalid tile type")

        // new homeless people
        t_from += t_toTile->curResidentsOrEmployees - maxRes;

        // limit tile population to tile max population
        t_toTile->curResidentsOrEmployees = maxRes;
    }

    return t_toTile->curResidentsOrEmployees;
}

//-------------------------------------------------
// Init
//-------------------------------------------------

void sg::city::City::Init()
{
    Log::SG_LOG_DEBUG("[City::Init()] Initializing city.");

    birthRate = Application::INI.Get<float>("city", "birth_rate");
    deathRate = Application::INI.Get<float>("city", "death_rate");
    homelessPeople = Application::INI.Get<float>("city", "homeless_people");
    proportionCanWork = Application::INI.Get<float>("city", "proportion_population_can_work");

    // at the beginning all residents are homeless
    population = homelessPeople;

    // indicates how many of the homeless are able to work
    employable = homelessPeople * proportionCanWork;

    // at the beginning all employable homeless people are also unemployed
    unemployedPeople = employable;

    Log::SG_LOG_DEBUG("[City::Init()] The city was successfully initialized.");
}
