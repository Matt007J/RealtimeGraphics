#version 330 core

in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 emissiveColor; // Color of the glow
uniform float emissiveStrength; // Intensity of the glow

out vec4 fragColor;

void main() {
    vec3 emissive = emissiveColor * emissiveStrength;
    fragColor = vec4(emissive, 3.0); // Output the emissive color
}
