#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;


in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	// calculate lambertian (l)
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l = dot(N, DIRDir);

	// Calculate diffuse brightness / colour for fragment
	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * DIRCol * l;

	// Set the alpha value for transparency (e.g., 0.5 for 50% transparency)
	float alpha = 0.5;

	// Combine ambient and diffuse components with transparency
	fragColour = vec4(DIRAmb, 0.01) + vec4(diffuseColour, alpha);
}

