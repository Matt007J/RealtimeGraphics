#include "Camera.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include "stringHelp.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
    : movementSpeed(5.0f), sensitivity(0.1f), yaw(-90.0f), pitch(0.0f)
{
    m_type = "CAMERA";
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - Initialize the camera
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
    float aspect_ratio = _screenWidth / _screenHeight;
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), aspect_ratio, m_near, m_far);
    UpdateViewMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////
// Tick() - Update the camera's view matrix
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Tick(float _dt)
{
    UpdateViewMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////
// Load() - Load camera properties from a file
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Load(ifstream& _file)
{
    StringHelp::String(_file, "NAME", m_name);
    StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
    StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
    StringHelp::Float(_file, "FOV", m_fov);
    StringHelp::Float(_file, "NEAR", m_near);
    StringHelp::Float(_file, "FAR", m_far);
}

/////////////////////////////////////////////////////////////////////////////////////
// SetRenderValues() - Set the camera's uniform values in the shader
/////////////////////////////////////////////////////////////////////////////////////
void Camera::SetRenderValues(unsigned int _prog)
{
    GLint loc;

    // Matrix for the view transform
    if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetView()));

    // Matrix for the projection transform
    if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetProj()));

    // The current camera position
    if (Helper::SetUniformLocation(_prog, "camPos", &loc))
        glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
}

/////////////////////////////////////////////////////////////////////////////////////
// Movement Methods
/////////////////////////////////////////////////////////////////////////////////////
void Camera::MoveForward(float deltaTime)
{
    glm::vec3 forward = glm::normalize(m_lookAt - m_pos);
    m_pos += forward * movementSpeed * deltaTime;
    m_lookAt += forward * movementSpeed * deltaTime;
    UpdateViewMatrix();
}

void Camera::MoveBackward(float deltaTime)
{
    glm::vec3 backward = glm::normalize(m_pos - m_lookAt);
    m_pos += backward * movementSpeed * deltaTime;
    m_lookAt += backward * movementSpeed * deltaTime;
    UpdateViewMatrix();
}

void Camera::MoveLeft(float deltaTime)
{
    glm::vec3 forward = glm::normalize(m_lookAt - m_pos);
    glm::vec3 left = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));
    m_pos += left * movementSpeed * deltaTime;
    m_lookAt += left * movementSpeed * deltaTime;
    UpdateViewMatrix();
}

void Camera::MoveRight(float deltaTime)
{
    glm::vec3 forward = glm::normalize(m_lookAt - m_pos);
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_pos += right * movementSpeed * deltaTime;
    m_lookAt += right * movementSpeed * deltaTime;
    UpdateViewMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////
// Rotation Method
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Rotate(float xOffset, float yOffset)
{
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_lookAt = m_pos + glm::normalize(direction);

    UpdateViewMatrix();
}

/////////////////////////////////////////////////////////////////////////////////////
// UpdateViewMatrix() - Update the view matrix based on the camera's position and orientation
/////////////////////////////////////////////////////////////////////////////////////
void Camera::UpdateViewMatrix()
{
    m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
}
