#pragma once

#include <vector.h>

// my own really terrible floating point random generator, which can be seedless (if you use the _random function)

static nov_fvector3 a = nov_fvector3{ -53.44637, 1.334546, -22.8456 };
static nov_fvector3 b = nov_fvector3{ 3.576754, -69.42134, 15.2298 };
static nov_fvector3 c = nov_fvector3{ 0.8878415, 13.37688, 1.33867 };

static float f = 0.3659397f;

static float _random(float i)
{
    nov_fvector3 { i, i, i };
    nov_fvector3 t = (a+t)*b;
    t = nov_fvector3{ modf(t.x, c.x), modf(t.y, c.y), modf(t.z, c.z) };
    return modf(t.x+t.y+t.z, 1.0f);
}

float random_float()
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

uint32_t random_uint32_t()
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