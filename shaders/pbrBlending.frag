#version 450

out vec4 out_color;

layout(binding = 1) uniform sampler2D DptMap;

flat in vec3 colorVF ; 
flat in float dptVF;

void main() {

	//if(dptVF<-10)
	//	discard;

	vec4 dptvec = texelFetch(DptMap, ivec2(int(gl_FragCoord.x),int(gl_FragCoord.y)), 0);
	float mindpt = dptvec.z;
	
	
	float thres=0.1;
	//float thres=0.000001;	//blend nothing
	//float thres=50;		//blend everying
	
	if(dptVF-mindpt<thres)
		out_color = vec4(colorVF, 1.0);
	else
		discard;

}

