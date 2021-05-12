#include "ComputeShader.h"
#include <fstream>
#include <string>
#include <sstream>

/*
@author = Lauren Cole
class to represent/wrap OpenGL compute shaders
*/

ComputeShader::ComputeShader(std::string& filepath) : m_filepath(filepath), m_rendererID(0)
{
    std::string shaderSrcCode = parseShader(filepath);
    m_rendererID = createShader(shaderSrcCode);

}

unsigned int ComputeShader::getShaderID()
{
    return m_rendererID;
}

unsigned int ComputeShader::compileShader(const std::string& source)
{
    unsigned int id = glCreateShader(GL_COMPUTE_SHADER);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    //error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile Compute shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

std::string ComputeShader::parseShader(std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss;

    while (getline(stream, line))
    {
        ss << line << "\n";
    }
    return ss.str();
}

unsigned int ComputeShader::createShader(const std::string& shader)
{
    unsigned int program = glCreateProgram();
    unsigned int cs = compileShader(shader);
    GLCall(glAttachShader(program, cs));
    GLCall(glLinkProgram(program));
    int rvalue;
    glGetProgramiv(program, GL_LINK_STATUS, &rvalue);
    if (!rvalue)
    {
        int length;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(program, length, &length, message);
        std::cout << "compute shader linking error!" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall(glValidateProgram(program));
    glDeleteShader(cs);
    return program;
}

void ComputeShader::dispatch(int x, int y, int z)
/*
activates the shader, binds the vbo and dispatches the shader
*/
{
    use();
    GLCall(glDispatchCompute(x, y, z));
}

void ComputeShader::use()
{
    GLCall(glUseProgram(m_rendererID));
}

void ComputeShader::setUniform3fv(const std::string& name, int num, glm::vec3 vec[])
{
    use();
    GLCall(glUniform3fv(getUniformLocation(name), num, glm::value_ptr(vec[0])));
}

void ComputeShader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void ComputeShader::setUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(getUniformLocation(name), value));
}


int ComputeShader::getUniformLocation(const std::string& name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
        return m_uniformLocationCache[name];
    }
    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform " << name << " doesn't exist." << std::endl;
    }
    else
    {
        //   std::cout << name << std::endl;
        m_uniformLocationCache[name] = location;
    }
    return location;
}