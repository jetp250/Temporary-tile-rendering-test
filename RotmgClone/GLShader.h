#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

namespace glh {

	class shader
	{
	public:
		shader();
		shader(const char* vertexPath, const char* fragmentPath);

		void destroy() const;

		bool isValid() const;
		void bind() const;

		void setBool(GLint, GLboolean) const;
		void setInt(GLint, GLint) const;
		void setFloat(GLint, GLfloat) const;
		void setMat4(GLint, const glm::mat4&) const;
		void setVec2(GLint, GLfloat, GLfloat) const;
		void setVec2(GLint, const glm::vec2&) const;
		void setVec3(GLint, GLfloat, GLfloat, GLfloat) const;
		void setVec3(GLint, const glm::vec3&) const;
		void setVec4(GLint, GLfloat, GLfloat, GLfloat, GLfloat) const;
		void setVec4(GLint, const glm::vec4&) const;

		void setBool(const std::string&, GLboolean) const;
		void setInt(const std::string&, GLint) const;
		void setFloat(const std::string&, GLfloat) const;
		void setMat4(const std::string&, const glm::mat4&) const;
		void setVec2(const std::string&, GLfloat, GLfloat) const;
		void setVec2(const std::string&, const glm::vec2&) const;
		void setVec3(const std::string&, GLfloat, GLfloat, GLfloat) const;
		void setVec3(const std::string&, const glm::vec3&) const;
		void setVec4(const std::string&, GLfloat, GLfloat, GLfloat, GLfloat) const;
		void setVec4(const std::string&, const glm::vec4&) const;

		GLint getUniformLocation(const std::string&) const;

	protected:
		bool m_valid = false;
		GLuint m_id = 0;

	private:
#ifdef _DEBUG
		std::string m_vertPath;
		std::string m_fragPath;
#endif
	};

}