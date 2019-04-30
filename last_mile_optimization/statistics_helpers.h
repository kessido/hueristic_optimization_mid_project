#pragma once
#include <ostream>

namespace statistics {
	struct statistics {
		double duration;
		double best_score;
		unsigned long long int number_of_solution_tried;
		int number_of_generations;

		friend std::ostream& operator<<(std::ostream& out, const statistics& s) {
			out << "|" << s.duration << "," << s.best_score << "," << s.number_of_solution_tried << "," << s.number_of_generations << "|";
			return out;
		}
	};
}
