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

#include "Log.h"

#if defined(_WIN64) && defined(_MSC_VER)
	#define SG_CITY_DEBUG_BREAK __debugbreak()
#elif defined(__linux__) && defined(__GNUC__) && (__GNUC__ >= 7)
	#include <csignal>
	#define SG_CITY_DEBUG_BREAK raise(SIGTRAP)
#else
	#error Unsupported platform or compiler!
#endif

#ifdef SG_CITY_DEBUG_BUILD
	#define SG_CITY_ENABLE_ASSERTS
#endif

#ifdef SG_CITY_ENABLE_ASSERTS
	#define SG_ASSERT(x, ...) { if(!(x)) { sg::Log::SG_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); SG_CITY_DEBUG_BREAK; } }
#else
	#define SG_ASSERT(x, ...)
#endif
