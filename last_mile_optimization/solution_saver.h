#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <iomanip>
#include "solution.h"

namespace solver_helper {
	template<int N>
	class solution_saver {
		const std::string outfile;
		double best_score = -1;
	public:
		solution_saver(std::string outfile) :outfile(outfile) {}
		void save(const data::solution<N>& sol, double second_elapsed) {
			using namespace std;
			auto score = sol.get_score();
			if (score < best_score || best_score == -1 && score >= 0) {
				best_score = score;
				cout << "Saving new solution with score:\t"
					<< std::fixed << std::setw(20) << std::setprecision(6) << std::setfill(' ')
					<< best_score
					<< "\t time elapsed:\t "
					<< std::fixed << std::setw(6) << std::setprecision(1) << std::setfill(' ')
					<< second_elapsed << endl;

				ofstream myfile;
				myfile.open(outfile, ios_base::trunc);
				myfile << sol;
				myfile.close();
			}
		}
		double get_score() { return best_score; }
	};
}