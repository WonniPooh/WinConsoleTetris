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
	void SetStartingPos(int pos);

	void GenerateFigure();

	int GetFigureType();

	void RotateFigure();
	void TryRotateFigure();
	void TryMoveFigure(int x_delta, int y_delta);

	const std::vector<std::pair<int,int>>& GetFigure();
	const std::vector<std::pair<int, int>>& get_trasformation_result();
	void CommitTransformation();

	bool WasTransformed();

private:
	int starting_pos = 0;
	int current_figure_type;
	bool is_transformed;
	void MoveFigure(int x_delta, int y_delta);

	std::vector<std::pair<int,int>> figure;
	std::vector<std::pair<int, int>> transformed_figure;
};
