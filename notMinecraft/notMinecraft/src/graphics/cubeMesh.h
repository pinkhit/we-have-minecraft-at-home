#pragma once


class cubeMesh
{
public:
	cubeMesh();
	~cubeMesh();

	unsigned int getVAO();
	GLsizei numVerts();

private:
	unsigned int vboID;
	unsigned int vaoID;
};