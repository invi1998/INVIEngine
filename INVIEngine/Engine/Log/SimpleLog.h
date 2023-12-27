#pragma once

#if defined(_WIN32)

#include <simple_library/simple_library.h>

#include "LogEditor/LogObject/LogSystem.h"

#define ENGINE_LOG(format, ...)				log_log(format, __VA_ARGS__);\
FLogSystem::GetLog()->AddLog(format, __VA_ARGS__);

#define ENGINE_LOG_SUCCESS(format, ...)		log_success(format, __VA_ARGS__);\
FLogSystem::GetLog()->AddSuccess(format, __VA_ARGS__);

#define ENGINE_LOG_ERROR(format, ...)		log_error(format, __VA_ARGS__);\
FLogSystem::GetLog()->AddError(format, __VA_ARGS__);

#define ENGINE_LOG_WARNING(format, ...)		log_warning(format, __VA_ARGS__);\
FLogSystem::GetLog()->AddWarning(format, __VA_ARGS__);

#elif 0

#define ENGINE_LOG(format, ...)
#define ENGINE_LOG_SUCCESS(format, ...)
#define ENGINE_LOG_ERROR(format, ...)
#define ENGINE_LOG_WARNING(format, ...)

#endif

