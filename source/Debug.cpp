#include "Debug.h"
#include <memory>

std::shared_ptr<spdlog::logger> Debug::mCoreLogger;
std::shared_ptr<spdlog::logger> Debug::mAppLogger;

void Debug::Initialize()
{
	spdlog::sink_ptr logSinkCore = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	spdlog::sink_ptr logSinkApp  = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	logSinkCore->set_pattern("%H:%M:%S|%n|%^%l: %v%$");
	logSinkApp->set_pattern("%H:%M:%S|%n|%^%l: %v%$");

	mCoreLogger = std::make_shared<spdlog::logger>("Engine", logSinkCore);
	spdlog::register_logger(mCoreLogger);

	mCoreLogger->set_level(spdlog::level::trace);
	mCoreLogger->flush_on(spdlog::level::trace);

	mAppLogger = std::make_shared<spdlog::logger>("Game", logSinkApp);
	spdlog::register_logger(mAppLogger);

	mCoreLogger->set_level(spdlog::level::trace);
	mCoreLogger->flush_on(spdlog::level::trace);

	mAppLogger->set_level(spdlog::level::trace);
	mAppLogger->flush_on(spdlog::level::trace);
}


void Debug::SetAppPattern(const char * pattern)
{
	mAppLogger->set_pattern(pattern);
}

void Debug::SetCorePattern(const char * pattern)
{
	mCoreLogger->set_pattern(pattern);
}

