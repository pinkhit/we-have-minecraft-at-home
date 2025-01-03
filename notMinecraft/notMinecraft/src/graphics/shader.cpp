#include "shader.h"

shader::shader(std::string vs, std::string fs):
    vertSourceFile(vs), fragSourceFile(fs)
{
    // check shader library for existing vs fs pairing
    for (int i = 0; i < shaderLib.size(); i++)
    {
        auto libPair = shaderLib[i];
        auto namePair = libPair.first;
        if (vs == namePair.first && fs == namePair.second)
        {
            this->progID = libPair.second;
            std::cout << "shader program with ID" << this->progID << "assigned from library" << std::endl;
            return;
        }

    }
    // not in library, new shader pair
    shaderSourceCode = readFile(vs, fs);
    if (shaderSourceCode)
    {
        progID = createShaderProgram(shaderSourceCode);
        std::cout << "shader program with ID " << progID << " created" << std::endl;

        // add pair to shader library
        auto nameP = std::make_pair(vs, fs);
        auto libP = std::make_pair(nameP, progID);
        shaderLib.push_back(libP);
        std::cout << "shader program with vs:" << vs << " and fs " << fs << " added to library" << std::endl;
    }
}

shader::~shader()
{
// why is this causing INVALID VALUE error ???
    //if (glIsProgram(progID)) 
    //    glDeleteProgram(progID);
    //delete shaderSourceCode;
}

unsigned int shader::getID() const
{
    return progID;
}

void shader::bind() const
{
    glUseProgram(progID);
}

void shader::unbind() const
{
    glUseProgram(0);
}



static unsigned int CreateShader(GLenum type, std::string& source, std::string& filename)
{
    if (source.empty() || filename.empty())
        return 0;

    unsigned int shader_id = glCreateShader(type);
    const char* source_ptr = source.c_str();
    glShaderSource(shader_id, 1, &source_ptr, nullptr);
    glCompileShader(shader_id);

    int success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

        char* error_message = static_cast<char*>(calloc(1, length + 1));
        if (!error_message) return 0;
        glGetShaderInfoLog(shader_id, length, &length, error_message);

        std::cout << "shader failed to compile" << std::endl;
        std::cout << error_message << std::endl;

        free(error_message);

        return 0;
    }

    return shader_id;
}

static unsigned int LinkShader(unsigned vertexShader, unsigned fragShader)
{
    if (!vertexShader || !fragShader) return 0;

    // link shaders to program
    unsigned int shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragShader);
    glLinkProgram(shaderProgramID);

    // error handling
    int success = 0;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        int length = 0;
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &length);

        // shader linking to program failed
        char* error_message = static_cast<char*>(calloc(1, length + 1));
        if (!error_message) return 0;
        glGetProgramInfoLog(shaderProgramID, length, &length, error_message);
        std::cout << "oh no my shaders didnt link to program!" << std::endl;

        free(error_message);

        return 0;
    }

    // delete shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgramID;
}

unsigned int shader::createShaderProgram(ShaderSources* shaderSource)
{
    unsigned int vertexShader = 0;
    unsigned int fragShader = 0;
    unsigned int geomShader = 0;

    if (!shaderSourceCode->vertexShader.empty())
    {
        vertexShader = CreateShader(GL_VERTEX_SHADER, shaderSourceCode->vertexShader, vertSourceFile);
        if (!vertexShader)
        {
            return 0;
        }
    }

    if (!shaderSourceCode->fragShader.empty())
    {
        fragShader = CreateShader(GL_FRAGMENT_SHADER, shaderSourceCode->fragShader, fragSourceFile);
        if (!fragShader)
        {
            glDeleteShader(vertexShader);
            return 0;
        }
    }

    unsigned int shaderProgramID = LinkShader(vertexShader, fragShader);

    return shaderProgramID;
}


static std::string ParseShader(std::string file)
{
    if (file.empty())
        return file;

    std::string shaderText;
    std::string line;

    std::ifstream fileStream(file);
    if (!fileStream)
    {
        return std::string();
    }

    while (true)
    {
        std::getline(fileStream, line);

        // read in shader
        shaderText.append(line);
        shaderText.append("\n");

        if (fileStream.eof())
            break;
    }

    if (fileStream.bad())
    {
        return std::string();
    }

    return shaderText;
}


ShaderSources* shader::readFile(std::string vertFile, std::string fragFile)
{
    ShaderSources* shaderSource = new ShaderSources();
    if (!shaderSource)
        return nullptr;

    if (!vertFile.empty())
        shaderSource->vertexShader = ParseShader(vertShaderPath + vertFile);

    if (!fragFile.empty())
        shaderSource->fragShader = ParseShader(fragShaderPath + fragFile);

    if (shaderSource->vertexShader.empty() ||
        shaderSource->fragShader.empty())
    {
        std::cout << "invalid shader filename" << std::endl;
        delete shaderSource;
        return nullptr;
    }

    return shaderSource;
}
void shader::setUniformMat4(const std::string& uniformName, glm::mat4 desiredMatrix) const
{
    // get the location of uniform
    int uniformLocation = glGetUniformLocation(progID, uniformName.c_str());
    // upload the matrix to the shader
    if (uniformLocation != -1)
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(desiredMatrix));
}
