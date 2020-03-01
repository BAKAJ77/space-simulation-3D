#include "LoggingManager.h"

#include <glad/glad.h>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

namespace Logging
{
	void CheckGLError(const char* file, int line)
	{
		std::stringstream info_stream;
		info_stream << " [" << file << " (";
		info_stream << line << ")]";

		while (GLenum error_enum = glGetError() != GL_NO_ERROR)
		{
			switch (error_enum)
			{
			case GL_INVALID_ENUM:
				HandleLogOutput("OpenGL Error: INVALID_ENUM" + info_stream.str(), Severity::FATAL);
				break;
			case GL_INVALID_VALUE:
				HandleLogOutput("OpenGL Error: INVALID VALUE" + info_stream.str(), Severity::FATAL);
				break;
			case GL_INVALID_OPERATION:
				HandleLogOutput("OpenGL Error: INVALID OPERATION" + info_stream.str(), Severity::FATAL);
				break;
			case GL_OUT_OF_MEMORY:
				HandleLogOutput("OpenGL Error: OUT OF MEMORY" + info_stream.str(), Severity::FATAL);
				break;
			}
		}
	}

	void HandleLogOutput(const std::string& log_msg, Severity severity_level)
	{
	#ifdef _DEBUG
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (severity_level)
		{
		case Severity::NOTIFICATION:
			SetConsoleTextAttribute(console_handle, 11);
			std::cout << "Notification: " << log_msg << std::endl;
			break;
		case Severity::WARNING:
			SetConsoleTextAttribute(console_handle, 14);
			std::cout << "Warning: " << log_msg << std::endl;
			break;
		case Severity::FATAL:
			SetConsoleTextAttribute(console_handle, 12);
			std::cout << "Notification: " << log_msg << std::endl;
			assert(false);
			break;
		}
	#else
		static bool cleared = false;
		std::ofstream log_file;

		if (cleared)
			log_file.open("Resources/Logs/runtime_log.txt", std::ios::app);
		else
		{
			log_file.open("Resources/Logs/runtime_log.txt", std::ios::trunc);
			cleared = true;
		}
		
		switch (severity_level)
		{
		case Severity::NOTIFICATION:
			log_file << "Notification: " << log_msg << std::endl;
			break;
		case Severity::WARNING:
			log_file << "Warning: " << log_msg << std::endl;
			break;
		case Severity::FATAL:
			log_file << "Notification: " << log_msg << std::endl;
			break;
		}
	#endif
	}
}