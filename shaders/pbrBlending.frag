#version 450

out vec4 out_color;

layout(binding = 1) uniform sampler2D DptMap;

flat in vec3 colorVF ; 
flat in float dptVF;

void main() {

	/*if(dptVF<-10)
		discard;

	vec4 dptvec = texelFetch(DptMap, ivec2(int(gl_FragCoord.x),int(gl_FragCoord.y)), 0);
	float mindpt = dptvec.z;
	
	//if(mindpt>0.69 && mindpt<0.71)
	//if(mindpt<0.5)
	if(mindpt>0.5)
	{
		out_color = vec4(1.0, 1.0, 0.0, 1.0);
		return;
	}

	out_color = vec4(1.0, 1.0, 0.0, 1.0);
	return;

	
	//float thres=0.01;
	float thres=0.05;
	//float thres=5;
	
	if(dptVF-mindpt<thres)
		out_color = vec4(colorVF, 1.0);
	else
		discard;*/

	vec4 dptvec = texelFetch(DptMap, ivec2(int(gl_FragCoord.x),int(gl_FragCoord.y)), 0);
	float mindpt = dptvec.z;

	//if(mindpt>0.5)
	if(mindpt<0.5)
	{
		out_color = vec4(0.0, 0.0, 1.0, 1.0);
		return;
	}
	
	out_color = vec4(1.0, 1.0, 0.0, 1.0);

	//out_color = vec4(colorVF, 1.0);

}

