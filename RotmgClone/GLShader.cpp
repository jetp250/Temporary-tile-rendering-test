#include "GLShader.h"

#include <algorithm>

/* SHADER PROGRAM CREATION */

static std::string readShader(const char* path, bool& success_out) {
	if (!success_out)
		return "";
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
	try {
		shaderFile.open(path);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();
		
		std::string toString = shaderStream.str();
#ifdef _DEBUG
		std::cout << "Source length for path \"" << path << "\": " << toString.size() << std::endl;
#endif
		return toString;
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Failed to read shader source (does the file \"" << path << "\" exist?)" << std::endl;
		std::cout << e.code().message() << std::endl;
		
		success_out = false;

		return "";
	}
}

static GLuint compileShader(GLenum type, const char* source, bool& success_out) {
	if (!success_out)
		return 0u;
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		GLchar log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
	
		std::cout << "Failed to compile shader! Enable debugging for more information" << std::endl;
		std::cout << log << std::endl;

		success_out = false;
	}

	return shader;
}

static GLuint createshader(GLenum type, const char* srcPath, bool& success_out) {
	std::string source = readShader(srcPath, success_out);
	return compileShader(type, source.c_str(), success_out);
}

static GLuint createProgram(const char* vertexPath, const char* fragmentPath, bool& success_out) {
	success_out = true;

	GLuint vShader = createshader(GL_VERTEX_SHADER, vertexPath, success_out);
	GLuint fShader = createshader(GL_FRAGMENT_SHADER, fragmentPath, success_out);
	if (!success_out)
		return GLuint(-1);

	GLuint program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar log[512];
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cout << "Failed to link shader! Enable debugging for more information" << std::endl;
		std::cout << log << std::endl;
		
		success_out = false;
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
}

namespace glh {
	/* Shader class */

	static GLuint _activeShader;

	shader::shader()
		: m_id(GLuint(-1)) // invalid
	{
	}

	shader::shader(const char* vertexPath, const char* fragmentPath)
		: m_id(createProgram(vertexPath, fragmentPath, m_valid))
#ifdef _DEBUG
		, m_vertPath(vertexPath),
		m_fragPath(fragmentPath)
#endif
	{
#ifdef _DEBUG
		std::cout << "Loaded shader, id: " << m_id << std::endl;
#endif
	}

	void shader::destroy() const {
		glDeleteProgram(m_id);
	}

	bool shader::isValid() const {
		return m_valid;
	}

	void shader::bind() const {
		if (m_id != _activeShader && isValid()) {
			_activeShader = m_id;
			glUseProgram(m_id);
		}
#ifdef _DEBUG
		else if (!isValid()) {
			printf("Shader %d is not valid!\n", m_id);
		}
#endif
	}

	GLint shader::getUniformLocation(const std::string& name) const {
		GLint loc = glGetUniformLocation(m_id, name.c_str());
		if (loc == -1) {
#ifdef _DEBUG
			std::cout << "No uniform location found for '" << name << "' for path \"" << m_vertPath << "\"" << std::flush << std::endl;
#endif
		}
		return loc;
	}

	void shader::setBool(GLint location, GLboolean value) const {
		setInt(location, value);
	}

	void shader::setInt(GLint location, GLint value) const {
		glUniform1i(location, value);
	}

	void shader::setFloat(GLint location, GLfloat value) const {
		glUniform1f(location, value);
	}

	void shader::setMat4(GLint location, const glm::mat4& value) const {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void shader::setVec3(GLint location, GLfloat x, GLfloat y, GLfloat z) const {
		glUniform3f(location, x, y, z);
	}

	void shader::setVec2(GLint location, const glm::vec2& vec) const {
		setVec2(location, vec.x, vec.y);
	}

	void shader::setVec2(GLint location, GLfloat x, GLfloat y) const {
		glUniform2f(location, x, y);
	}

	void shader::setVec3(GLint location, const glm::vec3& vec) const {
		setVec3(location, vec.x, vec.y, vec.z);
	}

	void shader::setVec4(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
		glUniform4f(location, x, y, z, w);
	}

	void shader::setVec4(GLint location, const glm::vec4& vec) const {
		setVec4(location, vec.x, vec.y, vec.z, vec.w);
	}

	void shader::setBool(const std::string& name, GLboolean value) const {
		setBool(getUniformLocation(name), value);
	}

	void shader::setInt(const std::string& name, GLint value) const {
		setInt(getUniformLocation(name), value);
	}

	void shader::setFloat(const std::string& name, GLfloat value) const {
		setFloat(getUniformLocation(name), value);
	}

	void shader::setMat4(const std::string& name, const glm::mat4& value) const {
		setMat4(getUniformLocation(name), value);
	}

	void shader::setVec2(const std::string& name, GLfloat x, GLfloat y) const {
		setVec2(getUniformLocation(name), x, y);
	}

	void shader::setVec2(const std::string& name, const glm::vec2& vec) const {
		setVec2(getUniformLocation(name), vec);
	}

	void shader::setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const {
		setVec3(getUniformLocation(name), x, y, z);
	}

	void shader::setVec3(const std::string& name, const glm::vec3& vec) const {
		setVec3(name, vec.x, vec.y, vec.z);
	}

	void shader::setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const {
		setVec4(getUniformLocation(name), x, y, z, w);
	}

	void shader::setVec4(const std::string& name, const glm::vec4& vec) const {
		setVec4(name, vec.x, vec.y, vec.z, vec.w);
	}


}