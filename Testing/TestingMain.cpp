#include <iostream>
#include <string>

#include "../OGLVoid2D/Lineal.h"

std::string balance(const std::string& a, const std::string& b) {
	std::string spcChar = "";

	while (a.length() + spcChar.length() < b.length()) {
		spcChar += " ";
	}

	return spcChar + a;
}

std::string toStr(const voi::Vec2f& vec) {
	std::string x = std::to_string(vec.x);
	std::string y = std::to_string(vec.y);
	return "| " + balance(x, y) + ", " + balance(y, x) + " |";
}

std::string toStr(const voi::Vec3f& vec) {
	std::string x = std::to_string(vec.x);
	std::string y = std::to_string(vec.y);
	return "| " + balance(x, y) + ", " + balance(y, x) + " |";
}

int main() {
	std::cout << toStr({123.25,123456}) << std::endl;
}