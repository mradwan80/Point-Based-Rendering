#version 450

layout(location = 0) out vec4 out_color;

flat in float dpt;

//flat in vec3 colorVF ; 

void main() {

	//out_color = vec4(1.0,1.0,dpt,1.0);	//just output the dpt in some texture//

	//out_color = vec4(1.0,1.0,30,1.0);	//just output the dpt in some texture//
	
	//out_color = vec4(1.0,1.0,0.7,1.0);	//just output the dpt in some texture//

	out_color = vec4(1.0,1.0,0.0,1.0);	//just output the dpt in some texture//

	
	//out_color = vec4(colorVF,1.0);	//just output the dpt in some texture//
}

