#version 400

in vec4 vColor;
in vec2 vUv;

uniform sampler2D tex;

out vec4 color;

void main()
{
	color = vColor * texture2D(tex, vUv);
}