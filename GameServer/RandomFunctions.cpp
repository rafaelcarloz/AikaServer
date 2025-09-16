#include "RandomFunctions.h"

using namespace std;

uint32_t RandomFunctions::RandomRange(uint32_t rangeStart, uint32_t rangeEnd) {
	random_device rd;

	mt19937 gen(rd());

	uniform_int_distribution<> dist(rangeStart, rangeEnd);

	return (uint32_t)dist(gen);
}

bool RandomFunctions::RandomPercentSelection(uint32_t percentChance) {
	return RandomFunctions::RandomRange(0, 99) < percentChance;
}