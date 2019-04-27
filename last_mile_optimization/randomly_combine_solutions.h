#pragma once
#include "simple_random_generator.h"
#include "solution.h"

namespace solvers {
	namespace solution_interpolations {
		using namespace data;
		template<unsigned int N>
		static void combine_into(const problem<N>* prob, const solution<N>& old_s1, const solution<N>& old_s2, solution<N>& s) {
			random::simple_uniform_random_generator urand;

			auto& s1 = old_s1.get_routes();
			auto& s2 = old_s2.get_routes();
			int n1 = (int)s1.size(), n2 = (int)s2.size();
			for (int r_1 = 0, r_2 = 0;
				s.total_cars_in_use() <= prob->total_cars && s.get_score() < 0;
				r_1 = (r_1 + 1) % n1, r_2 = (r_2 + 1) % n2)
				if (urand.next(1)) s += s1[r_1];
				else s += s2[r_2];
			if (s.total_cars_in_use() > prob->total_cars || old_s1 < s)
				s = old_s1;
		}
	}
}