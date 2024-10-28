#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include "utilDefs.h"
#include "Pixel.h"
#include "GAO.h"
#include "Shader.h"
#include "RenderBatch.hpp"

namespace voi {
	class VoiOGLEngine {
		GLFWwindow* window;
		Pixel clearColor;

		GAO *mainGao;
		std::vector<RenderBatch> batches;

		float totalTime;
		float loopStartT;
		float loopEndT;

		ui64 frameCount = 0;

		ui32 shapeVertexCount = 0;

	public:
		~VoiOGLEngine() {
			if (mainGao != nullptr) delete mainGao;
		}

		void Start() {
			First();
			/*cleans resources alocated by glfw*/
			glfwTerminate();
		}
		bool Construct(const char* title, ui32 width, ui32 height) {
			glfwInit();
			/*hints at the version of openGL to use (3.3)*/
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			/*hints that we want to use the core mode in openGL*/
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			/*creates the window*/
			window = glfwCreateWindow(width, height, title, NULL, NULL);

			if (window == NULL) {
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return false;
			}

			/*makes the created window the current context in wich glfw works*/
			glfwMakeContextCurrent(window);
			glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);

			/*GLAD initialization*/
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize GLAD" << std::endl;
				return false;
			}


			/*sets opengl viewport size*/
			glViewport(0, 0, 800, 600);

			/*sets function to callback when window is rezised*/
			glfwSetFramebufferSizeCallback(window, viewportResize);

			//glfwSwapInterval(0);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			mainGao = new GAO(1);

			batches.emplace_back(mainGao, 0, "default.vert", "default.frag"); //batches[0] = plygons with textures batch

			batches[0].defineVertBufferData({ 3,4 });
		}


	protected:
		virtual void Begin() = 0;
		virtual void Update(float deltaTime) = 0;

		void Clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto &batch : batches) {
				batch.clearBatch();
			}
		}

		Pixel GetClearColor() { return clearColor; }
		void setClearColor(const Pixel &p) {
			clearColor = p;
			glClearColor(p.r, p.g, p.b, p.a);
		}

		float GetTotalTime() { return totalTime; }

		ui64 GetFrameCount() { return frameCount; }

		GLFWwindow* GetWindow() { return window; }

		Pixel drawColor = { 1.0f,1.0f,1.0f,1.0f };

		void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float z = 0) {
			FillTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 }, z);
		}
		void FillTriangle(voi::Vec2f p1, voi::Vec2f p2, voi::Vec2f p3, float z = 0) {
			batches[0].addVertices({
				p1.x, p1.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a,
				p2.x, p2.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a,
				p3.x, p3.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a
				}, { 0,1,2 }
			);
		}

		void FillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float z = 0) {
			FillTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 });
		}
		void FillQuad(voi::Vec2f p1, voi::Vec2f p2, voi::Vec2f p3, voi::Vec2f p4, float z = 0) {

			batches[0].addVertices({
				p1.x, p1.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a,
				p2.x, p2.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a,
				p3.x, p3.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a,
				p4.x, p4.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a
			}, { 0, 1, 2, 2, 3, 0 });
		}

		void FillRect(float x, float y, float w, float h, float z = 0) {
			FillQuad(
				{     x, y     },
				{ x + w, y     },
				{ x + w, y + h },
				{     x, y + h },
				z
			);
		}

		void FillVertices() {

		}

	private:

		void First() {
			loopStartT = glfwGetTime();
			loopEndT = loopStartT;


			this->Begin();

			batches[0].enableVAA({ 0,1 });

			glClear(GL_COLOR_BUFFER_BIT);

			for(auto &batch: batches) { batches[0].DrawBatch(); }
			glfwSwapBuffers(window);

			glClear(GL_COLOR_BUFFER_BIT);

			for (auto &batch : batches) { batches[0].ReDrawBatch(); }
			glfwSwapBuffers(window);

			frameCount++;

			this->Loop();
		}
		void Loop() {
			float elapsed = 0;
			while (!glfwWindowShouldClose(window)) {
				loopEndT = glfwGetTime();
				totalTime = loopEndT;

				elapsed = loopEndT - loopStartT;
				loopStartT = loopEndT;

				this->Update(elapsed);

				//defaultGao->setElBufferData(0, defaultElementVec, GL_DYNAMIC_DRAW);

				//defaultGao->enable(0, { 0,1 });

				//glDrawElements(GL_TRIANGLES, defaultElementVec.size(), GL_UNSIGNED_INT, 0);

				for (auto batch : batches) { batches[0].DrawBatch(); }

				glfwSwapBuffers(window);

				frameCount++;

				glfwPollEvents();
			}
		}

		static void viewportResize(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}
	};
}
