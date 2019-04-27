#pragma once
#include <algorithm>
#include <cassert>
#include <map>
#include <utility>
#include "problem.h"
#include "score_functions.h"
#include "brute_force_solver_parameters.h"

namespace data {
	struct route_car_allocation {
		int route_index, car_count;

	private:
		std::pair<int, int> as_pair() const { return { route_index, car_count }; }
		friend bool operator<(const route_car_allocation& lhs, const route_car_allocation& rhs) { return lhs.as_pair() < rhs.as_pair(); }
		friend bool operator==(const route_car_allocation& lhs, const route_car_allocation& rhs) { return lhs.as_pair() == rhs.as_pair(); }
	};

	template<int N>
	struct solution {
		// default constructors.
		solution() = default;
		solution(const solution& other) = default;
		solution(solution&& other) = default;
		solution& operator=(const solution& other) = default;
		solution& operator=(solution&& other) = default;

		solution(const problem<N>* prob) : prob(prob), routes_left_to_add(prob->total_routes_needed)
		{
			routes.reserve(prob->total_routes_needed);
			fill(*current_solution, *current_solution + N * N, INFll);
		};

		solution& operator+=(const route_car_allocation rca) { add_to_solution(rca); return *this; }
		solution operator+(const route_car_allocation rca) const { auto s = *this; return s += rca; }

	private:
		static const long long empty_score = -2;

		static double calc_score_from_last_score(long long score) {
			assert(score >= 0 || score == -1); return ((double)score) / (1ll << percision);
		}

	public:
		// < 0 - not complete.
		double get_score() {
			return calc_score_from_last_score(get_score_inner());
		}

		double get_score() const { return calc_score_from_last_score(last_score); }

		int total_cars_in_use() const { int res = 0; for (auto& i : routes) res += i.car_count; return res; }

		const vector<route_car_allocation>& get_routes() const { return routes; }

		friend ostream& operator<<(ostream & out, const solution & s) {
			out << s.routes.size() << endl; for (auto r : s.routes) out << (r.route_index + 1) << " " << r.car_count << endl; return out;
		}

		friend bool operator<(const solution & lhs, const solution & rhs) { return lhs.get_score() < rhs.get_score(); }
		friend bool operator==(const solution & lhs, const solution & rhs) { return lhs.to_map() == rhs.to_map(); }
	private:
		const problem<N>* prob;
		int routes_left_to_add;
		long long last_score = empty_score;
		unsigned long long current_solution[N][N] = { INFll };
		vector<route_car_allocation> routes;

		long long get_score_inner() {
			if (last_score == empty_score) {
				if (this->need_brute_force()) brute_force();
				else {
					if (total_cars_in_use() != prob->total_cars) last_score = -1;
					else {
						switch (problem_type_parameters::problem_mode)
						{
						case ProblemMode::Direct:
							last_score = scoring::get_score_direct<N>(current_solution, prob->demand);
							break;
						case ProblemMode::SinglePass:
							last_score = scoring::get_score_single<N>(current_solution, prob->demand);
							break;
						}
					}
				}
			}
			return last_score;
		}

		map<int, int> to_map() { map<int, int> s1; for (auto r : routes) s1[r.route_index] += r.car_count; return s1; }
		bool need_brute_force() const {
			return
				routes_left_to_add > 0 &&
				routes_left_to_add <= solvers::brute_force::brute_force_solver_parameters::number_of_routes_to_brute_force &&
				total_cars_in_use() < prob->total_cars;
		}
		void brute_force() {
			int car_left = prob->total_cars - total_cars_in_use();
			assert(car_left > 0);
			last_score = INFll;
			auto s_best = *this;
			auto s = *this;
			for (auto ind = 0; ind < prob->routes.size(); ++ind) {
				int car_count = (car_left + 1) / max(1, routes_left_to_add);
				car_count = max(car_count, 1);
				if (routes_left_to_add <= 1) car_count = car_left;
				s = *this;
				for (; car_count <= car_left; ++car_count) {
					s = *this + route_car_allocation{ ind, car_count };
					auto score = s.get_score_inner();
					if (score < last_score && score >= 0)
						last_score = score, s_best = s;
				}
			}
			*this = s_best;
		}
		void add_to_solution(route_car_allocation rca) {
			assert(rca.car_count > 0);
			last_score = empty_score;
			routes_left_to_add--;
			routes.push_back(rca);
			rca.car_count = 0;

			for (auto& i : routes)
				if (i.route_index == rca.route_index)
					rca.car_count += i.car_count;

			auto & r2 = prob->routes[rca.route_index].distances;
			unsigned long long r_wait = ((((long long)prob->routes[rca.route_index].total_length) << percision) / rca.car_count) / 2;

			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					current_solution[i][j] = std::min(current_solution[i][j], r_wait + (((long long)r2[i][j]) << percision));
				}
			}
		}
	};
}
