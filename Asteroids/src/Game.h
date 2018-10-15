#ifndef game_H
#define game_H

namespace Juego 
{

	enum GameScreen { menu = 0, game, over, credits, controls };
	extern GameScreen Screens;

	int core();
	void InitGame();
	void UpdateGame();
	void DrawGame();
	void UnloadGame();
	void Change();
	void UpdateDrawFrame();

}

#endif // !game_H