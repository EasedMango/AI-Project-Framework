#pragma once
#include <source_location>
#include <string>

enum class MessageType : unsigned short {
	TYPE_NONE = 0,
	TYPE_FATAL_ERROR,
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_TRACE,
	TYPE_INFO
};
class Debug
{
private:
	inline static std::string GetFilename(const std::string& str = "Default") {
		//size_t slash = str.find_last_of("\\/");

		//if (slash == std::string::npos)
		//	return str;
		static std::string filename = str;
		if (filename == "Default")
			filename = str;//str.substr(str.find_last_of("\\/") + 1);

		return filename;
	}
	//	inline static std::string logFileName;
	static void Log(MessageType type, std::string message, std::source_location source);

public:
	Debug(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug& operator=(Debug&&) = delete;

	Debug() = delete;

	static void DebugInit(const std::string& fileName_);

	static void Info(const std::string& message,
		const std::source_location location =
		std::source_location::current());

	static void Error(const std::string& message,
		const std::source_location location =
		std::source_location::current());

	static void Warning(const std::string& message,
		const std::source_location location =
		std::source_location::current());

	static void FatalError(const std::string& message,
		const std::source_location location =
		std::source_location::current());
	static void Trace(const std::string& message,
		const std::source_location location =
		std::source_location::current());



};


