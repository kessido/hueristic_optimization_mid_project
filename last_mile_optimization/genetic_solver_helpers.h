#pragma once
#include <cassert>
#include <vector>
#include "abstract_solver.h"
#include "simple_random_generator.h"
#include "genetic_solver_parameters.h"

namespace solvers {
	namespace genetic {
		using namespace data;
		using namespace std;

		namespace helpers {
			route_car_allocation get_random_route_car_allocation(const float mean_car_to_randomize,
				const float std_car_to_randomize, const int no_routes, int car_left) {
				static random::simple_uniform_random_generator urand;
				static random::simple_normal_random_generator nrand;
				int car_count = (int)floor(nrand.next(mean_car_to_randomize, std_car_to_randomize));
				car_count = min(car_left, car_count);
				car_count = max(1, car_count);
				int index = urand.next(1, no_routes - 1);
				return route_car_allocation{ index, car_count };
			}

			template<int N>
			solution<N> get_random_solution(const problem<N> * prob) {
				solution<N> s(prob);
				int number_of_routes_to_randomize = prob->total_routes_needed -
					solvers::brute_force::brute_force_solver_parameters::number_of_routes_to_brute_force;
				assert(number_of_routes_to_randomize != 0);

				const double needed_cars_to_randomize = genetic_solver_parameters::car_randomized_precentage * prob->total_cars;
				const float mean_car_to_randomize = (float)(needed_cars_to_randomize / number_of_routes_to_randomize);
				const float std_car_to_randomize = (float)(mean_car_to_randomize / 2);

				for (int i = 0; i < number_of_routes_to_randomize; ++i)
					s += get_random_route_car_allocation(mean_car_to_randomize, std_car_to_randomize,
					(int)prob->routes.size(), prob->total_cars - s.total_cars_in_use());
				if (s.get_score() < 0)
					return get_random_solution(prob);
				return s;
			}
		}
	}
}