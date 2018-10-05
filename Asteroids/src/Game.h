#ifndef Game_H
#define Game_H

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

#endif // !Game_H