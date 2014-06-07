#version 400

in vec4 passThroughColor;
out vec4 finalFragColor;

void main()
{
	finalFragColor = passThroughColor;
}
