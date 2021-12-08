#include "Camera.h"
#include "Log.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::camera::Camera::Camera(const glm::vec3& t_position, const float t_yaw, const float t_pitch)
    : m_position{ t_position }
    , m_yaw{ t_yaw }
    , m_pitch{ t_pitch }
{
    Log::SG_LOG_DEBUG("[Camera::Camera()] Create Camera.");

    Update();
}

sg::ogl::camera::Camera::Camera(const glm::vec3& t_position)
    : m_position{ t_position }
{
    Log::SG_LOG_DEBUG("[Camera::Camera()] Create Camera.");

    Update();
}

sg::ogl::camera::Camera::~Camera() noexcept
{
    Log::SG_LOG_DEBUG("[Camera::~Camera()] Destruct Camera.");
}

//-------------------------------------------------
// Update
//-------------------------------------------------

void sg::ogl::camera::Camera::Update()
{
    // Calculate the new Front vector.
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the Right and Up vector.
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void sg::ogl::camera::Camera::ProcessKeyboard(const camera::Camera::Direction t_direction)
{
    if (t_direction == Direction::FORWARD)
        m_position += m_front * m_movementSpeed;
    if (t_direction == Direction::BACKWARD)
        m_position -= m_front * m_movementSpeed;
    if (t_direction == Direction::LEFT)
        m_position -= m_right * m_movementSpeed;
    if (t_direction == Direction::RIGHT)
        m_position += m_right * m_movementSpeed;
    if (t_direction == Direction::UP)
        m_position += m_up * m_movementSpeed;
    if (t_direction == Direction::DOWN)
        m_position -= m_up * m_movementSpeed;
}

void sg::ogl::camera::Camera::ProcessMouse(const glm::vec2& t_displVec)
{
    m_yaw += t_displVec.y * m_mouseSensitivity;
    m_pitch -= t_displVec.x * m_mouseSensitivity;

    if (m_yaw > 359.0f)
    {
        m_yaw = 359.0f;
    }
    else if (m_yaw < -359.0f)
    {
        m_yaw = -359.0f;
    }

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles.
    Update();
}
