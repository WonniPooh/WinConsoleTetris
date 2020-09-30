#pragma once
#include "BaseApp.h"
#include "TetrisField.h"

class TetrisGame : public BaseApp
{
public:
	explicit TetrisGame();
	void KeyPressed(int btnCode) override;
	void UpdateF(float deltaTime) override; //update current frame

private:
	enum {
		game_new,
		game_active,
		game_paused,
		game_over
	};

	int game_status;

	const std::pair<int, int> controls_desc_position{ 14, 2 };
	const std::vector<std::string> controls_description{"q - quit", "s - start", "r - reset", "p - pause", "c - continue", " ",
														"ArrowUp - rotate", "ArrowDown - step down", "ArrowRight - move right", "ArrowLeft - move left",  
														"Space - drop fig"};

	void PrintControlInfo();
	void PrintGameOver();
	void PrintPaused();
	TetrisField tetris;
};

