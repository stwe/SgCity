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

#include "Log.h"
#include "Application.h"
#include "SgException.h"

//-------------------------------------------------
// Main
//-------------------------------------------------

int main()
{
    sg::Log::Init();

    sg::Log::SG_LOG_DEBUG("[main()] Starting main.");
    sg::Log::SG_LOG_DEBUG("[main()] Logger was initialized.");

    try
    {
        sg::Application application;
        application.Run();

        return EXIT_SUCCESS;
    }
    catch (const sg::SgException& e)
    {
        sg::Log::SG_LOG_ERROR("SgException {}", e.what());
    }
    catch (const std::exception& e)
    {
        sg::Log::SG_LOG_ERROR("Standard Exception: {}", e.what());
    }
    catch ( ... )
    {
        sg::Log::SG_LOG_ERROR("Unknown Exception. No details available.");
    }

    return EXIT_FAILURE;
}
