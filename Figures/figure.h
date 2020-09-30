#pragma once
#include <utility>
#include <memory>
#include <vector>

enum figure_types {
	square,
	line,
	triangle_like,
	left_rake,
	right_rake,
	left_duck,
	right_duck
};

class figure
{
public:
	void generate_figure();

	int get_figure_type();

	void rotate_figure();
	void try_rotate_figure();
	void try_move_figure(int x_delta, int y_delta);

	const std::vector<std::pair<int,int>>& get_figure();
	const std::vector<std::pair<int, int>>& get_trasformation_result();
	void commit_transformation();

	bool was_transformed();

private:
	int current_figure_type;
	bool is_transformed;
	void move_figure(int x_delta, int y_delta);

	std::vector<std::pair<int,int>> figure;
	std::vector<std::pair<int, int>> transformed_figure;
};
