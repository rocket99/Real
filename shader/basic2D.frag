#version 450 core

layout(location=0) in vec2 varyUv;

uniform sampler2D s0;

layout(location=0) out vec4 frag_color;

void main(){
	frag_color = texture(s0, varyUv);
}
