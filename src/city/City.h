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

#include <string>
#include <memory>

//-------------------------------------------------
// Forward declarations
//-------------------------------------------------

namespace sg::map
{
    class Map;
    class Tile;
}

//-------------------------------------------------
// City
//-------------------------------------------------

namespace sg::city
{
    /**
     * Represents a city.
     */
    class City
    {
    public:
        //-------------------------------------------------
        // Constants
        //-------------------------------------------------

        /**
         * The amount of real world time each day should last.
         */
        static constexpr auto TIME_PER_DAY{ 1.0f };

        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The real world time (in seconds) since the day updated.
         */
        float currentTime{ 0.0f };

        /**
         * Proportion of residents who give birth each day.
         */
        float birthRate{ 0.0f };

        /**
         * Proportion of residents who die.
         * death rate = 1 / (life expectancy * 360)
         */
        float deathRate{ 0.0f };

        /**
         * Number of people who do not have a home.
         */
        float homelessPeople{ 0.0f };

        /**
         * Number of people who do not have a job.
         */
        float unemployedPeople{ 0.0f };

        /**
         * Proportion of the population that can work.
         */
        float proportionCanWork{ 0.0f };

        /**
         * The total population.
         */
        float population{ 0.0f };

        /**
         * The total employable people.
         */
        float employable{ 0.0f };

        /**
         * Days already gone.
         */
        int day{ 0 };

        //-------------------------------------------------
        // Ctors. / Dtor.
        //-------------------------------------------------

        City() = delete;

        /**
         * Constructs a new City object.
         *
         * @param t_name The name of the city.
         * @param t_map The Map object for access to the tiles.
         */
        City(std::string t_name, std::shared_ptr<map::Map> t_map);

        City(const City& t_other) = delete;
        City(City&& t_other) noexcept = delete;
        City& operator=(const City& t_other) = delete;
        City& operator=(City&& t_other) noexcept = delete;

        ~City() noexcept;

        //-------------------------------------------------
        // Logic
        //-------------------------------------------------

        void Update();
        void RenderImGui();

    protected:

    private:
        //-------------------------------------------------
        // Member
        //-------------------------------------------------

        /**
         * The name of the city.
         */
        std::string m_name;

        /**
         * The Map object.
         */
        std::shared_ptr<map::Map> m_map;

        //-------------------------------------------------
        // Distribute
        //-------------------------------------------------

        /**
         * Moving 4 people into a tile.
         *
         * @param t_from From which is moved.
         * @param t_toTile The tile into people are moved.
         * @param t_rate A birth rate if it's positive and a death rate if it's negative.
         */
        float Distribute(float& t_from, const std::shared_ptr<map::Tile>& t_toTile, float t_rate = 0.0f);

        //-------------------------------------------------
        // Init
        //-------------------------------------------------

        void Init();
    };
}
