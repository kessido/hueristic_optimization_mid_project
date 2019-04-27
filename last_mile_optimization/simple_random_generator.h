#pragma once
#include <random>
#include <mutex>

namespace random {
	class simple_random_generator {
	protected:
		std::random_device rd;
		std::mt19937 gen;
	public:
		simple_random_generator() :rd(), gen(rd()) {}
		~simple_random_generator() = default;
		simple_random_generator(const simple_random_generator& o) = default;
		simple_random_generator(simple_random_generator&& o) = default;
		simple_random_generator& operator=(simple_random_generator&& o) = default;
		simple_random_generator& operator=(const simple_random_generator& o) = default;
	};

	class simple_uniform_random_generator :public simple_random_generator
	{
	public:
		// return [a,b]
		int next(int a, int b) { return std::uniform_int_distribution<>(a, b)(gen); }
		// return [0,b]
		int next(int b) { return next(0, b); }
	};

	class simple_normal_random_generator :public simple_random_generator
	{
	public:
		// return [a,b]
		double next(float mean, float std) { return std::normal_distribution<>(mean, std)(gen); }
	};
}

