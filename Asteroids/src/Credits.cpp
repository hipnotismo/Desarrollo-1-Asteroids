#include "Credits.h"

#include "include/raylib.h"

#include "Game.h"

namespace Juego
{

	void CreditsUpdate() {
		CreditsDraw();
		if (IsKeyDown(KEY_SPACE)) {
			Screens = Game;
		}
	}


	void CreditsDraw() {
		BeginDrawing();

		ClearBackground(BLACK);
		DrawText("Funciones creadas porIan Eito, Albert Martos y Ramon Santamaria", 20, 200, 20, MAROON);
		DrawText("Copyright (c) 2015 Ramon Santamaria (@raysan5)", 20, 220, 20, MAROON);

		DrawText("P O N G, por Martin Concetti", 20, 260, 20, MAROON);
		DrawText("Creado usando raylib y sfxr", 130, 280, 20, MAROON);
		DrawText("Consultantes: Baretto, Sergio", 130, 300, 20, MAROON);
		DrawText("Version 0.7", 130, 320, 20, MAROON);

		EndDrawing();
	}

}