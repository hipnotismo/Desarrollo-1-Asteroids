#ifndef game_H
#define game_H

namespace Juego 
{

	enum GameScreen { Main = 0, Game, Over, Credits };
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