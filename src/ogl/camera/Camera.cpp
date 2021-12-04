#include <glm/gtc/matrix_transform.hpp>
#include "Log.h"
#include "Camera.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::Camera::Camera(const glm::vec2& t_position)
    : position{ t_position }
{
    Log::SG_LOG_DEBUG("[Camera::Camera()] Create Camera.");
}

sg::ogl::camera::Camera::~Camera() noexcept
{
    Log::SG_LOG_DEBUG("[Camera::~Camera()] Destruct Camera.");
}

//-------------------------------------------------
// Getter
//-------------------------------------------------

glm::mat4 sg::ogl::camera::Camera::GetViewMatrix() const
{
    auto transformMatrix{ glm::mat4(1.0f) };
    transformMatrix = translate(transformMatrix, glm::vec3(position.x, position.y, 0.0f));

    return inverse(transformMatrix);
}

//-------------------------------------------------
// Logic
//-------------------------------------------------

void sg::ogl::camera::Camera::Update()
{
    HandleKeyInput();
    HandleMouseInput();
}

//-------------------------------------------------
// Helper
//-------------------------------------------------

void sg::ogl::camera::Camera::HandleKeyInput()
{

}

void sg::ogl::camera::Camera::HandleMouseInput()
{

}
