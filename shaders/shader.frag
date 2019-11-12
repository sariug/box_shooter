#version 330

in vec4 vCol;
in vec2 TexCoord;

uniform sampler2D theTexture;

out vec4 color; 

void main()
{
	color = texture(theTexture, TexCoord);//*vCol;//;
}