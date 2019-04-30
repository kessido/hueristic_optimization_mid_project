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
#include "statistics_helpers.h"

parse_cmd::cmd_args arg;
data::parsed_problem parsed_prob;

template<unsigned int N>
statistics::statistics solve_helper() {
	timer::timer t;

	data::problem<N> prob(parsed_prob);

	auto solution_saver = solver_helper::solution_saver<N>(arg.outfile);
	auto solver = solvers::solver_factory(&prob, arg.solver_name);
	auto number_of_generation = 1;
	auto get_stats = [&]() { return statistics::statistics{ t.get_seconds() ,solution_saver.get_score(),scoring::scoring_statistics::number_of_solution_checked ,number_of_generation }; };

	solution_saver.save(solver->get_best_solution(), get_stats());
	do {
		if (solver->solve_step())
			solution_saver.save(solver->get_best_solution(), get_stats());
		number_of_generation++;
	} while (!solver->finished() && (arg.runtime < 0 || t.get_seconds() < arg.runtime));
	delete solver;

	return get_stats();
}


#define solve_n_1(x) if (x == parsed_prob.no_stations) return solve_helper<x>();
#define solve_n_2(x) solve_n_1(x) solve_n_1(x+1)
#define solve_n_4(x) solve_n_2(x) solve_n_2(x+2)  
#define solve_n_8(x) solve_n_4(x) solve_n_4(x+4)  
#define solve_n_16(x) solve_n_8(x) solve_n_8(x+8)
#define solve_n_32(x) solve_n_16(x) solve_n_16(x+16) 
#define solve_n_64(x) solve_n_32(x) solve_n_32(x+32)  
#define solve_n_128(x) solve_n_64(x) solve_n_64(x+64)  
#define MAX_N 128

statistics::statistics solve() {
	solve_n_16(1);
}

int main(int argc, char* argv[])
{
	if (parse_cmd::try_parse_command_line_argument(argc, argv, arg)) {
		parsed_prob = data::parsed_problem::from_file(arg.infile);
		if (parsed_prob.no_stations > MAX_N)
			throw std::string("Doesn't support more than ") + std::to_string(MAX_N)
			+ std::string(" station (needs to be recompiled for that...).");

		auto stat = solve();
		std::cout << "elapsed_time:\t\t\t" << stat.duration << std::endl;
		std::cout << "best_score_found:\t\t" << stat.best_score << std::endl;
		std::cout << "number_of_solution_tried:\t" << stat.number_of_solution_tried << std::endl;
		std::cout << "number_of_generations:\t\t" << stat.number_of_generations << std::endl;

		std::cout << "(for parsing " << stat << ")" << std::endl;
	}
}