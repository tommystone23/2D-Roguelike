#version 330 core

in vec4 fragment_color;
in vec2 fragment_uv;

out vec4 final_color;

uniform sampler2D texture0;

void main()
{
    vec4 player_texture = texture(texture0, fragment_uv);
    // Discard alpha values
    if(player_texture.a < 0.1)
        discard;
    final_color = (player_texture * fragment_color);
}