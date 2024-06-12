#pragma once


#include <string>
#include <fstream>


static const std::string baseShaderPath = "data/shaders/";
static const std::string vertShaderPath = "data/shaders/vert/";
static const std::string fragShaderPath = "data/shaders/frag/";

// struct to store shader code that is parsed in from shader file
struct ShaderSources
{
    std::string vertexShader; // source code for vertex shader to be used with glShaderSource
    std::string fragShader;   // source code for frag shader
};

class shader
{
public:
	shader(std::string vs, std::string fs);
	~shader();

	unsigned int getID() const;   // to get shader ID
	void bind() const;
	void unbind() const;

	// uniforms
	void setUniformMat4(const std::string& uniformName, glm::mat4 desiredMatrix) const;

private:
	// private variables
	unsigned int progID;
	std::string vertSourceFile;   // file path to vert shader
	std::string fragSourceFile;   // file path to frag shader
	ShaderSources* shaderSourceCode;    // store shader source code
	
	// private methods
	unsigned int createShaderProgram(ShaderSources* shaderSource); // returns ID to shader program
	ShaderSources* readFile(std::string vs, std::string fs);

};