#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include "Lineal.h"

#ifndef MAX_GAO_SIZE
#define MAX_GAO_SIZE 1
#endif // !MAX_GAO_SIZE

template<typename T, typename U>
struct VABO {
	std::vector <T> Pos2DVec;
	std::vector <U> colorVec;
	std::vector <voi::Vec2f> TexCordVec;
};

typedef VABO<voi::Vec2f, voi::Vec3f> VABO_2D_RGB;
typedef VABO<voi::Vec3f, voi::Vec3f> VABO_3D_RGB;
typedef VABO<voi::Vec2f, voi::Vec4f> VABO_2D_RGBA;
typedef VABO<voi::Vec3f, voi::Vec4f> VABO_3D_RGBA;

class GAO {

	static const int MAX_SIZE = MAX_GAO_SIZE;
	
	uint32_t VAOs[MAX_SIZE];
	uint32_t VBOs[MAX_SIZE];
	uint32_t EBOs[MAX_SIZE];

public:
	GAO(){
		glGenVertexArrays(MAX_SIZE, VAOs);
		glGenBuffers(MAX_SIZE, VBOs);
		glGenBuffers(MAX_SIZE, EBOs);
		for (int i = 0; i < MAX_SIZE; i++) {
			glBindVertexArray(VAOs[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		}
	}
	~GAO() {
		glDeleteVertexArrays(MAX_SIZE, VAOs);
		glDeleteBuffers(MAX_SIZE, VBOs);
		glDeleteBuffers(MAX_SIZE, EBOs);
	}

	void bind(uint32_t i){
		if (i < MAX_SIZE) {
			glBindVertexArray(VAOs[i]);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void enable(uint32_t i, const std::vector<uint32_t>& atrrs) {
		if (i < MAX_SIZE) {
			for (auto n : atrrs) {
				glEnableVertexAttribArray(n);
			}
		}
		else {
			throw "Outside of range Exception";
		}
	}

	void setElBufferData(uint32_t i, const std::vector<uint32_t>& elData, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elData.size() * sizeof(uint32_t), elData.data(), usage);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec2f>& pos, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 3);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(0.0f);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec3f>& pos, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 3);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(pos[a].z);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	/*TODO: create color type in voi*/
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec2f>& pos, const std::vector<voi::Vec3f>& color, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 6);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(0.0f);
				vertData.emplace_back(color[a].x);
				vertData.emplace_back(color[a].y);
				vertData.emplace_back(color[a].z);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec3f>& pos, const std::vector<voi::Vec3f>& color, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 3);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(pos[a].z);
				vertData.emplace_back(color[a].x);
				vertData.emplace_back(color[a].y);
				vertData.emplace_back(color[a].z);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec2f>& pos, const std::vector<voi::Vec2f>& texCord, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 3);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(0.0f);
				vertData.emplace_back(texCord[a].x);
				vertData.emplace_back(texCord[a].y);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(uint32_t i, const std::vector<voi::Vec3f>& pos, const std::vector<voi::Vec2f>& texCord, GLenum usage) {
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 3);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(pos[a].z);
				vertData.emplace_back(texCord[a].x);
				vertData.emplace_back(texCord[a].y);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(
		uint32_t i,
		const std::vector<voi::Vec2f>& pos,
		const std::vector<voi::Vec3f>& color,
		std::vector<voi::Vec2f> texcord,
		GLenum usage)
	{
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 6);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(0.0f);
				vertData.emplace_back(color[a].x);
				vertData.emplace_back(color[a].y);
				vertData.emplace_back(color[a].z);
				vertData.emplace_back(texcord[a].x);
				vertData.emplace_back(texcord[a].y);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		}
		else {
			throw "Outside of range Exception";
		}
	}
	void setVerBufferData(
		uint32_t i,
		const std::vector<voi::Vec3f>& pos,
		const std::vector<voi::Vec3f>& color,
		std::vector<voi::Vec2f> texcord,
		GLenum usage)
	{
		if (i < MAX_SIZE) {
			bind(i);
			std::vector<float> vertData;
			vertData.reserve(pos.size() * 6);

			for (int a = 0; a < pos.size(); a++) {
				vertData.emplace_back(pos[a].x);
				vertData.emplace_back(pos[a].y);
				vertData.emplace_back(pos[a].z);
				vertData.emplace_back(color[a].x);
				vertData.emplace_back(color[a].y);
				vertData.emplace_back(color[a].z);
				vertData.emplace_back(texcord[a].x);
				vertData.emplace_back(texcord[a].y);
			}

			glBufferData(GL_ARRAY_BUFFER, vertData.size() * sizeof(float), vertData.data(), usage);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		}
		else {
			throw "Outside of range Exception";
		}
	}

private:
};
