#version 450

layout(location = 0) out vec4 out_color;

flat in float dpt;

void main()
{

	out_color = vec4(1.0,1.0,dpt,1.0);	//just output the dpt in some texture//
}

