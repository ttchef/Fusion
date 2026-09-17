
#pragma once

#include <volk.h>

#define VK_CHECK(expr) \
	do { \
		VkResult result = expr; \
		if (result != VK_SUCCESS)\
		{\
			throw std::runtime_error("Expression: '" + std::string(#expr) + "' failed with error code: " + std::string(utils::string_VkResult(result)));\
		}\
	} while(false)
