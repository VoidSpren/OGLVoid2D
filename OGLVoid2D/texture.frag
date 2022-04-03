#version 330 core

in vec4 vColor;
in vec2 vTexCord;

out vec4 fColor;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float mixLinear;

void main(){
	fColor = mix(texture(tex0, vTexCord), texture(tex1, vTexCord), mixLinear);
}
