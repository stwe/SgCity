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
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::state::State::State(std::shared_ptr<StateStack> t_stateStack, const Context t_context)
    : context{ t_context }
    , m_stateStack{ std::move(t_stateStack) }
{
    Log::SG_LOG_DEBUG("[State::State()] Create State.");
}

sg::state::State::~State() noexcept
{
    Log::SG_LOG_DEBUG("[State::~State()] Destruct State.");
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
    m_stateStack->PopState();
}

void sg::state::State::RequestStateClear() const
{
    m_stateStack->ClearStates();
}
