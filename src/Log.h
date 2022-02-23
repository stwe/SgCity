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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//-------------------------------------------------
// Log
//-------------------------------------------------

namespace sg
{
    class Log
    {
    public:
        static void Init()
        {
            spdlog::set_pattern("%^[%T] %n: %v%$");

            m_logger = spdlog::stdout_color_mt("SgCity");

#ifdef SG_CITY_DEBUG_BUILD
            m_logger->set_level(spdlog::level::trace);
#else
            m_logger->set_level(spdlog::level::info);
#endif
        }

        template <typename ...T>
        static void SG_LOG_TRACE(T ...t_args)
        {
            m_logger->trace(t_args...);
        }

        template <typename ...T>
        static void SG_LOG_DEBUG(T ...t_args)
        {
            m_logger->debug(t_args...);
        }

        template <typename ...T>
        static void SG_LOG_INFO(T ...t_args)
        {
            m_logger->info(t_args...);
        }

        template <typename ...T>
        static void SG_LOG_WARN(T ...t_args)
        {
            m_logger->warn(t_args...);
        }

        template <typename ...T>
        static void SG_LOG_ERROR(T ...t_args)
        {
            m_logger->error(t_args...);
        }

        template <typename ...T>
        static void SG_LOG_FATAL(T ...t_args)
        {
            m_logger->critical(t_args...);
        }

    protected:

    private:
        inline static std::shared_ptr<spdlog::logger> m_logger;
    };
}
