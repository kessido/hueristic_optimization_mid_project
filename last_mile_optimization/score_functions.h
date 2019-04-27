#pragma once
#include <algorithm>
#include <cstring>
#include "data_helpers.h"

namespace scoring {
	template<unsigned int N>
	long long get_score_direct(const unsigned long long current_solution[N][N], const unsigned int demand[N][N]) {
		long long res = 0;
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				if (demand[i][j] != 0) {
					if (current_solution[i][j] == data::INFll) return -1;
					res += current_solution[i][j] * demand[i][j];
				}
		return res;
	}

	template<unsigned int N>
	long long get_score_single(const unsigned long long current_solution[N][N], const unsigned int demand[N][N]) {
		unsigned long long res[N][N];
		memcpy(res, current_solution, sizeof(res));
#pragma omp parallel for
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				if (demand[i][j]) {
					unsigned long long m = res[i][j];
					for (int k = 0; k < N; k++)
					{
						unsigned long long m1 = current_solution[i][k] + current_solution[k][j];
						if (m > m1) m = m1;
					}
					res[i][j] = m;
				}
			}
		return get_score_direct<N>(res, demand);
	}
}
