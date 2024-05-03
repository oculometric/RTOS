#pragma once

#include <vector.h>

// my own really terrible floating point random generator, which can be seedless (if you use the _random function)

using namespace nov::vector;

static FVector3 a = FVector3{ -53.44637, 1.334546, -22.8456 };
static FVector3 b = FVector3{ 3.576754, -69.42134, 15.2298 };
static FVector3 c = FVector3{ 0.8878415, 13.37688, 1.33867 };

static float f = 0.3659397f;

static float _random(float i)
{
    FVector3 { i, i, i };
    FVector3 t = (a+t)*b;
    t = FVector3{ modf(t.x, c.x), modf(t.y, c.y), modf(t.z, c.z) };
    return modf(t.x+t.y+t.z, 1.0f);
}

float randomFloat()
{
    f = _random(f);
    return f;
}

// this code is based very heavily on the code by David Blackman and Sebastiano Vigna here: https://prng.di.unimi.it/xoshiro128starstar.c

static uint32_t state[4] = {0x35774fa8};

static inline uint32_t rotl(uint32_t x, int k)
{
	return (x << k) | (x >> (32-k));
}

inline uint32_t random()
{
    uint32_t result = rotl(state[1]*5, 7)*9;
	uint32_t t = state[1] << 9;

	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];
	state[2] ^= t;
	state[3] = rotl(state[3], 11);

	return result;
}

inline bool randomBool() { return random() & (1 << 10); }