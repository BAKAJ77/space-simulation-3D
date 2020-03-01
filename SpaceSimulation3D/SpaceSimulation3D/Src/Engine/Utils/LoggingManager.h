#pragma once
#include <string>

#define GLCheck(gl_func) gl_func; \
	Logging::CheckGLError(__FILE__, __LINE__)

#define OutputLog(msg, severity) Logging::HandleLogOutput(msg, severity)

namespace Logging
{
	enum class Severity
	{
		NOTIFICATION,
		WARNING,
		FATAL
	};

	extern void CheckGLError(const char* file, int line); // Checks for errors thrown by OpenGL
	extern void HandleLogOutput(const std::string& log_msg, Severity severity_level); // Outputs log to console/file (depending on build config)
}