#include "include/raylib.h"
#include "Menu.h"
#include "Game.h"

void MenuUpdate() {
	MenuDraw();

	
}
void MenuDraw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	DrawRectangle(screenWidth / 2 - 40, screenHeight / 2 - 40, 80, 80, MAROON);
	DrawText("Presione \"Escape\" para salir", 130, 300, 20, MAROON);
	DrawText("r", 130, 300, 20, MAROON);

	EndDrawing();
}