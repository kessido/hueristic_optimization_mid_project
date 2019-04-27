// last_mile_optimization.cpp : This file contains the 'main' function. Program execution begins and ends there. //
#include <iostream>
#include "parsed_problem.h"
#include "timer.h"
#include "parse_command_line_arguments.h"
#include "brute_force_solver_parameters.h"
#include "genetic_solver_parameters.h"
#include "problem_type_parameters.h"
#include "problem.h"
#include "solution_saver.h"
#include "solvers.h"

template<unsigned int N>
void solve(const parse_cmd::cmd_args& arg, const data::parsed_problem& parsed_prob) {
	if (N != parsed_prob.no_stations) {
		solve<N - 1>(arg, parsed_prob);
		return;
	}

	timer::timer t;

	data::problem<N> prob(parsed_prob);

	auto solution_saver = solver_helper::solution_saver<N>(arg.outfile);
	auto solver = solvers::solver_factory(&prob, arg.solver_name);
	solution_saver.save(solver->get_best_solution(), t.get_seconds());
	do {
		if (solver->solve_step()) solution_saver.save(solver->get_best_solution(), t.get_seconds());
	} while (!solver->finished() && (arg.runtime < 0 || t.get_seconds() < arg.runtime));
	delete solver;

	std::cout << "elapsed_time:\t\t" << t.get_seconds() << std::endl;
	std::cout << "best_score_found:\t" << solution_saver.get_score() << std::endl;
}

template<>void solve<0>(const parse_cmd::cmd_args & arg, const data::parsed_problem & parsed_prob) {} // recursion end.


constexpr unsigned int MAX_N = 15;

int main(int argc, char* argv[])
{
	parse_cmd::cmd_args arg;
	if (parse_cmd::try_parse_command_line_argument(argc, argv, arg)) {
		auto parsed_prob = data::parsed_problem::from_file(arg.infile);
		if (parsed_prob.no_stations > MAX_N)
			throw std::string("Doesn't support more than ") + std::to_string(MAX_N)
			+ std::string(" station (needs to be recompiled for that...).");
		solve<MAX_N>(arg, parsed_prob);
	}
}