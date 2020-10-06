#include "TetrisGame.h"

TetrisGame::TetrisGame() :  BaseApp(50, 30),
							tetris(*this),
							game_status(game_new)
{
	PrintControlInfo();
}

void TetrisGame::KeyPressed(int btnCode)
{
	char ch = 0;

	enum {
		quit = 113,
		pause = 112,
		game_continue = 99,
		reset = 114,
		start = 115
	};

	switch (btnCode) {
	case quit:
		exit(0);
		break;
	case pause:
		game_status = game_paused;
		PrintPaused();
		break;
	case game_continue:
		if (game_status == game_paused) {
			game_status = game_paused;
			game_status = game_active;
			tetris.SetTetrisField();
			tetris.FigureAction(true);
		}
		break;
	case reset:
		if (game_status != game_new) {
			game_status = game_new;
			PrintGameOver();
			tetris.ResetGame();
		}
		break;
	case start:
		if (game_status == game_new) {
			game_status = game_active;
			tetris.StartGame();
		}
		break;
	}

	if (game_status == game_active) {
		tetris.KeyPressed(btnCode);
	}
}

void TetrisGame::UpdateF(float deltaTime)
{
	bool result = false;
	if (game_status == game_active) {
		result = tetris.UpdateF(deltaTime);
		if (result == false) {
			game_status = game_over;
			PrintGameOver();
		}
	}
}

void TetrisGame::PrintGameOver()
{
	auto over_str = "Game Over";

	for (int i = 0; i < strlen(over_str); i++) {
		SetChar(1 + i, tetris.tetris_field_pos.second + 7, ' ');
		SetChar(1 + i, tetris.tetris_field_pos.second + 8, over_str[i]);
		SetChar(1 + i, tetris.tetris_field_pos.second + 9, ' ');
	}
}

void TetrisGame::PrintPaused()
{
	auto paused_str = "Paused";

	for (int i = 0; i < strlen(paused_str); i++) {
		SetChar(3+i, tetris.tetris_field_pos.second + 7, ' ');
		SetChar(3+i, tetris.tetris_field_pos.second + 8, paused_str[i]);
		SetChar(3+i, tetris.tetris_field_pos.second + 9, ' ');
	}
}

void TetrisGame::PrintControlInfo()
{
	for (int i = 0; i < controls_description.size(); i++) {
		for (int j = 0; j < controls_description[i].length(); j++) {
			SetChar(controls_desc_position.first + j, controls_desc_position.second + i, controls_description[i][j]);
		}
	}
}
