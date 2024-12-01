#version 310 es

precision mediump float; // Default precision for floats
precision mediump int;   // Default precision for integers

in vec2 TexCoords;        // Interpolated texture coordinates from vertex shader
out vec4 FragColor;       // Output fragment color

uniform sampler2D textureDiffuse; // Texture sampler for the diffuse map

void main()
{
    // Sample the texture color using the texture coordinates
    vec3 color = texture(textureDiffuse, TexCoords).rgb;

    // Set the final fragment color
    FragColor = vec4(color, 1.0);
}
