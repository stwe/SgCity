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

sg::state::StateStack::StateStack(std::shared_ptr<State::Context> t_context)
    : m_context{ std::move(t_context) }
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

void sg::state::StateStack::PushState(const State::Id t_id)
{
    Log::SG_LOG_DEBUG("[StateStack::PushState()] Add pending stack operation PUSH for state {}.", State::STATE_IDS.at(static_cast<int>(t_id)));
    m_pendingChanges.emplace_back(Action::PUSH, t_id);
}

void sg::state::StateStack::PopState(const State::Id t_id)
{
    Log::SG_LOG_DEBUG("[StateStack::PushState()] Add pending stack operation POP for state {}.", State::STATE_IDS.at(static_cast<int>(t_id)));
    m_pendingChanges.emplace_back(Action::POP, t_id);
}

void sg::state::StateStack::ClearStates()
{
    Log::SG_LOG_DEBUG("[StateStack::ClearStates()] Add pending stack operation CLEAR all states.");
    m_pendingChanges.emplace_back(Action::CLEAR, State::Id::ALL);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::state::StateStack::Input()
{
    // iterate from top to bottom
    for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr)
    {
        (*itr)->Input();
    }

    ApplyPendingChanges();
}

void sg::state::StateStack::Update()
{
    // iterate from top to bottom
    for (auto itr{ m_stack.rbegin() }; itr != m_stack.rend(); ++itr)
    {
        (*itr)->Update();
    }

    ApplyPendingChanges();
}

void sg::state::StateStack::Render() const
{
    for (const auto& state : m_stack)
    {
        state->StartFrame();
        state->Render();
        state->RenderImGui();
        state->EndFrame();
    }
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

std::unique_ptr<sg::state::State> sg::state::StateStack::CreateState(const State::Id t_id)
{
    Log::SG_LOG_DEBUG("[StateStack::CreateState()] Running factory function for state {}.", State::STATE_IDS.at(static_cast<int>(t_id)));

    const auto it{ m_factories.find(t_id) };
    SG_ASSERT(it != m_factories.end(), "[StateStack::CreateState()] Factory function not found for state " + std::string(State::STATE_IDS.at(static_cast<int>(t_id))) + ".")

    return it->second();
}

void sg::state::StateStack::ApplyPendingChanges()
{
    for (const auto& change : m_pendingChanges)
    {
        switch (change.action)
        {
            case Action::PUSH:
                // add element at the end
                m_stack.emplace_back(CreateState(change.id));
                Log::SG_LOG_INFO("Stack size is {} after PUSH state {}.", m_stack.size(), State::STATE_IDS.at(static_cast<int>(change.id)));
                break;
            case Action::POP:
                // removes the last element in the vector
                SG_ASSERT(change.id == m_stack.back()->GetId(), "[StateStack::ApplyPendingChanges()] Invalid POP operation.")
                m_stack.pop_back();
                Log::SG_LOG_INFO("Stack size is {} after POP state {}.", m_stack.size(), State::STATE_IDS.at(static_cast<int>(change.id)));
                break;
            case Action::CLEAR:
                m_stack.clear();
                Log::SG_LOG_INFO("Stack size is {} after CLEAR all states.", m_stack.size());
                break;
        }
    }

    m_pendingChanges.clear();
}
