#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;	//not used here//
layout(location = 2) in int in_rad;	//to be used//

uniform mat4 vm_matrix;
uniform mat4 pvm_matrix;

flat out float dpt;

void main()
{
	
	vec4 pos4 = vec4(in_position,1.0);

	gl_Position =   pvm_matrix * pos4;

	vec4 view_pos = vm_matrix * pos4;
	dpt = -view_pos.z;
	
	gl_PointSize=8;

}

