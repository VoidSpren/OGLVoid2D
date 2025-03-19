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
	struct BatchGroup {
		ui32 index;
		ui32 count;
		ui32 position;
		ui32 current;
	};

	class VoiOGLEngine {
		GLFWwindow* window;
		Pixel clearColor = { 0.f,0.f,0.f,0.f };

		GAO *mainGao;
		std::vector<RenderBatch> batches;

		float totalTime;
		float loopStartT;
		float loopEndT;

		ui64 frameCount = 0;

		ui32 shapeVertexCount = 0;

		ui32 textures[32];
		ui32 unasignedTexBatch = 0;

		//---batches configuration---//
		
		// index dictates in wich place de batch group starts, count how many consecutive batches of said group there are
		// to get the real position of a batch group, add the count of all previous batch groups
		BatchGroup solidGroup = { 0, 1, 0, 0 };
		BatchGroup singleTexGroup = { 1, 32, 1, 0 };

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
			glViewport(0, 0, width, height);

			/*sets function to callback when window is rezised*/
			glfwSetFramebufferSizeCallback(window, viewportResize);

			//glfwSwapInterval(0);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			mainGao = new GAO(
				solidGroup.count +
				singleTexGroup.count
			);
			glGenTextures(
				singleTexGroup.count
				, textures);

			batches.emplace_back(mainGao, solidGroup.position, "default.vert", "default.frag"); //solidBatch
			batches[solidGroup.position].defineVertBufferData({ 3,4 });

			std::ifstream vertexFile("texture.vert"), fragmentFile("texture.frag");
			std::stringstream vertexStream, fragmentStream;

			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();

			std::string vertexCode = vertexStream.str(), fragmentCode = fragmentStream.str();

			vertexFile.close(); fragmentFile.close();

			for (int i = singleTexGroup.position; i < (singleTexGroup.position + singleTexGroup.count); i++) {
				batches.emplace_back(mainGao, i, vertexCode.c_str(), fragmentCode.c_str(), false); //singleTexBatches
				batches[i].defineVertBufferData({ 3,4,2 });
			}
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
		void SetClearColor(const Pixel &p) {
			clearColor = p;
			glClearColor(p.r, p.g, p.b, p.a);
		}

		float GetTotalTime() { return totalTime; }

		ui64 GetFrameCount() { return frameCount; }

		GLFWwindow* GetWindow() { return window; }

		Pixel drawColor = { 1.0f,1.0f,1.0f,1.0f };

		bool ChooseCurrentTextures(ui32 batch, ui32 unit = 0) {
			if (batch >= 0 && batch < singleTexGroup.count) {
				singleTexGroup.current = batch;
				return true;
			}
			return false;
		}

		ui32 AddTexture(int width, int height, const ui8 *data, bool mipmap = true, GLenum pixType = GL_RGBA, ui32 unit = 0, i32 batch = -1) {
			if (data) {
				i32 batchIndex = batch;
				if (batchIndex < 0) {
					if (unasignedTexBatch >= 32)
						return -1;
					batchIndex = unasignedTexBatch;
					unasignedTexBatch++;
				}

				glBindTexture(GL_TEXTURE_2D, textures[batchIndex]);


				// set the texture wrapping/filtering options (on the currently bound texture object)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, pixType, GL_UNSIGNED_BYTE, data);
				if (mipmap) {
					glGenerateMipmap(GL_TEXTURE_2D);
				}

				batches[batchIndex + singleTexGroup.position].addTexture(textures[batchIndex]);

				return batchIndex;
			}

			return -1;
		}

		void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float z = 0) {
			FillTriangle({ x1,y1 }, { x2,y2 }, { x3,y3 }, z);
		}
		void FillTriangle(voi::Vec2f p1, voi::Vec2f p2, voi::Vec2f p3, float z = 0) {
			batches[solidGroup.current + solidGroup.position].addVertices({
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

			batches[solidGroup.current + solidGroup.position].addVertices({
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

		void TextureTri(voi::Vec2f p1, voi::Vec2f p2, voi::Vec2f p3, float z = 0,
			voi::Vec2f t1 = { 0.0,0.0 }, voi::Vec2f t2 = { 1.0,0.0 }, voi::Vec2f t3 = { 0.0,1.0 }) {

			batches[singleTexGroup.current + singleTexGroup.position].addVertices({
				p1.x, p1.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t1.x, t1.y,
				p2.x, p2.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t2.x, t2.y,
				p3.x, p3.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t3.x, t3.y
				}, { 0, 1, 2 });
		}

		void TextureQuad(voi::Vec2f p1, voi::Vec2f p2, voi::Vec2f p3, voi::Vec2f p4, float z = 0,
			voi::Vec2f t1 = { 0.0,0.0 }, voi::Vec2f t2 = { 1.0,0.0 }, voi::Vec2f t3 = { 1.0,1.0 }, voi::Vec2f t4 = { 0.0,1.0 }) {

			batches[singleTexGroup.current + singleTexGroup.position].addVertices({
				p1.x, p1.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t1.x, t1.y,
				p2.x, p2.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t2.x, t2.y,
				p3.x, p3.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t3.x, t3.y,
				p4.x, p4.y, z, drawColor.r, drawColor.g, drawColor.b, drawColor.a, t4.x, t4.y
				}, { 0, 1, 2, 2, 3, 0 });
		}

		void TextureRect(float x, float y, float w, float h, float z = 0,
			voi::Vec2f t1 = { 0.0,0.0 }, voi::Vec2f t2 = { 1.0,0.0 }, voi::Vec2f t3 = { 1.0,1.0 }, voi::Vec2f t4 = { 0.0,1.0 }) {
			TextureQuad(
				{ x, y },
				{ x + w, y },
				{ x + w, y + h },
				{ x, y + h },
				z, 
				t1, t2, t3, t4
			);
		}

		void FillVertices(voi::Vec2f vert) {

		}

	private:

		void First() {
			loopStartT = glfwGetTime();
			loopEndT = loopStartT;


			this->Begin();

			batches[0].enableVAA({ 0,1 });
			batches[1].enableVAA({ 0,1,2 });

			glClear(GL_COLOR_BUFFER_BIT);

			for(auto &batch: batches) { batch.DrawBatch(); }
			glfwSwapBuffers(window);

			glClear(GL_COLOR_BUFFER_BIT);

			for (auto &batch : batches) { batch.ReDrawBatch(); }
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

				for (auto batch : batches) {
					batch.DrawBatch();
				}

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
