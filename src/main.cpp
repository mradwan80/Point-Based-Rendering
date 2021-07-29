
//in future will have explore and navigate modes

#include "main.h"
#include "Shader.h"
#include "FloatTexture.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	
	//if (action==GLFW_RELEASE)
	//	return;

	switch (key)
	{
		
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
	}
	
}

int main()
{
	int GlobalW=1024;
	int GlobalH=800;

	if (!glfwInit())
	{
		std::cout << "GLFW Initialization failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* myWindow = glfwCreateWindow(GlobalW, GlobalH, "DDS-Layers", NULL, NULL);
	if (!myWindow)
	{
		std::cout << "failed to create GLFW Window" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(myWindow);
	glfwSwapInterval(0);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	vector<pointCoords> Coords;
	vector<pointColor> Colors;
	vector<float>Rads;	//constant for now//
	
	//read data from file//
	//std::ifstream inputfile("../models/three_bunnies.xyz", std::ios_base::in);
	std::ifstream inputfile("../models/conference-room.xyz", std::ios_base::in);
	float x,y,z,r,g,b,o;
	float maxx=std::numeric_limits<float>::lowest();
	float maxy=std::numeric_limits<float>::lowest();
	float maxz=std::numeric_limits<float>::lowest();
	float minx=std::numeric_limits<float>::max();
	float miny=std::numeric_limits<float>::max();
	float minz=std::numeric_limits<float>::max();
	while(!inputfile.eof())
	{
		inputfile>>x>>y>>z>>r>>g>>b>>o;
		
		//Coords.push_back(pointCoords(x,y,z));
		pointCoords p; p.x=x;p.y=y;p.z=z;
		Coords.push_back(p);

		//Colors.push_back(pointColor(r,g,b));
		pointColor c;c.r=r/255;c.g=g/255;c.b=b/255;
		Colors.push_back(c);

		Rads.push_back(0.05);

		if(x<minx)	minx=x;
		if(y<miny)	miny=y;
		if(z<minz)	minz=z;
		if(x>maxx)	maxx=x;
		if(y>maxy)	maxy=y;
		if(z>maxz)	maxz=z;
	}
	int pnum=Coords.size();	//points number//
	float midx=(minx+maxx)/2;
	float midy=(miny+maxy)/2;
	float midz=(minz+maxz)/2;
	float ViewWidth=(maxx-minx)*1.5;

	//vao and vbos//
	GLuint vao;
	vector<GLuint> vbo;
	vbo.resize(4);
	glGenVertexArrays(1, &vao); 
	glGenBuffers(4, &vbo[0]); 
	glBindVertexArray(vao); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pnum * 3 * sizeof(GLfloat), &Coords[0], GL_DYNAMIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, pnum * 3 * sizeof(GLfloat), &Colors[0], GL_DYNAMIC_DRAW_ARB);
	//send rad !!!
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	//glBufferData(GL_ARRAY_BUFFER, pnum * 1 * sizeof(GLint), &ObjectIds[0], GL_DYNAMIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	GLuint vaoRect;
	vector<GLuint> vboRect;
	vboRect.resize(1);
	float RectVx [] = {-1.0,-1.0,0.5,	-1.0,+1.0,0.5,	+1.0,-1.0,0.5,	+1.0,+1.0,0.5};
	glGenVertexArrays(1, &vaoRect);
	glBindVertexArray(vaoRect); //  vertex array
	glGenBuffers(1, &vboRect[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboRect[0]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), RectVx, GL_DYNAMIC_DRAW_ARB);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//matrices//
	glm::mat4 ModelMat = glm::translate(glm::vec3(0,0,0));
	glm::mat4 ViewMat = glm::translate(glm::vec3(-midx,-midy,-(minz+1.95*(maxz-midz)))); //for conference room
	//glm::mat4 ViewMat = glm::translate(glm::vec3(-midx,-midy,-(minz+3*(maxz-midz))));	//for 3 bunnies
	float fov=70.0;
	float Near = 0.01;
	float Far = 7*(maxz-minz);
	glm::mat4 ProjectionMat =	glm::perspective(fov, float(GlobalW)/float(GlobalH) , Near, Far);
	glm::mat4 vmMat = ViewMat*ModelMat;
	glm::mat4 pvmMat = ProjectionMat*ViewMat*ModelMat;


	//read shaders//
	//Shader PntRdr("../shaders/PointsRenderer");
	//PntRdr.CompileShader();
	Shader VisibilitySh("../shaders/pbrVisibility");
	VisibilitySh.CompileShader();
	Shader BlendingSh("../shaders/pbrBlending");
	BlendingSh.CompileShader();
	Shader NormalizationSh("../shaders/pbrNormalization");
	NormalizationSh.CompileShader();
	
	FloatTexture* dptTex = new FloatTexture(GlobalW, GlobalH);
	FloatTexture* accTex = new FloatTexture(GlobalW, GlobalH);

	GLuint dptFbo; glGenFramebuffers(1, &dptFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, dptFbo);
	GLuint r0;	glGenTextures(1, &r0);
	glBindTexture(GL_TEXTURE_2D, r0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, GlobalW, GlobalH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, r0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	dptTex->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dptTex->GetHandle(), 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	///////
	GLuint accFbo; glGenFramebuffers(1, &accFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, accFbo);
	GLuint r1;	glGenTextures(1, &r1);
	glBindTexture(GL_TEXTURE_2D, r1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, GlobalW, GlobalH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, r0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	accTex->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accTex->GetHandle(), 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//textures and fbos
	/*GLuint dptFbo; glGenFramebuffers(1, &dptFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, dptFbo);
	GLuint r0;	glGenTextures(1, &r0);
	glBindTexture(GL_TEXTURE_2D, r0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, GlobalW, GlobalH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, r0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint dptTex;	glGenTextures(1, &dptTex);
	glBindTexture(GL_TEXTURE_2D, dptTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, GlobalW, GlobalH, 0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dptTex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	/////
	GLuint accFbo;	glGenFramebuffers(1, &accFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, accFbo);
	GLuint r1;	glGenTextures(1, &r1);
	glBindTexture(GL_TEXTURE_2D, r1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, GlobalW, GlobalH, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, r1, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint accTex;	glGenTextures(1, &accTex);
	glBindTexture(GL_TEXTURE_2D, accTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, GlobalW, GlobalH, 0, GL_RGBA, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accTex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
	
	
	glfwSetKeyCallback(myWindow, key_callback);

	
	glEnable(GL_PROGRAM_POINT_SIZE);
	glViewport(0, 0, GlobalW, GlobalH);

	//render loop//
	while (!glfwWindowShouldClose(myWindow))
	{
		//////////////
		//Visibility//
		//////////////
	
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glUseProgram(VisibilitySh.GetHandle());
		glBindFramebuffer(GL_FRAMEBUFFER, dptFbo); //set the fbo//
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClearDepth(1.0);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(1);
		glUniformMatrix4fv(glGetUniformLocation(VisibilitySh.GetHandle(), "vm_matrix"), 1, GL_FALSE, glm::value_ptr(vmMat));
		glUniformMatrix4fv(glGetUniformLocation(VisibilitySh.GetHandle(), "pvm_matrix"), 1, GL_FALSE, glm::value_ptr(pvmMat));
		glDrawArrays(GL_POINTS, 0, pnum);
	
		////////////
		//Blending//
		////////////

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		
		glUseProgram(BlendingSh.GetHandle());
		glBindFramebuffer(GL_FRAMEBUFFER, accFbo); //set the fbo//
		glClearDepth(1.0);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, accFbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(1);
		glUniformMatrix4fv(glGetUniformLocation(BlendingSh.GetHandle(), "vm_matrix"), 1, GL_FALSE, glm::value_ptr(vmMat));
		glUniformMatrix4fv(glGetUniformLocation(BlendingSh.GetHandle(), "pvm_matrix"), 1, GL_FALSE, glm::value_ptr(pvmMat));
		//glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, dptTex);
		glActiveTexture(GL_TEXTURE1);		glBindTexture(GL_TEXTURE_2D, dptTex->GetHandle());
		glDrawArrays(GL_POINTS, 0, pnum);

		glDisable(GL_BLEND);

		/////////////////
		//Normalization//
		////////////////

		glUseProgram(NormalizationSh.GetHandle());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearDepth(1.0);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vaoRect);
		glBindBuffer(GL_ARRAY_BUFFER, vboRect[0]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(0);
		//glActiveTexture(GL_TEXTURE2);		glBindTexture(GL_TEXTURE_2D, accTex);
		glActiveTexture(GL_TEXTURE2);		glBindTexture(GL_TEXTURE_2D, accTex->GetHandle());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


		glfwSwapBuffers(myWindow);
		glfwPollEvents(); 
	}

	return 1;
}
