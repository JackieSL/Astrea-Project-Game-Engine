#pragma once
#ifdef _WIN64

#elif defined(_WIN32)

#else
#error "Platform is not supported."
#endif

#include <loguru.hpp>

bool initLogging(int argc, char* argv[])
{
	LOG_IF_F(ERROR, loguru::add_file("../logs/dev_log.txt", loguru::Append, loguru::Verbosity_MAX) == false, "Failed to create Log file");
	loguru::g_preamble_date = false;

	loguru::init(argc, argv);
	return true;
}

#ifdef _DEBUG
#undef NDEBUG
#endif

#ifdef NDEBUG
const bool enableValidationLayers = false;
#define DBG
#else
const bool enableValidationLayers = true;
#define DBG LOG_SCOPE_FUNCTION(INFO);
#endif
const int WIDTH = 800;
const int HEIGHT = 600;






