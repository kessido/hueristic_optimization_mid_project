#pragma once
#include <cassert>
#include "solution.h"

namespace solvers {
	using namespace data;

	template<int N>
	class solver {
	protected:
		const problem<N> * prob;
		solution<N>* best_solution = nullptr;
		bool update_best_solution(const solution<N>& s) {
			if (best_solution == nullptr || best_solution->get_score() > s.get_score()) {
				best_solution = new solution<N>(s);
				return true;
			}
			return false;
		}
	public:
		solver(const problem<N>* prob) : prob(prob) {}
		~solver() { delete best_solution; }
		const solution<N>& get_best_solution() const {
			assert(best_solution != nullptr);
			return *best_solution;
		}
		virtual bool solve_step() = 0;
		virtual bool finished() const = 0;
	};
}