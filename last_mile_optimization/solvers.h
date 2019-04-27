#pragma once
#include <string>
#include "abstract_solver.h"
#include "genetic_solver.h"

namespace solvers {
	using namespace std;
	using namespace data;

	template<int N>
	solver<N>* solver_factory(const problem<N> * prob, string solver_name) {
		if (solver_name == "genetic") return new genetic::genetic_solver<N>(prob);

		auto exp_str = solver_name + string("bad solver name: ");
		throw new std::exception(exp_str.c_str());
	}
}