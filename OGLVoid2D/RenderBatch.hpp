#pragma once
#include "GAO.h"
#include "Shader.h"

class RenderBatch {
	GAO *gao;
	Shader program;
	std::vector<ui32> elementVec;

	ui32 vaoIndex = 0;
	i32 lastElement = -1;
	std::vector<i32> textureIds	;

public:
	RenderBatch(GAO *_gao, ui32 _vaoIndex, const std::string& vertStr, const std::string&fragstr, bool path = true):
		gao(_gao), program(vertStr, fragstr, path), vaoIndex(_vaoIndex) {}

	void defineVertBufferData(const std::vector<ui32>& attributes, GLenum usage = GL_DYNAMIC_DRAW, ui32 size = 1000, const std::vector<float>& vertData = {}) {
		gao->defineVerBufferData(vaoIndex, attributes, usage, size, vertData);
	}

	/*(VAA) VertexAttributeArray*/
	void enableVAA(const std::vector<ui32>& atrrs) {
		gao->enable(vaoIndex, atrrs);
	}
	 


	void clearBatch() {
		gao->clearVerBufferData(vaoIndex);
		elementVec.clear();

		lastElement = -1;
	}

	void addVertices(const std::vector<float>& vertData, const std::vector<ui32>& newElems) {
		gao->addVerBufferData(vaoIndex, vertData);

		for (auto elem : newElems) {
			elementVec.push_back((ui32)lastElement + 1 + elem);
		}
		lastElement += newElems.size();
	}

	i32 addTexture(ui32 id, i32 unit = -1) {
		if (unit >= 0 && unit < textureIds.size()) {
			textureIds[unit] = id;
			return unit;
		}
		else if (textureIds.size() < 32) {
			textureIds.push_back(id);
			return textureIds.size() - 1;
		}
		return -1;
	}

	void DrawBatch(bool redraw = false) {
		program.use();

		for (int i = 0; i < textureIds.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		}

		if(!redraw) gao->setElBufferData(vaoIndex, elementVec, GL_DYNAMIC_DRAW);

		glDrawElements(GL_TRIANGLES, elementVec.size(), GL_UNSIGNED_INT, 0);
	}
	void ReDrawBatch() {
		glDrawElements(GL_TRIANGLES, elementVec.size(), GL_UNSIGNED_INT, 0);
	}
};