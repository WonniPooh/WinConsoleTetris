#pragma once
#include "BaseApp.h"
#include "figure.h"

#include <memory>
#include <utility>

class TetrisField
{
public:
	const std::pair<int, int> tetris_field_size{10,20};
	const std::pair<int, int> tetris_field_pos{ 1,  7 };

	void StartGame();

	void KeyPressed(int btnCode);
	bool UpdateF(float deltaTime); //update current frame

	void ResetGame();
	void SetTetrisField();
	void FigureAction(bool is_print);
	explicit TetrisField(BaseApp& output);

private:

	BaseApp& output;
	float time_since_last_step;

	int current_score;
	int current_lines;
	int speed_level;

	std::unique_ptr<figure> current_figure;
	std::unique_ptr<figure> next_figure;

	const std::vector<int> awards{ 40,100,300,1200 };

	const std::pair<int, int> score_pos{ 3, 2 };
	const std::pair<int, int> line_pos{ 1, 5 };
	const std::pair<int, int> next_fig_pos{ 6,  4};

	const char boundaries = '+';

	const wchar_t field_full = 223;//L'\u2589';
	const wchar_t field_empty = 249;

	std::vector<bool> tetris_array;
	
	void CreateField();
	void CreateBoundaries(const std::pair<int, int>& left_up, const std::pair<int, int>& right_down);

	void SetScore();
	void SetNextFigure(bool is_full=true);
	void SetLines();
	
	bool CheckFigTransformationPossible(const std::vector<std::pair<int, int>>& figure);
	bool MakeFigureStatic();
	void CheckCleanFilledLanes();

};