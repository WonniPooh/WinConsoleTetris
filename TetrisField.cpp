#include "TetrisField.h"
#include <string.h>
#include <algorithm>
#include <conio.h>

TetrisField::TetrisField(BaseApp& m_output) : output(m_output),
													tetris_array(tetris_field_size.first * tetris_field_size.second, 0)
{
	current_score = 0;
	current_lines = 0;
	time_since_last_step = 0;

	CreateField();
	SetTetrisField();

	current_figure.reset(new figure());
	next_figure.reset(new figure());

	current_figure->SetStartingPos(tetris_field_size.first / 2 - 1);
	next_figure->SetStartingPos(tetris_field_size.first / 2 - 1);
}

void TetrisField::StartGame()
{
	current_score = 0;
	current_lines = 0;
	time_since_last_step = 0;

	current_figure->GenerateFigure();
	next_figure->GenerateFigure();

	SetScore();
	SetLines();

	SetTetrisField();

	SetNextFigure();
	FigureAction(true);
}

void TetrisField::ResetGame()
{
	for (auto& elem : tetris_array) {
		elem = 0;
	}
	SetNextFigure(false);
}

void TetrisField::CreateField()
{
	auto score_str = "Score";
	for (int i = 0; i < strlen(score_str); i++) {
		output.SetChar(i + score_pos.first+1, score_pos.second-1, score_str[i]);
	}

	CreateBoundaries(std::pair<int, int>(score_pos.first-3, score_pos.second-2), 
					 std::pair<int, int>(score_pos.first+8, score_pos.second+1));

	
	auto lines_str = "Line";
	for (int i = 0; i < strlen(lines_str); i++) {
		output.SetChar(i + 1, line_pos.second-1, lines_str[i]);
	}

	CreateBoundaries(std::pair<int, int>(line_pos.first-1, line_pos.second-2),
					 std::pair<int, int>(line_pos.first+4, line_pos.second+1));

	CreateBoundaries(std::pair<int, int>(next_fig_pos.first-1, next_fig_pos.second-1),
					 std::pair<int, int>(next_fig_pos.first+5, next_fig_pos.second+2));

	CreateBoundaries(std::pair<int, int>(tetris_field_pos.first - 1, tetris_field_pos.second - 1),
					 std::pair<int, int>(tetris_field_pos.first + tetris_field_size.first, tetris_field_pos.second + tetris_field_size.second));

}



void TetrisField::CreateBoundaries(const std::pair<int, int>& left_up, const std::pair<int, int>& right_down) {
	for (int i = left_up.first; i <= right_down.first; i++) {
		for (int j = left_up.second; j <= right_down.second; j++) {

			if (i == left_up.first || i == right_down.first) {
				output.SetChar(i, j, boundaries);
			}
			else {
				if (j == left_up.second || j == right_down.second) {
					output.SetChar(i, j, boundaries);
				}
			}
		}
	}
}

void TetrisField::SetScore() {

	auto current_score(std::to_string(current_score));
	current_score = std::string(6-current_score.length(), '0') + current_score;

	for (int i = 0; i < 6; i++) {
		output.SetChar(score_pos.first +i, score_pos.second, current_score[i]);
	}
}

void TetrisField::SetLines() {

	auto lines(std::to_string(current_lines));
	lines = std::string(4 - lines.length(), '0') + lines;

	for (int i = 0; i < 4; i++) {
		output.SetChar(line_pos.first + i, line_pos.second, lines[i]);
	}
}

void TetrisField::SetNextFigure(bool is_full) {
	auto m_figure = next_figure->GetFigure();

	wchar_t char_to_use = field_full;

	if (!is_full) {
		char_to_use = field_empty;
	}

	for (auto elem : m_figure) {
		
		int x_pos = tetris_field_size.first/2 - elem.first + next_fig_pos.first + 2;
		
		if (next_figure->GetFigureType() == line)
			x_pos -= 1;

		output.SetChar(x_pos, next_fig_pos.second + elem.second + 1, char_to_use);
	}
}


void TetrisField::SetTetrisField() {
	for (int i = 0; i < tetris_field_size.second; i++) {

		for (int j = 0; j < tetris_field_size.first; j++) {

			wchar_t set_val;
			if (tetris_array[i * tetris_field_size.first + j] == 0) {
				set_val = field_empty;
			}
			else {
				set_val = field_full;
			}
			output.SetChar(j + tetris_field_pos.first, i + tetris_field_pos.second, set_val);
		}
	}
}


