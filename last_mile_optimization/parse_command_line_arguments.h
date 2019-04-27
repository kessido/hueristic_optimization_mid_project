#pragma once
#include <string>
#include <iostream>
#include "clipp.h"
#include "brute_force_solver_parameters.h"
#include "genetic_solver_parameters.h"
#include "problem_type_parameters.h"

namespace parse_cmd {
	using namespace clipp;
	using namespace std;
	struct cmd_args {
		string infile = "", outfile = "";
		string solver_name = "genetic";
		int runtime = -1;
	};

	bool try_parse_command_line_argument(int argc, char* argv[], cmd_args& cmd) {
		// options:
		auto infile_required = required("-i", "--input") & value("input_file", cmd.infile);
		auto outfile_required = required("-o", "--out") & value("output_file", cmd.outfile);
		auto mode_required = required("-m", "--mode") &
			(required("direct").set(data::problem_type_parameters::problem_mode, data::ProblemMode::Direct) |
				required("single").set(data::problem_type_parameters::problem_mode, data::ProblemMode::SinglePass));
		auto brute_option = option("-b", "--brute") &
			value("brute_force_option", solvers::brute_force::brute_force_solver_parameters::number_of_routes_to_brute_force);
		auto solver_option = option("-s", "--solver") & (required("genetic").set(cmd.solver_name, string("genetic")));
		auto runtime_option = option("-r", "--runtime") & value("run_time", cmd.runtime);
		auto car_randomized_precentage =
			option("--genetic_car_randomized_precentage") &
			value("car_randomized_precentage", solvers::genetic::genetic_solver_parameters::car_randomized_precentage);
		auto generation_size =
			option("--genetic_generation_size") &
			value("generation_size", solvers::genetic::genetic_solver_parameters::generation_size);

		// docs:
		infile_required.doc("The input file for the algorithm");
		outfile_required.doc("The output file to save to");
		mode_required.doc("The mode for the algorithm. \n"
			"direct: rider can only use a single ride to arrive to their destination (NATIV YASHIR). \n"
			"single: rider can use up to 2 rides to arrive to thier destination (MAAVAR AHAD)");
		brute_option.doc(string("Set how many routes choises should be done using brute force. \n"
			"default = ") + to_string(solvers::brute_force::brute_force_solver_parameters::number_of_routes_to_brute_force));
		solver_option.doc(string("Set which solver algorithm to use. \n"
			"default = ") + cmd.solver_name);
		runtime_option.doc(string("Set the running time of the algorithm. After each step in the algorithm, "
			"If more then run_time seconds passed, the algorithm shall stop.\n"
			"-1 = infinitly (it will keep saving the best result to the output file)\n"
			"default = ") + to_string(cmd.runtime));
		car_randomized_precentage.doc(string("Used inside the genetic algorithm. "
			"This is the precentage of cars that are being set in the genetic algorithm phase, "
			"and not in the brute force phase. \n"
			"default = ") + to_string(solvers::genetic::genetic_solver_parameters::car_randomized_precentage));
		generation_size.doc(string("Used inside the genetic algorithm. "
			"This set the size if the generations. \n"
			"default = ") + to_string(solvers::genetic::genetic_solver_parameters::generation_size));

		auto cli = (
			infile_required,
			outfile_required,
			mode_required,
			brute_option,
			solver_option,
			runtime_option,
			car_randomized_precentage,
			generation_size
			);

		if (parse(argc, argv, cli)) return true;
		cout << make_man_page(cli, argv[0]);
		return false;
	}
}