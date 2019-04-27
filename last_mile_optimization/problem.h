#pragma once
#include "data_helpers.h"
#include "parsed_problem.h"

namespace data {
	template<int N>
	struct problem {
		struct route {
			unsigned int distances[N][N] = { INF };
			int total_length;

			struct route_node { int index; int distance_to_next; };

			route(const vector<int>& route_indices, const vector<vector<int>>& adjusted_matrix) {
				fill(*distances, *distances + N * N, INF);
				int n = (int)route_indices.size();
				vector<route_node> nodes;
				nodes.reserve(n);

				total_length = 0;
				for (auto i = 0; i < n; ++i)
					nodes.push_back({ route_indices[i], adjusted_matrix[route_indices[i]][route_indices[(i + 1) % n]] });
				for (auto i = 0; i < n; ++i)
					total_length += nodes[i].distance_to_next;

				for (int i = 0; i < n; ++i) {
					int length = nodes[i].distance_to_next;
					for (int j = (i + 1) % n; j != i; j = (j + 1) % n)
						distances[nodes[i].index][nodes[j].index] = length,
						length += nodes[j].distance_to_next;
				}
			}
		};

		const int no_nodes;
		unsigned int demand[N][N] = { 0 };
		const vector<route> routes;
		const int total_routes_needed, total_cars;
		int total_demand = 0;
		problem(const parsed_problem & parse_prob) :
			no_nodes(parse_prob.no_stations),
			routes(to_routes(parse_prob.routes, parse_prob.adjacency_matrix)),
			total_routes_needed(parse_prob.no_routes_in_solution),
			total_cars(parse_prob.no_cars)
		{
			for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j)demand[i][j] = parse_prob.ride_request_matrix[i][j];
		}

	private:
		static vector<route> to_routes(const vector<vector<int>> & routes, const vector<vector<int>> & adjusted_matrix) {
			vector<route> t; for (auto r : routes) t.emplace_back(r, adjusted_matrix); return t;
		}
	};
}