#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in int in_rad;	//to be used//

uniform mat4 vm_matrix;
uniform mat4 pvm_matrix;

flat out vec3 colorVF ; 
flat out float dptVF;

void main()
{
	
	vec4 pos4 = vec4(in_position,1.0);

	gl_Position =   pvm_matrix * pos4;

	colorVF = in_color;

	
	vec4 view_pos = vm_matrix * pos4;
	dptVF = -view_pos.z;
	
	gl_PointSize=8;

}

