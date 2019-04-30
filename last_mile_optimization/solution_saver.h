#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <iomanip>
#include "solution.h"
#include "statistics_helpers.h"

namespace solver_helper {
	template<int N>
	class solution_saver {
		const std::string outfile;
		double best_score = -1;
	public:
		solution_saver(std::string outfile) :outfile(outfile) {}
		void save(const data::solution<N>& sol, statistics::statistics stats) {
			using namespace std;
			auto score = sol.get_score();
			if (score < best_score || best_score == -1 && score >= 0) {
				best_score = score;
				cout << "Saves with score: "
					<< std::fixed << std::setw(15) << std::setprecision(6) << std::setfill(' ')
					<< best_score
					<< "  time: "
					<< std::fixed << std::setw(6) << std::setprecision(1) << std::setfill(' ')
					<< stats.duration 
					<< "  # generation:"
					<< std::fixed << std::setw(5) << std::setfill(' ')
					<< stats.number_of_generations
					<< "  # solutions evaluated:"
					<< std::fixed << std::setw(11) << std::setfill(' ')
					<< stats.number_of_solution_tried
					<<endl;

				ofstream myfile;
				myfile.open(outfile, ios_base::trunc);
				myfile << sol;
				myfile.close();
			}
		}
		double get_score() { return best_score; }
	};
}