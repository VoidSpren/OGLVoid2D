#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Lineal.h"
#include "GAO.h"
#include "Renderer.h"
#include <random>

/*program created following the examples and tutorials from https://learnopengl.com*/

/*Click on System.
Change "SubSystem" to Windows (/SUBSYSTEM:WINDOWS)
Click on Advanced under the expanded Linker items.
In the Entry Point box, change it to mainCRTStartup.

That should force your project to compile as a windows application
and not a console application, so the console won't be called.*/

//void viewportSizeCallback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}

class Testing : public voi::VoiOGLEngine {

	std::random_device randDev;
	std::default_random_engine randEng{ randDev() };
	std::uniform_real_distribution<float> getRandPos{ -1.0f,1.0f };
	std::uniform_real_distribution<float> getRandNormal{ 0.0f,1.0f };

	voi::Vec2f ap1 = { -0.5,-0.5 };
	voi::Vec2f bp1 = { -0.5, 0.5 };

	voi::Vec2f ap2 = { -0.5, 0.5 };
	voi::Vec2f bp2 = {  0.5, 0.5 };

	voi::Vec2f ap3 = {  0.5, 0.5 };
	voi::Vec2f bp3 = {  0.5,-0.5 };

	voi::Vec2f ap4 = {  0.5,-0.5 };
	voi::Vec2f bp4 = { -0.5,-0.5 };

	voi::Pixel colorA = { 1.0f,0.0f,0.0f,1.0f };
	voi::Pixel colorB = { 0.0f,0.0f,1.0f,1.0f };

	int framePrevIntT = 0;
	int prevFrameCount = 0;

	float prevTimeInterval = 0;

	float timeInterval = 1.5f;

	voi::Vec2f lerp(voi::Vec2f a, voi::Vec2f b, float t) {
		return a + (b - a) * t;
	}

	void setTitleToFramesInSecond() {
		int intT = GetTotalTime();

		if (intT > framePrevIntT) {
			glfwSetWindowTitle(GetWindow(), std::to_string(GetFrameCount() - prevFrameCount).c_str());
			framePrevIntT = intT;
			prevFrameCount = GetFrameCount();
		}
	}

	void Begin() override {
		//FillTriangle(lerp(ap1, bp1, 0.5f), lerp(ap2, bp2, 0.5f), lerp(ap3, bp3, 0.5f));
	}

	void Update(float delta) override{
		setTitleToFramesInSecond();

		//Clear();

		float t = GetTotalTime() / timeInterval;
		t = (t - (int)t)* timeInterval;

		if (t < prevTimeInterval) {

			drawColor = { getRandNormal(randEng), getRandNormal(randEng), getRandNormal(randEng), 1.0 };

			voi::Vec2f M = { getRandPos(randEng), getRandPos(randEng) };
			voi::Vec2f N = { getRandPos(randEng), getRandPos(randEng) };
			voi::Vec2f O = { getRandPos(randEng), getRandPos(randEng) };

			FillTriangle(M, N, O);
		}

		prevTimeInterval = t;
	}
};

