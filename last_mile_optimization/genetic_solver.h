#pragma once

#include <cassert>
#include <vector>
#include <omp.h>
#include "genetic_solver_parameters.h"
#include "genetic_solver_helpers.h"
#include "abstract_solver.h"
#include "simple_random_generator.h"
#include "randomly_combine_solutions.h"

namespace solvers {
	namespace genetic {
		using namespace data;
		using namespace std;
		
		template<int N>
		class genetic_solver : public solvers::solver<N>
		{
			vector<solution<N>> generation;
			random::simple_uniform_random_generator urand;
		public:
			genetic_solver(const problem<N> * prob) :solver<N>(prob), generation(genetic_solver_parameters::generation_size) {
				#pragma omp parallel for
				for (int gen_i = 0; gen_i < genetic_solver_parameters::generation_size; ++gen_i)
					generation[gen_i] = helpers::get_random_solution(this->prob);

				for (auto& s : generation) 
					this->update_best_solution(s);
			}

			bool solve_step() override {
				const int generation_size = genetic_solver_parameters::generation_size;
				solution<N> base_solution(this->prob);
				vector<solution<N>> new_generation(generation_size, solution<N>(this->prob));
				vector<int> new_generation_rand_index(generation_size);
				for (auto& i : new_generation_rand_index) i = urand.next(generation_size - 1);
				
				#pragma omp parallel for 
				for (int gen_i = 0; gen_i < generation_size; ++gen_i) {
					auto& old_s1 = generation[gen_i];
					int rand_ind = new_generation_rand_index[gen_i];
					auto& old_s2 = generation[rand_ind];
					auto& s = new_generation[gen_i];
					solution_interpolations::combine_into<N>(this->prob, old_s1, old_s2, s);
				}

				// maybe add here another option

				generation = new_generation;
				//after multithreading
				sort(generation.begin(), generation.end());

				#pragma omp parallel for
				for (int i = 2 * (generation_size + 1) / 3; i < generation_size; ++i)
					generation[i] = helpers::get_random_solution(this->prob);
				
				return this->update_best_solution(generation.front());
			}
			bool finished() const override { return false; }
		};
	}
}
