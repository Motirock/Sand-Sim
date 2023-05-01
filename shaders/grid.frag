#version 330 core
out vec4 FragColor;
  
in vec4 color;
in float brightness;

void main()
{
	FragColor = color*brightness;
}