int main() {

	Testing test;
	if (test.Construct("VoiOGLEngine", 800, 600)) test.Start();

	///*starts the glfw enviroment*/
	//glfwInit();
	///*hints at the version of openGL to use (3.3)*/
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	///*hints that we want to use the core mode in openGL*/
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	///*creates the window*/
	//GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//if (window == NULL) {
	//	std::cout << "Failed to create GLFW window" << std::endl;
	//	glfwTerminate();
	//	return -1;
	//}

	///*makes the created window the current context in wich glfw works*/
	//glfwMakeContextCurrent(window);

	///*GLAD initialization*/
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}

	///*sets opengl viewport size*/
	//glViewport(0, 0, 800, 600);

	///*sets function to callback when window is rezised*/
	//glfwSetFramebufferSizeCallback(window, viewportSizeCallback);

	//glClearColor(0.0f,0.01f,0.1f,1.0f);
	///*Data preparation for Vertices-------------------------------*/

	//const std::vector<voi::Vec2f> hexPosVec = {
	//	{ -0.5f,  -0.5f  },
	//	{ -0.5f,   0.5f  },	
	//	{  0.0f,   0.75f },	
	//	{  0.5f,   0.5f  },	
	//	{  0.5f,  -0.5f  },	
	//	{  0.0f,  -0.75f }	
	//};
	//const std::vector<voi::Vec2f> sqrPosVec = {
	//	{ -0.75f,   0.75f  },
	//	{  0.75f,   0.75f  },
	//	{ -0.75f,  -0.75f  },
	//	{  0.75f,  -0.75f  },
	//};

	//const std::vector<voi::Vec3f> colorVec = {
	//	{ 1.0f, 0.0f, 0.0f },
	//	{ 1.0f, 1.0f, 0.0f },
	//	{ 0.0f, 1.0f, 0.0f },
	//	{ 0.0f, 1.0f, 1.0f },
	//	{ 0.0f, 0.0f, 1.0f },
	//	{ 1.0f, 0.0f, 1.0f }
	//};

	//const std::vector<voi::Vec2f> sqrTexCord = {
	//	{ 0.0f, 0.0f },
	//	{ 1.0f, 0.0f },
	//	{ 0.0f, 1.0f },
	//	{ 1.0f, 1.0f }
	//};

	//const std::vector<uint32_t> hexVert = {
	//	0, 1, 2,
	//	0, 2, 3,
	//	0, 3, 5,
	//	5, 3, 4
	//};
	//const std::vector<uint32_t> sqrVert = {
	//	0, 1, 2,
	//	1, 2, 3
	//};
	//const std::vector<uint32_t> triVert = {
	//	0, 2, 4
	//};

	///*Data configuration only------------------------------------*/

	///*TEXTURE*/
	//
	///* all texture config required */

	//uint32_t texture0;
	//glGenTextures(1, &texture0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture0);

	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//

	//int imgWidth, imhHeight, nChannels;
	//uint8_t* imgData = stbi_load("container.jpg", &imgWidth, &imhHeight, &nChannels, 0);

	//if (imgData) {
	//	std::cout << "IMAGE::LOADED" << nChannels << std::endl;
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imhHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else std::cout << "IMAGE::FAILED" << std::endl;

	//stbi_image_free(imgData);

	//uint32_t texture1;
	//glGenTextures(1, &texture1);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture1);

	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	////glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//imgData = stbi_load("awesomeface.png", &imgWidth, &imhHeight, &nChannels, 0);

	//if (imgData) {
	//	std::cout << "IMAGE::LOADED" << nChannels << std::endl;
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imhHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else std::cout << "IMAGE::FAILED" << std::endl;

	//stbi_image_free(imgData);

	///*VERTEXOBJECT*/
	////after abstraction
	//GAO rainbowShape;
	//rainbowShape.setVerBufferData(0, sqrPosVec, colorVec, sqrTexCord, GL_STATIC_DRAW);
	//rainbowShape.setElBufferData(0, sqrVert, GL_STATIC_DRAW);


	///*ALWAYS REMEMBER TO ENABLE VERTEX ATTRIBUTES*/
	//rainbowShape.enable(0, { 0,1,2 });

	//Shader program("texture.vert", "texture.frag");

	//program.setInt("tex0", 0);
	//program.setInt("tex1", 1);

	//program.setFloat("mixLinear", 0.3f);

	//float mixLinear = 0.3f;

	//auto printVec = [](voi::Vec4i v) {
	//	std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << "\n";
	//};
	//auto printMat = [&printVec](voi::Mat4i m) {
	//	for (auto i : m.m) {
	//		printVec(i);
	//	}
	//};

	//voi::Mat4i A{ {
	//	{2,0,0,5},
	//	{0,3,0,5},
	//	{0,0,2,5},
	//	{0,0,0,1}
	//} };

	//voi::Vec4i vA{ 1,1,1,1 };

	//printVec(A * vA);

	//float elapsed = glfwGetTime();
	//float prevTime = glfwGetTime();

	///*render loop*/
	//while (!glfwWindowShouldClose(window)) {
	//	elapsed = glfwGetTime() - prevTime;
	//	prevTime = glfwGetTime();

	//	processInput(window);

	//	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	//		mixLinear += 0.2;
	//	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	//		mixLinear += -0.2 * elapsed;

	//	program.setFloat("mixLinear", mixLinear);

	//	glClear(GL_COLOR_BUFFER_BIT);

	//	glDrawElements(GL_TRIANGLES, sqrVert.size(), GL_UNSIGNED_INT, 0);

	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	///*cleans resources alocated by glfw*/
	//glfwTerminate();

	return 0;
}
