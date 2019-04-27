#pragma once
#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace data {
	using namespace std;

	struct parsed_problem {
		int no_stations, no_routes, no_cars, no_routes_in_solution;
		vector<vector<int>> routes, ride_request_matrix, adjacency_matrix;

		friend ifstream& operator>>(ifstream& myfile, parsed_problem& s) {
			s.parse_from_file(myfile);
			return myfile;
		}

		static parsed_problem from_file(const string infile) {
			ifstream myfile;
			myfile.open(infile);
			parsed_problem res;
			myfile >> res;
			myfile.close();
			return res;
		}

	private:
		void parse_from_file(ifstream &myfile) {
			auto next_line = [&]() {
				string line;
				getline(myfile, line);
				return istringstream(line);
			};
			auto next_array = [&](int offset = 0) {
				auto line = next_line();
				int i;
				vector<int> res;
				while (line >> i) res.push_back(i + offset);
				return res;
			};
			auto next_matrix = [&](int n, int offset = 0) {
				vector<vector<int>> res(n);
				for (auto& i : res) i = next_array(offset);
				return res;
			};
			auto assert_matrix_size = [](vector<vector<int>> & mat, int n, int m) {
				assert(mat.size() == n);
				for (auto& i : mat) assert(i.size() == m);
			};
			next_line() >> no_stations >> no_routes >> no_cars >> no_routes_in_solution;
			routes = next_matrix(no_routes, -1);
			ride_request_matrix = next_matrix(no_stations);
			assert_matrix_size(ride_request_matrix, no_stations, no_stations);
			adjacency_matrix = next_matrix(no_stations);
			assert_matrix_size(adjacency_matrix, no_stations, no_stations);
		}
	};
}