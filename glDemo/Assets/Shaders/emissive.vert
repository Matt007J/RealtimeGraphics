#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    fragNormal = mat3(transpose(inverse(modelMatrix))) * normal;
    fragTexCoord = texCoord;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
