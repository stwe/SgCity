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

#include "StateStack.h"
#include "SgAssert.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::state::StateStack::StateStack(const sg::state::State::Context t_context)
    : m_context{ t_context }
{
    Log::SG_LOG_DEBUG("[StateStack::StateStack()] Create StateStack.");
}

sg::state::StateStack::~StateStack() noexcept
{
    Log::SG_LOG_DEBUG("[StateStack::~StateStack()] Destruct StateStack.");
}

//-------------------------------------------------
// Stack operations
//-------------------------------------------------

void sg::state::StateStack::PushState(const sg::state::State::Id t_id)
{
    m_pendingChanges.emplace_back(Action::PUSH, t_id);
}

void sg::state::StateStack::PopState()
{
    m_pendingChanges.emplace_back(Action::POP);
}

void sg::state::StateStack::ClearStates()
{
    m_pendingChanges.emplace_back(Action::CLEAR);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::state::StateStack::Update()
{
    // iterate from top to bottom
    for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr)
    {
        (*itr)->Update();
    }

    ApplyPendingChanges();
}

void sg::state::StateStack::Render()
{
    for (const auto& state : m_stack)
    {
        state->Render();
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::unique_ptr<sg::state::State> sg::state::StateStack::CreateState(const sg::state::State::Id t_id)
{
    const auto it{ m_factories.find(t_id) };
    SG_ASSERT(it != m_factories.end(), "[StateStack::CreateState()] Factory function not found.")

    return it->second();
}

void sg::state::StateStack::ApplyPendingChanges()
{
    for (const auto& change : m_pendingChanges)
    {
        switch (change.action)
        {
            case Action::PUSH:
                m_stack.emplace_back(CreateState(change.id));
                break;
            case Action::POP:
                m_stack.pop_back();
                break;
            case Action::CLEAR:
                m_stack.clear();
                break;
        }
    }

    m_pendingChanges.clear();
}
