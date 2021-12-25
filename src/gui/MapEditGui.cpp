#include <imgui.h>
#include "MapEditGui.h"
#include "ogl/input/MouseInput.h"

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
        raise = true;
    }

    if (ImGui::Button("Lower terrain"))
    {
        raise = false;
    }

    ImGui::End();
}
