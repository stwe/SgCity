#include <imgui.h>
#include "MapEditGui.h"
#include "Log.h"
#include "ogl/input/MouseInput.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::gui::MapEditGui::Render(map::Map& t_map)
{
    ImGui::Begin("Game Debug");
    ImGui::Text("Press the Escape key to exit.");
    ImGui::Separator();
    ImGui::Text("Mouse x: %d", ogl::input::MouseInput::GetInstance().GetX());
    ImGui::Text("Mouse y: %d", ogl::input::MouseInput::GetInstance().GetY());

    ImGui::Separator();

    for (auto i{ 0 }; i < 4; ++i)
    {
        if (m_buttons[i])
        {
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.6f, 0.6f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.7f, 0.7f)));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(7.0f, 0.8f, 0.8f)));
            ImGui::Button(m_buttonNames[i].c_str());
            if (ImGui::IsItemClicked(0))
            {
                m_buttons[i] = !m_buttons[i];
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }
        else
        {
            if (ImGui::Button(m_buttonNames[i].c_str()))
            {
                std::fill(m_buttons.begin(), m_buttons.end(), false);
                m_buttons[i] = true;
                action = static_cast<Action>(i);
                Log::SG_LOG_DEBUG("[MapEditGui::Render()] Action: {}", m_buttonNames[i]);
            }
        }
    }

    ImGui::End();
}
