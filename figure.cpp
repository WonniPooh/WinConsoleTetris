#include "figure.h"


void figure::MoveFigure(int x_delta, int y_delta)
{
	for (auto& elem : transformed_figure) {
		elem.first += x_delta;
		elem.second += y_delta;
	}
}

void figure::GenerateFigure()
{
	figure.clear();
	is_transformed = false;

	current_figure_type = rand() % 7;

	auto line_len_3 = [&]() {
		figure.push_back(std::pair<int, int>(4, 0));
		figure.push_back(std::pair<int, int>(5, 0));
		figure.push_back(std::pair<int, int>(6, 0));
	};

	switch (current_figure_type)
	{
	case square:
		figure.push_back(std::pair<int, int>(4, -1));
		figure.push_back(std::pair<int, int>(5, -1));
		figure.push_back(std::pair<int, int>(4, 0));
		figure.push_back(std::pair<int, int>(5, 0));
		break;
	case line:
		figure.push_back(std::pair<int, int>(3, 0));
		line_len_3();
		break;
	case triangle_like:
		figure.push_back(std::pair<int, int>(5, -1));
		line_len_3();
		break;
	case left_rake:
		figure.push_back(std::pair<int, int>(4, -1));
		line_len_3();
		break;
	case right_rake:
		figure.push_back(std::pair<int, int>(6, -1));
		line_len_3();
		break;
	case left_duck:
		figure.push_back(std::pair<int, int>(4, -1));
		figure.push_back(std::pair<int, int>(5, -1));
		figure.push_back(std::pair<int, int>(5, 0));
		figure.push_back(std::pair<int, int>(6, 0));
		break;
	case right_duck:
		figure.push_back(std::pair<int, int>(5, -1));
		figure.push_back(std::pair<int, int>(6, -1));
		figure.push_back(std::pair<int, int>(4, 0));
		figure.push_back(std::pair<int, int>(5, 0));
		break;
	default:
		break;
	}
}

int figure::GetFigureType()
{
	return current_figure_type;
}

void figure::RotateFigure() {

	enum direction {
		left = 0,
		right
	};

	auto rotate_over_point = [this](int index, bool direction=left) {
		auto turnover_point = transformed_figure[index];

		for (int i = 0; i < 4; i++) {
			transformed_figure[i].first -= turnover_point.first;
			transformed_figure[i].second -= turnover_point.second;

			auto temp_var = transformed_figure[i].first;
			if (direction == left) {
				transformed_figure[i].first = transformed_figure[i].second + turnover_point.first;
				transformed_figure[i].second = -temp_var + turnover_point.second;
			}
			else {
				transformed_figure[i].first = -transformed_figure[i].second + turnover_point.first;
				transformed_figure[i].second = temp_var + turnover_point.second;
			}
		}
	};

	switch (current_figure_type)
	{
	case square:
		break;
	case line:
		if(transformed_figure[0].first == transformed_figure[1].first) // vertical
			rotate_over_point(1);
		else
			rotate_over_point(1, right);
		break;
	case triangle_like:
		rotate_over_point(2);
		break;
	case left_rake:
		if (transformed_figure[1].second == transformed_figure[3].second) {		//   horizontal
			rotate_over_point(2);
			if (transformed_figure[0].first > transformed_figure[1].first) {	//	on the right
				MoveFigure(-1, 1);
			}
			break;
		}

		if (transformed_figure[1].first == transformed_figure[3].first) {		//   vertical
			if (transformed_figure[0].first < transformed_figure[1].first) {	//on the left
				MoveFigure(0, -1);
			}
			else {
				MoveFigure(1, 0);
			}
			rotate_over_point(2);
		}
		break;
	case right_rake:
		if (transformed_figure[1].second == transformed_figure[3].second){		//   horizontal
			rotate_over_point(2);
			if (transformed_figure[0].first > transformed_figure[1].first) {	// on the right
				MoveFigure(-1, 1);
			}
			break;
		}

		if (transformed_figure[1].first == transformed_figure[3].first){		//   vertical
			if (transformed_figure[0].first < transformed_figure[1].first) {	// on the left
				MoveFigure(0, -1);
			}
			else {
				MoveFigure(1, 0);
			}
			rotate_over_point(2);
		}
		break;
	case left_duck:
		if (transformed_figure[1].first != transformed_figure[2].first) // if 0 is left or down
			rotate_over_point(2, right);
		else
			rotate_over_point(2);
		break;
	case right_duck:
		if (transformed_figure[0].first < transformed_figure[3].first) // if 0 is left or down
			rotate_over_point(3, right);
		else
			rotate_over_point(3);

		break;
	};
}


void figure::TryRotateFigure()
{
	is_transformed = true;
	transformed_figure = figure;
	RotateFigure();
}

void figure::TryMoveFigure(int x_delta, int y_delta)
{
	is_transformed = true;
	transformed_figure = figure;
	MoveFigure(x_delta, y_delta);
}

const std::vector<std::pair<int, int>>& figure::get_figure()
{
	return figure;
}

const std::vector<std::pair<int, int>>& figure::get_trasformation_result()
{
	return transformed_figure;
}

void figure::CommitTransformation()
{
	figure = std::move(transformed_figure);
	is_transformed = false;
}

bool figure::WasTransformed()
{
	return is_transformed;
}
