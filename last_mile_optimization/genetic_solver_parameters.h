#pragma once

namespace solvers {
	namespace genetic {

		class genetic_solver_parameters {
		public:
			static double car_randomized_precentage;
			static int generation_size;
		};
	}
}

double solvers::genetic::genetic_solver_parameters::car_randomized_precentage = 0.7;
int solvers::genetic::genetic_solver_parameters::generation_size = 500;
