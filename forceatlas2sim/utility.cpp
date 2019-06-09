#include <spdlog/spdlog.h>

#include "utility.h"

void logAndThrow(std::string message)
{
	spdlog::error(message);
	throw std::runtime_error(message);
}
