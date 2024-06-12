#include <includes/glad/glad.h>
#include <includes/GLFW/glfw3.h>

#include <iostream>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/camera.h"
#include "graphics/cubeMesh.h"


static float vpW = 800.0f;
static float vpH = 600.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void CallbackGLDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

// handles window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	vpW = width;
	vpH = height;
}

int main()
{
// init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    
	// for apple
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "notMinecraft", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// make window context the main context on current thread
	glfwMakeContextCurrent(window);
    
	// init glad: load openGL fn ptrs 
	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(CallbackGLDebugMessage, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	// build shader program
	//shader shaderProg = shader("basic.vert", "basic.frag");
	shader shaderProg = shader("textured.vert", "textured.frag");
	texture ourTex = texture();
	ourTex.load("mbrot.png");
	
	camera cam = camera();

	cubeMesh cube = cubeMesh();

	// lower left corner is 0,0
	// todo: screen resizing callback for camera
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
// update & render
	// wireframe 
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	while (!glfwWindowShouldClose(window))
	{
		// todo: input callback for mouse & kb
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.forward(0.1f);	
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.forward(-0.1f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.right(-0.1f);	
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.right(0.1f);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cam.up(-0.1f);	
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cam.up(0.1f);

		cam.update(vpW, vpH);
		shaderProg.bind();
		glm::mat4 proj = cam.getProjMat();
		shaderProg.setUniformMat4("projection", proj);
		glm::mat4 view = cam.getViewMat();
		shaderProg.setUniformMat4("view", view);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ourTex.bind();
		glEnable(GL_DEPTH_TEST);


		glBindVertexArray(cube.getVAO()); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, cube.numVerts());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

// exit
	glfwTerminate();
	return 0;

}


void APIENTRY CallbackGLDebugMessage(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}