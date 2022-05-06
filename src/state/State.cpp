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

#include "State.h"
#include "StateStack.h"
#include "SgAssert.h"
#include "ogl/Window.h"
#include "ogl/OpenGL.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::state::State::State(const Id t_id, StateStack* t_stateStack, Context t_context)
    : context{ std::move(t_context) }
    , m_id{ t_id }
    , m_stateStack{ t_stateStack }
{
    SG_ASSERT(t_stateStack, "[State::State()] Null pointer")

    Log::SG_LOG_DEBUG("[State::State()] Create State.");
}

sg::state::State::~State() noexcept
{
    Log::SG_LOG_DEBUG("[State::~State()] Destruct State.");
}

//-------------------------------------------------
// Frame
//-------------------------------------------------

void sg::state::State::StartFrame()
{
    ogl::OpenGL::SetClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    ogl::OpenGL::Clear();
}

void sg::state::State::EndFrame() const
{
    context.window->SwapBuffersAndCallEvents();
}

//-------------------------------------------------
// Stack operations
//-------------------------------------------------

void sg::state::State::RequestStackPush(const Id t_id) const
{
    m_stateStack->PushState(t_id);
}

void sg::state::State::RequestStackPop() const
{
    m_stateStack->PopState(m_id);
}

void sg::state::State::RequestStateClear() const
{
    m_stateStack->ClearStates();
}
