#version 460 core

in vec3 v_pos;

void main(void)
{
	gl_Position = vec4(v_pos, 1.0);
}
