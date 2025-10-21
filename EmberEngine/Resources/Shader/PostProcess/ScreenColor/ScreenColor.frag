#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    vec4 SceneColor = texture(screenTexture, TexCoords);

    FragColor = SceneColor;
}