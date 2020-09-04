#version 330 core

out vec4 FragColor;
in vec2 v_TextureCoordinates;  
uniform sampler2D u_Texture;
void main()
{
    FragColor = vec4(texture(u_Texture,v_TextureCoordinates));
}

