#version 330 core

layout(location = 0) in vec3 aPos;       // Vertex position
layout(location = 1) in vec3 aNormal;    // Vertex normal (not used but kept for compatibility)
layout(location = 2) in vec2 aTexCoords; // Texture coordinates

out vec2 TexCoords;                     // Pass texture coordinates to fragment shader
uniform mat4 MVP;                       // Model-View-Projection matrix

void main()
{
    // Pass the texture coordinates to the fragment shader
    TexCoords = aTexCoords;

    // Transform vertex position using the MVP matrix
    gl_Position = MVP * vec4(aPos, 1.0);
}
