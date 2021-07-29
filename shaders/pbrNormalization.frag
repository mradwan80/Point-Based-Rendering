#version 450

out vec4 out_color;

layout(binding = 2) uniform sampler2D Image;
//layout(binding = 1) uniform sampler2D Image;

void main()
{
	vec4 inp = texelFetch(Image, ivec2(int(gl_FragCoord.x),int(gl_FragCoord.y)),0);
	if(inp.w>0.5)
		out_color = vec4(inp.xyz/inp.w,1.0);
	else
		out_color = vec4(1.0,1.0,1.0,1.0);

	//out_color = vec4(0.0,1.0,0.0,1.0);

	//out_color = vec4(inp.xyz,1.0);
	
}
