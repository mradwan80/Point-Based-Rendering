#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in float in_rad;	//to be used//

uniform mat4 vm_matrix;
uniform mat4 pvm_matrix;

flat out vec3 colorVF ; 
flat out float dptVF;

void main()
{
	
	/*vec4 pos4 = vec4(in_position,1.0);

	gl_Position =   pvm_matrix * pos4;

	colorVF = in_color;

	
	vec4 view_pos = vm_matrix * pos4;
	dptVF = -view_pos.z;
	
	gl_PointSize=8;*/

	//////////

	vec4 pos4 = vec4(in_position,1.0);
	gl_Position =   pvm_matrix * pos4;

	
	vec4 pos4_dx = vec4(in_position.x+in_rad,in_position.y,in_position.z,1.0);
	vec4 position_dx =   pvm_matrix * pos4_dx;

	float sx=(gl_Position.x/gl_Position.w)*0.5+0.5;
	float sx_dx=(position_dx.x/position_dx.w)*0.5+0.5;
	float srad = abs(sx_dx-sx)*1024;

	//gl_PointSize=2*int(srad);
	gl_PointSize=2*ceil(srad);
	//if(gl_PointSize==0) gl_PointSize=1;

	colorVF = in_color;

	
	vec4 view_pos = vm_matrix * pos4;
	dptVF = -view_pos.z;





}

