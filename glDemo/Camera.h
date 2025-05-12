#pragma once
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include "core.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

class cTransform;
class Light;
class Scene;

// Base class for a camera
class Camera
{
public:
    Camera();
    virtual ~Camera();

    // Initialize the camera with screen dimensions and scene reference
    virtual void Init(float _w, float _h, Scene* _scene);

    // Update the camera (called every frame)
    virtual void Tick(float _dt);

    // Load camera info from the manifest
    virtual void Load(ifstream& _file);

    // Getters
    string GetType() { return m_type; }
    glm::mat4 GetProj() { return m_projectionMatrix; }
    glm::mat4 GetView() { return m_viewMatrix; }
    glm::vec3 GetPos() { return m_pos; }
    float GetFOV() { return m_fov; }
    float GetNear() { return m_near; }
    float GetFar() { return m_far; }
    string GetName() { return m_name; }
    void SetName(string _name) { m_name = _name; }

    // Movement methods
    void MoveForward(float deltaTime);
    void MoveBackward(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveRight(float deltaTime);

    // Rotation method
    void Rotate(float xOffset, float yOffset);

    // Set the camera's look-at position
    vec3 GetLookAt() { return m_lookAt; }
    void SetLookAt(vec3 _pos) { m_lookAt = _pos; UpdateViewMatrix(); }

    // Set up shader values for rendering
    virtual void SetRenderValues(unsigned int _prog);

protected:
    // Update the view matrix based on position and orientation
    void UpdateViewMatrix();

    // Standard transforms needed for rendering
    glm::mat4 m_projectionMatrix;  // Projection matrix
    glm::mat4 m_viewMatrix;        // View matrix

    // Camera position and orientation
    glm::vec3 m_pos;               // Camera position
    glm::vec3 m_lookAt;            // Look-at position

    // Camera properties
    float m_fov;                   // Field of view
    float m_near;                  // Near plane distance
    float m_far;                   // Far plane distance

    // Movement and rotation properties
    float movementSpeed = 5.0f;    // Movement speed
    float sensitivity = 0.1f;      // Mouse sensitivity
    float yaw = -90.0f;            // Yaw angle (rotation around Y-axis)
    float pitch = 0.0f;            // Pitch angle (rotation around X-axis)

    // Metadata
    string m_name;
    string m_type;
};
