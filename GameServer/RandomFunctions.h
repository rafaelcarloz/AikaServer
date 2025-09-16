#pragma once

#ifndef _RANDOM_FUNCTIONS_H_
#define _RANDOM_FUNCTIONS_H_

#include <stdint.h>
#include <random>

class RandomFunctions {
public:
	static uint32_t RandomRange(uint32_t rangeStart, uint32_t rangeEnd);
	static bool RandomPercentSelection(uint32_t percentChance);
};

#endif