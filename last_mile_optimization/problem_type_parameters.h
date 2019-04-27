#pragma once

namespace data {

	enum ProblemMode {
		Direct,
		SinglePass
	};

	class problem_type_parameters {
	public:
		static ProblemMode problem_mode;
	};
}

data::ProblemMode data::problem_type_parameters::problem_mode = data::ProblemMode::Direct;