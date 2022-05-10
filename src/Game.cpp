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

#include "Game.h"
#include "MenuState.h"
#include "StartState.h"
#include "CityState.h"
#include "state/StateStack.h"
#include "ogl/Window.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::Game::Game()
{
    Log::SG_LOG_DEBUG("[Game::Game()] Create Game.");
}

sg::Game::~Game() noexcept
{
    Log::SG_LOG_DEBUG("[Game::~Game()] Destruct Game.");
}

//-------------------------------------------------
// Run
//-------------------------------------------------

void sg::Game::Run()
{
    Log::SG_LOG_DEBUG("[Game::Run()] Starts running game.");

    Init();
    GameLoop();
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::Game::Init()
{
    Log::SG_LOG_DEBUG("[Game::Init()] Initializing game.");

    // create window
    m_window = std::make_shared<ogl::Window>();

    // create state stack
    m_stateStack = std::make_unique<state::StateStack>(std::make_unique<state::State::Context>(m_window));

    // register states
    m_stateStack->RegisterState<MenuState>(state::State::Id::MAIN_MENU);
    m_stateStack->RegisterState<StartState>(state::State::Id::START);
    m_stateStack->RegisterState<CityState>(state::State::Id::CITY);

    // start with the main menu
    m_stateStack->PushState(state::State::Id::MAIN_MENU);

    Log::SG_LOG_DEBUG("[Game::Init()] The game was successfully initialized.");
}

void sg::Game::Input()
{
    m_stateStack->Input();
}

void sg::Game::Update()
{
    m_stateStack->Update();
}

void sg::Game::Render()
{
    m_stateStack->Render();
}

//-------------------------------------------------
// Game loop
//-------------------------------------------------

void sg::Game::GameLoop()
{
    Log::SG_LOG_DEBUG("[Game::GameLoop()] Starting the game loop.");

    auto lastTime{ glfwGetTime() };
    auto resetTimer{ lastTime };

    auto dt{ 0.0 };
    auto fps{ 0 };
    auto updates{ 0 };

    while (!m_window->WindowShouldClose())
    {
        const auto now{ glfwGetTime() };
        dt += (now - lastTime) / FRAME_TIME;
        lastTime = now;

        Input();

        while (dt >= 1.0)
        {
            Update();
            updates++;
            dt--;
        }

        if (m_stateStack->IsEmpty())
        {
            m_window->Close();
        }

        Render();
        fps++;

        if (glfwGetTime() - resetTimer > 1.0)
        {
            resetTimer++;

            std::stringstream ss;
#ifdef SG_CITY_DEBUG_BUILD
            ss << m_window->GetTitle() << " [DEBUG BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#else
            ss << m_window.GetTitle() << " [RELEASE BUILD] " << "   |   Fps: " << fps << "   |   Updates: " << updates;
#endif
            glfwSetWindowTitle(m_window->GetWindowHandle(), ss.str().c_str());

            updates = 0;
            fps = 0;
        }
    }

    Log::SG_LOG_DEBUG("[Game::GameLoop()] The game loop has ended.");
}