void TetrisField::FigureAction(bool is_print) {

	auto m_figure = current_figure->GetFigure();

	auto symbol = field_full;

	if (!is_print) {
		symbol = field_empty;
	}

	for (auto elem : m_figure) {
		if (elem.first >= 0 && elem.second >= 0) {
			output.SetChar(elem.first + tetris_field_pos.first, elem.second + tetris_field_pos.second, symbol);
		}
	}
}


bool TetrisField::CheckFigTransformationPossible(const std::vector<std::pair<int, int>>& figure)
{
	bool is_possible = true;
	for (auto elem : figure) {
		if (elem.first >= tetris_field_size.first || elem.second >= tetris_field_size.second || elem.first < 0) {
			is_possible = false;
			break;
		}

		int tetris_array_coord = elem.first + elem.second*tetris_field_size.first;

		if (tetris_array_coord >= 0 && tetris_array[tetris_array_coord] == true) { //is full
			is_possible = false;
			break;
		}
	}

	return is_possible;
}

bool TetrisField::MakeFigureStatic()
{
	auto figure = current_figure->GetFigure();

	for (auto elem : figure) {
		int index = elem.first + elem.second * tetris_field_size.first;
		if (index < 0) {
			return false;
		}
		tetris_array[index] = true;
	}
	return true;
}

void TetrisField::CheckCleanFilledLanes()
{
	std::vector<int> rows_index_to_drop;
		
	for (int i = 0; i < tetris_field_size.second; i++) {
		bool is_full = true;

		for (int j = 0; j < tetris_field_size.first; j++) {
			if (tetris_array[i * tetris_field_size.first + j] == false) {
				is_full = false;
				break;
			}
		}

		if (is_full) {
			rows_index_to_drop.push_back(i);
		}
	}

	int arr_size = rows_index_to_drop.size();

	if (arr_size > 0) {
		current_score += awards[arr_size - 1];
		current_lines += arr_size;
	}

	for (int row_index : rows_index_to_drop) {
		auto iter = tetris_array.begin() + row_index * tetris_field_size.first;
		tetris_array.erase(iter, iter+tetris_field_size.first);
	}

	auto missig_part(std::vector<int>(10*arr_size, 0));
	tetris_array.insert(tetris_array.begin(), missig_part.begin(), missig_part.end());
}



void TetrisField::KeyPressed(int btnCode) {
	
	char ch = 0;
	bool action_ok = false;

	enum {
		arrow_right=77,
		arrow_left=75,
		arrow_down=80,
		arrow_up=72,
		space=32
	};

	if (btnCode == 224) {
		ch = getch();
		
		if (ch == arrow_right) {
			current_figure->TryMoveFigure(1, 0);
		}else if (ch == arrow_left) {
			current_figure->TryMoveFigure(-1, 0);
		} else if (ch == arrow_down) {
			current_figure->TryMoveFigure(0, 1);
		}else if (ch == arrow_up) {
			current_figure->TryRotateFigure();
		}
	}

	if (btnCode == space) { //drop figure
		FigureAction(false);
		while (true) {
			current_figure->TryMoveFigure(0, 1);
			action_ok = CheckFigTransformationPossible(current_figure->get_trasformation_result());
			
			if (action_ok) {
				current_figure->CommitTransformation();
			}
			else {
				time_since_last_step = 1;
				UpdateF(0);
				return;
			}
		}
	}

	if(current_figure->WasTransformed())
		action_ok = CheckFigTransformationPossible(current_figure->get_trasformation_result());
	
	if (action_ok) {
		FigureAction(false);
		current_figure->CommitTransformation();
		FigureAction(true);
	}
}

bool TetrisField::UpdateF(float deltaTime) //Update frame
{
	time_since_last_step += deltaTime;
	if (time_since_last_step >= 1) {
		time_since_last_step = 0;
		
		current_figure->TryMoveFigure(0, 1);
		bool result = CheckFigTransformationPossible(current_figure->get_trasformation_result());
		
		if (result) {
			FigureAction(false);
			current_figure->CommitTransformation();
			FigureAction(true);
		}
		else {
			bool is_ok = MakeFigureStatic();
			if (!is_ok) {
				return false;
			}
			CheckCleanFilledLanes();
			SetNextFigure(false);

			current_figure.swap(next_figure);
			next_figure->GenerateFigure();

			SetScore();
			SetLines();

			SetNextFigure();
			SetTetrisField();
			FigureAction(true);
		}		
	}

	return true;
}