#pragma once

namespace solvers {
	namespace brute_force {

		class brute_force_solver_parameters {
		public:
			
			static int number_of_routes_to_brute_force;
		};
	}
}

int solvers::brute_force::brute_force_solver_parameters::number_of_routes_to_brute_force = 1;
