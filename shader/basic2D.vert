#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color;
layout(location = 4) in vec2 uv;

layout(set=0, binding=0) uniform param {
	vec2 screen;
	vec2 position;
	vec2 scale;
	vec2 rotate;
	vec4 uv_modset;
};

layout(location=0) out varyUv;

void main(){
	float angle = param.rotate.x;
	vec2 P0 = vec2(pos.x*cos(angle)+pos.y*sin(angle), -pos.x*sin(angle)+pos.y*cos(angle));
	gl_Position = vec4((P0*param.scale+param.position)/param.screen*2.0 - 1.0, 0.0, 1.0);
	varyUv = uv*param.uv_modset.xy+param.uv_modeset.zw;
}

