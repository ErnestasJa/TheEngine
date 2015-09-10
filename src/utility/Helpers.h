#ifndef _HELPERS_H_INCLUDED_
#define _HELPERS_H_INCLUDED_

#include <sstream>

#define loop(var,count) for(uint32_t (var) = 0; (var) < (count); (var)++)
#define loopi(count) for(int32_t i = 0; i < (count); i++)
#define loopr(var,start,count) for(uint32_t (var) = (start); (var) < (count); (var)++)
#define loopxyz(cx,cy,cz) loop(z,(cz)) loop(y,(cy)) loop(x,(cx))
#define comp(vec) (vec).x, (vec).y, (vec).z
#define ROUNDING_ERROR 0.00001f

namespace helpers
{
	uint32_t read(const std::string & file, char *& buf);

	struct u8vec4
	{
		uint8_t v[4];
	};

	template <typename T> inline std::string to_str(const T& t);
	template <typename T> inline std::wstring to_wstr(const T& t);
	template <typename T> inline T limit(T val, T min, T max);
	const inline glm::vec4 color255(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	inline void invert(glm::vec3 & vec);
	template <class T> inline bool equals(const T & v1, const T & v2);
	inline float coslerp(float y1, float y2, float mu);
	inline glm::vec3 hsv2rgb(float h, float s, float v);

	inline glm::vec3 rgb2hsv(float r, float g, float b);

	inline int wtoi(const wchar_t *str)
	{
		return (int)wcstol(str, 0, 10);
	}

#include "Helpers.inl"
}

template <class T>
inline std::shared_ptr<T> share(T* t)
{
	return std::shared_ptr<T>(t);
}

#define _DEBUG_OGL
#ifdef _DEBUG_OGL

inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::string errorName = "";
		switch (err)
		{
		case GL_INVALID_ENUM:
			errorName = "InvalidEnum";
			break;
		case GL_INVALID_VALUE:
			errorName = "InvalidValue";
			break;
		case GL_INVALID_OPERATION:
			errorName = "InvalidOperation";
			break;
		case GL_STACK_OVERFLOW:
			errorName = "StackOverflow";
			break;
		case GL_STACK_UNDERFLOW:
			errorName = "StackUnderflow";
			break;
		case GL_OUT_OF_MEMORY:
			errorName = "OutOfMemory";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorName = "InvalidFramebufferOperation";
			break;
		default:
			errorName = "UnspecifiedError";
			break;
		}
		printf("OpenGL error %s(%x), at %s:%i - for %s\n", errorName.c_str(), err, fname, line, stmt);
		abort();
	}
}

#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)

inline void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_LOW || severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::cout << "---------------------opengl-callback-start------------" << std::endl;
		std::cout << "message: " << message << std::endl;
		std::cout << "type: ";
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "ERROR";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "DEPRECATED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "UNDEFINED_BEHAVIOR";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "PORTABILITY";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "PERFORMANCE";
			break;
		case GL_DEBUG_TYPE_OTHER:
			std::cout << "OTHER";
			break;
		}
		std::cout << std::endl;

		std::cout << "id: " << id << std::endl;
		std::cout << "severity: ";
		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "HIGH";
			break;
		}
		std::cout << std::endl;
		std::cout << "---------------------opengl-callback-end--------------" << std::endl;
	}
}
#else
#define GL_CHECK(stmt) stmt
#endif

#endif // _HELPERS_H_INCLUDED_
