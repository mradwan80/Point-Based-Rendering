#include "Shader.h"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <limits>

#ifdef WIN32
#define WIN
#endif

#ifdef WIN64
#define WIN
#endif

#ifndef WIN
# include <unistd.h>
#endif
using namespace std;


char* textFileRead(char *fn) 
{
	ifstream file(fn, ios::in);
	file.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize size = file.gcount();
	file.clear();   //  Since ignore will have set eof.
	file.seekg(0, std::ios_base::beg);

	char* content = new char[size + 1];

	file.read(content, size);       // read the whole file into the buffer
	file.close();
	content[size] = '\0';
	
	return content;
}            

GLuint Shader::GetHandle()
{
	return handle;

}

Shader::Shader()
{
	handle = glCreateProgram();
}

Shader::Shader(string path)
{
	handle = glCreateProgram();
	shader_path=path;
}


//read shader file, and create corresponding shading program
void Shader::CompileShader()
{
	GLuint vSh,fSh;

	GLint * param=new GLint;

	vSh = glCreateShader(GL_VERTEX_SHADER);
	fSh = glCreateShader(GL_FRAGMENT_SHADER);	

	const char* file = shader_path.c_str();

	char name[150]="",vname[100],fname[100];
	strcat(name,file);
	strcpy(vname,name);
	strcat(vname,".vert");
	strcpy(fname,name);
	strcat(fname,".frag");



	char *vs = textFileRead(vname);
	char *fs = textFileRead(fname);

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(vSh, 1, &vv,NULL);
	glShaderSource(fSh, 1, &ff,NULL);

	glCompileShader(vSh);
	glCompileShader(fSh);

	glGetShaderiv(vSh, GL_COMPILE_STATUS,param);
	if(param[0]==0)
		cout<<" Compile error in vertex shader of "<<file<<'\n';
	glGetShaderiv(fSh, GL_COMPILE_STATUS,param);
	if(param[0]==0)
		cout << " Compile error in fragment shader of " << file << '\n';

	free(vs);free(fs); //important

	
	glAttachShader(handle,vSh);
	glAttachShader(handle,fSh);
	glLinkProgram(handle);

}
	
void Shader::CompileShaderWithGeomShader()
{
	GLuint vSh,gSh,fSh;
	GLint * param=new GLint;

	glProgramParameteriEXT(handle, GL_GEOMETRY_VERTICES_OUT_EXT, 3); // at least 3
	glProgramParameteriEXT(handle, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
	glProgramParameteriEXT(handle, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);


	vSh = glCreateShader(GL_VERTEX_SHADER);
	gSh = glCreateShader(GL_GEOMETRY_SHADER);
	fSh = glCreateShader(GL_FRAGMENT_SHADER);	

	const char* file = shader_path.c_str();

	char name[150]="",vname[100],gname[100],fname[100];
	strcat(name,file);
	strcpy(vname,name);
	strcat(vname,".vert");
	strcpy(gname,name);
	strcat(gname,".geom");
	strcpy(fname,name);
	strcat(fname,".frag");

	char *vs = textFileRead(vname);
	char *gs = textFileRead(gname);
	char *fs = textFileRead(fname);

	const char * vv = vs;
	const char * gg = gs;
	const char * ff = fs;

	glShaderSource(vSh, 1, &vv,NULL);
	glShaderSource(gSh, 1, &gg,NULL);
	glShaderSource(fSh, 1, &ff,NULL);

	glCompileShader(vSh);
	glCompileShader(gSh);
	glCompileShader(fSh);

	glGetShaderiv(vSh, GL_COMPILE_STATUS,param);
	if(param[0]==0)
		cout << " Compile error in vertex shader of " << file << '\n';
	glGetShaderiv(gSh, GL_COMPILE_STATUS,param);
	if(param[0]==0)
		cout << " Compile error in geometry shader of " << file << '\n';
	glGetShaderiv(fSh, GL_COMPILE_STATUS,param);
	if(param[0]==0)
		cout << " Compile error in fragment shader of " << file << '\n';

	free(vs);free(gs);free(fs); //important

	glAttachShader(handle,vSh);
	glAttachShader(handle,gSh);
	glAttachShader(handle,fSh);
	glLinkProgram(handle);

}

