#include "Debug.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <source_location>
#include <chrono>
void Debug::Log(MessageType type, std::string message, std::source_location source)
{
	std::ofstream out;
	std::string msg;
	out.open(GetFilename(), std::ios::out | std::ios::app);
	if (type == MessageType::TYPE_INFO) {
		msg = message;
	}
	else {
		msg = message + " in file: " + source.file_name() + " on line: " + std::to_string(source.line());
	}
#ifdef _DEBUG
	std::cout << msg << std::endl;
#endif
	out << msg << std::endl;
	out.flush();
	out.close();
}

void Debug::DebugInit(const std::string& fileName_)
{
	GetFilename(fileName_);
	std::ofstream out;
	out.open(GetFilename(), std::ios::out);

	char str[26]; /// get the time and date
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t result = std::chrono::system_clock::to_time_t(p);
	ctime_s(str, sizeof str, &result); /// Since C11
	out << str;

	out.close();
}

void Debug::Info(const std::string& message, const std::source_location location)
{
	Log(MessageType::TYPE_INFO, message, location);
}

void Debug::Error(const std::string& message, const std::source_location location)
{
	Log(MessageType::TYPE_ERROR, message, location);
}

void Debug::Warning(const std::string& message, const std::source_location location)
{
	Log(MessageType::TYPE_WARNING, message, location);
}

void Debug::FatalError(const std::string& message, const std::source_location location)
{
	Log(MessageType::TYPE_ERROR, message, location);
	//throw std::exception(message.c_str());
	throw std::runtime_error(message.c_str());
}

void Debug::Trace(const std::string& message, const std::source_location location)
{
	Log(MessageType::TYPE_ERROR, message, location);
}
