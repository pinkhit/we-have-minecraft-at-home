
#include <src/FastNoiseLite.h>

#include "game/block.h"
#include "graphics/cubeMesh.h"
#include "game/player.h"
#include "graphics/camera.h"
#include "game/chunk.h"

static float vpW = 800;
static float vpH = 600;

bool firstMouse = true;
static float lastX = 0.0f;
static float lastY = 0.0f;

static float numX = 16.0f;
static float numY = 16.0f;
static float numZ = 16.0f;
static int numBlocks = 0;

static camera cam;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

void CallbackGLDebugMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

// handles window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	vpW = static_cast<float>(width);
	vpH = static_cast<float>(height);
}

// mouse look
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xDelta = lastX - xpos; 
	float yDelta = lastY - ypos;

	 //check margin: given margin to be 20.0 pixels
	if (xpos <= 20.0f)
		cam.mouse_rightEdge = true;
	else if (xpos >= vpW - 20.0f)
		cam.mouse_leftEdge = true;
	else
	{
		cam.mouse_rightEdge = false;
		cam.mouse_leftEdge = false;
	}

	if (ypos <= 20.0f)
		cam.mouse_TopEdge = true;
	else if (ypos >= vpH - 20.0f)
		cam.mouse_bottEdge = true;
	else
	{
		cam.mouse_bottEdge = false;
		cam.mouse_TopEdge = false;
	}


	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.001f;
	xDelta *= sensitivity;
	yDelta *= sensitivity;

	cam.updateLook(xDelta, yDelta);
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

	// surface layer
	std::vector<block> fakeChunk;
	int xWidth = 64, zLength = 64;
	// noise generation
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	int noiseSize = xWidth * zLength;
	std::vector<float> surfaceHeights(noiseSize);
	int index = 0;
	for (int y = 0; y < xWidth; y++)
		for (int x = 0; x < zLength; x++)
			surfaceHeights[index++] = noise.GetNoise((float)x, (float)y);
	// create blocks
	//index = 0;
	//for (int x = 0; x < xWidth; x++)
	//	for (int z = 0; z < zLength; z++)
	//	{
	//		block block = block::block("grass");
	//		block.setPos(static_cast<float>(x), surfaceHeights[index++] * 30.0f, static_cast<float>(z)); // apply sca
	//		fakeChunk.push_back(block);
	//		numBlocks++;
	//	}

	chunk chunk = chunk::chunk();
	chunk.generateTerrain(surfaceHeights);

	cam = camera();

	// lower left corner is 0,0
	// todo: screen resizing callback for camera
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// hide cursor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);



	// fps counting-------------
	double frameStart = glfwGetTime();
	float dt = 0.0f;
	int frames = 0;
	// --------------------------
// main loop: update & render
	while (!glfwWindowShouldClose(window))
	{
		// fps counting---------------------	
		double frameEnd = glfwGetTime();
		dt = static_cast<float>(frameEnd - frameStart);
		frames++;
		if (dt >= 1.0f) 
		{ 
			// print and reset timer every 1s
			float frameTime = 1000.0f / static_cast<float>(frames);
			printf("%f ms/frame -- ", frameTime);
			printf("fps: %f\n", 1000.0f / frameTime);
			frames = 0;
			frameStart += 1.0f;
		}
		//-------------------------------------

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

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
			

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);


		glm::mat4 view = cam.getViewMat();
		glm::mat4 proj = cam.getProjMat();
		//for (float i = 0.0f; i < numBlocks; i++)
		//{
		//	//blocks[i].render(proj, view, false);
		//	//wireFrames[i].render(proj, view, true);
		//	fakeChunk[i].render(proj, view, false);
		//}
		chunk.render(proj, view);

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