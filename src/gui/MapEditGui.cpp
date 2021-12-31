#include <imgui.h>
#include "MapEditGui.h"
#include "ogl/input/MouseInput.h"

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::gui::MapEditGui::Render()
{
    ImGui::Begin("Game Debug");
    ImGui::Text("Press the Escape key to exit.");
    ImGui::Separator();
    ImGui::Text("Mouse x: %d", ogl::input::MouseInput::GetInstance().GetX());
    ImGui::Text("Mouse y: %d", ogl::input::MouseInput::GetInstance().GetY());

    ImGui::Separator();

    if (ImGui::Button("Raise terrain"))
    {
        action = Action::RAISE;
    }

    if (ImGui::Button("Lower terrain"))
    {
        action = Action::LOWER;
    }

    if (ImGui::Button("Set Traffic type"))
    {
        action = Action::SET_TRAFFIC;
    }

    ImGui::End();
}
