#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath) 
	: m_filepath(filepath), m_rendererID(0)
{
    ShaderProgramSource shaderSrcCode = ParseShader(filepath);
    m_rendererID = createShader(shaderSrcCode.VertexSource, shaderSrcCode.FragmentSource);
    std::cout << "m_rendererID " << m_rendererID << std::endl;
}

Shader::Shader(const std::string& filepath, const std::string& geomFilepath)
    : m_filepath(filepath), m_rendererID(0), m_geomFilepath(geomFilepath)
{
    ShaderProgramSource shaderSrcCode = ParseShader(filepath, geomFilepath);
    m_rendererID = createShader(shaderSrcCode.VertexSource, shaderSrcCode.FragmentSource);
    std::cout << "m_rendererID " << m_rendererID << std::endl;
}

Shader::Shader()
{
   
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}

unsigned int Shader::getID()
{
    return m_rendererID;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    //std::cout << source << std::endl;
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
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                //set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str(), "" };
}

std::stringstream Shader::ParseGeometryShader(const std::string& geomFilepath)
{
    std::ifstream stream(geomFilepath);
    std::string line;
    std::stringstream src;
    while (getline(stream, line))
    {
        src << line << "\n";
    }
    return src;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath, const std::string& geomFilepath)
{
    ShaderProgramSource shaders = ParseShader(filepath); //should return vertex and fragment, and empty string placeholder for geom
    shaders.GeometrySource = ParseGeometryShader(geomFilepath).str();
    return shaders;
}
void Shader::bind() const
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::unbind() const
{
    GLCall(glUseProgram(0));
}

//SET UNIFORMS
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform3fv(const std::string& name, glm::vec3& vec)
{
    GLCall(glUniform3fv(getUniformLocation(name), 1, &vec[0]));
}

void Shader::setUniform3fv(const std::string& name, int num, glm::vec3 vec[])
{
    GLCall(glUniform3fv(getUniformLocation(name), num, glm::value_ptr(vec[0])));
}


void Shader::setUniformMat4f(const std::string& name, glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::setUniform4fv(const std::string& name, glm::vec4& vec)
{
    GLCall(glUniform4fv(getUniformLocation(name), 1, &vec[0]));
}

int Shader::getUniformLocation(const std::string & name)
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

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glAttachShader(program, gs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(gs);
    return program;
}
