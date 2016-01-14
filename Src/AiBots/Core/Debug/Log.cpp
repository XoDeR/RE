// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Debug/Log.h"
#include "Core/Strings/StringUtils.h"
#include "Core/Strings/StringStream.h"
#include "Core/Os/Os.h"
#if RIO_USE_DEBUG_CONSOLE
#include "Core/Debug/ConsoleServer.h"
#endif // RIO_USE_DEBUG_CONSOLE

#if RIO_DEBUG

namespace Rio
{
	namespace LogInternalFn
	{
#if RIO_USE_DEBUG_CONSOLE
		static StringStream& sanitize(StringStream& ss, const char* msg)
		{
			using namespace StringStreamFn;
			const char* ch = msg;
			for (; *ch; ch++)
			{
				if (*ch == '"')
				{
					ss << "\\";
				}
				ss << *ch;
			}

			return ss;
		}

		static void logToConsole(const char* msg, LogSeverity::Enum severity)
		{
			using namespace StringStreamFn;
			static const char* stt[] = 
			{ 
				"info", 
				"warning", 
				"error", 
				"debug" 
			};

			// Build json message
			TempAllocator2048 alloc;
			StringStream json(alloc);

			json << "{\"type\":\"message\",";
			json << "\"severity\":\"" << stt[severity] << "\",";
			json << "\"message\":\""; 
			sanitize(json, msg) << "\"}";

			ConsoleServerFn::getDebugConsoleServer().send(toCStr(json));
		}
#endif // RIO_USE_DEBUG_CONSOLE

		void logx(LogSeverity::Enum sev, const char* msg, va_list args)
		{
			char buf[2048];
			int len = vsnPrintf(buf, sizeof(buf), msg, args);
			if (len > (int)sizeof(buf))
			{
				len = sizeof(buf) - 1;
			}

			buf[len] = '\0';
#if RIO_USE_DEBUG_CONSOLE
			logToConsole(buf, sev);
#endif // RIO_USE_DEBUG_CONSOLE
			Os::log(buf);
		}

		void logx(LogSeverity::Enum sev, const char* msg, ...)
		{
			va_list args;
			va_start(args, msg);
			logx(sev, msg, args);
			va_end(args);
		}
	} // namespace LogInternalFn

} // namespace Rio

#endif // RIO_DEBUG
