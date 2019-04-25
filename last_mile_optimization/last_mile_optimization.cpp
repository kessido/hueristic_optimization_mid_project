// last_mile_optimization.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <assert.h>
#include <iostream>
#include <sstream>
#include <sstream>
#include <fstream>
#include "clipp.h"

int main(int argc, char* argv[])
{
	using namespace clipp;
	using namespace std;
	string infile = "";
	bool direct_mode = false, single_mode = false;

	int no_stations, no_routes, no_cars, no_routes_in_solution;
	vector<vector<int>> routes, ride_request_matrix, adjacency_matrix;


	auto try_parse_command_line_argument = [&]() {
		auto infile_required = required("-i", "--input") & value("input_file", infile);
		auto mode_required = required("-m", "--mode") & (required("direct").set(direct_mode) | required("single").set(single_mode));

		auto cli = (
			infile_required.doc("The input file for the algorithm"),
			mode_required.doc("The mode for the algorithm. \n"
				"direct: rider can only use a single ride to arrive to their destination (NATIV YASHIR). \n"
				"single: rider can use up to 2 rides to arrive to thier destination (MAAVAR AHAD)")
			);

		if (!parse(argc, argv, cli)) {
			cout << make_man_page(cli, argv[0]);
			return false;
		}
		else {
			return true;
		}
	};
	
	if (try_parse_command_line_argument()) {
		ifstream myfile;
		myfile.open(infile);
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
		myfile.close();
	}



}