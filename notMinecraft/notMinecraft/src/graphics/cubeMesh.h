#pragma once

#include <map>

struct blockTextures
{
	int frameIndex_top;
	int frameIndex_sides;
	int frameIndex_bottom;
};

const std::map<std::string, blockTextures> blockTypes = {
	{"grass", {12, 13, 14}},
	{"dirt", {14, 14, 14}},
	{"stone", {0, 1, 2}},
	{"wood", {4, 5, 6}},
};

class cubeMesh
{
public:
	cubeMesh();
	cubeMesh(std::string);
	~cubeMesh();

	unsigned int getVAO();
	GLsizei numVerts();

private:
	void generateTexCoords(const blockTextures&);


	unsigned int vboID;
	unsigned int vertsVBO;
	unsigned int texCoordsVBO;
	unsigned int EBO;
	unsigned int vaoID; // 1 vao per cube
};



// side
//i: 0 0.25
//i : 1 0.25
//i : 2 0.25
//i : 3 0.25
//i : 4 0.5
//i : 5 0.75
//i : 6 0.25
//i : 7 0.75
//i : 8 0.5
//i : 9 0.5
//i : 10 0
//i : 11 0.5
//i : 12 0
//i : 13 0.25
//i : 14 0.5
//i : 15 0.75
//i : 16 0
//i : 17 1
//i : 18 0
//i : 19 0.5
//i : 20 0.25
//i : 21 0
//i : 22 0
//i : 23 1
//i : 24 0
//i : 25 0
//i : 26 0
//i : 27 0.25
//i : 28 0
//i : 29 0
//i : 30 0
//i : 31 1
//i : 32 0.5
//i : 33 0.75
//i : 34 0.75
//i : 35 0.75
//i : 36 0.75
//i : 37 1
//i : 38 0.5
//i : 39 1
//i : 40 0
//i : 41 0.75
//i : 42 0.25
//i : 43 0.75
//i : 44 0.25
//i : 45 1
//i : 46 0
//i : 47 1