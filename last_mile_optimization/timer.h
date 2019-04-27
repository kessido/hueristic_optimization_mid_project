#pragma once
#include <chrono>

namespace timer {
	using namespace std;

	class timer {
		chrono::steady_clock::time_point start_;
	public:
		timer() { start_ = chrono::steady_clock::now(); }
		double get_seconds() {
			return 0.001* chrono::milliseconds::rep(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start_).count());
		}
	};
}