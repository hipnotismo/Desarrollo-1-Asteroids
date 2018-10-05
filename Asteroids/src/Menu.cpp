#include "include/raylib.h"
#include "Menu.h"
#include "Game.h"

Vector2 mousePoint;
Rectangle rec1;
void MenuUpdate() 
{
	MenuDraw();
	mousePoint = GetMousePosition();
	rec1.x = screenWidth / 2 - 40;
	rec1.y = screenHeight / 2 - 40;
	rec1.height = 40;
	rec1.width = 80;

	if (CheckCollisionPointRec(mousePoint, rec1))
	{
		
		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) 
		{
			Screens = Game;
		}
	}
	

	
}

void MenuDraw() 
{
	BeginDrawing();

	ClearBackground(RAYWHITE);


	DrawRectangle(screenWidth / 2 - 40, screenHeight / 2 - 40, 80, 80, MAROON);
	//DrawText("Presione \"Escape\" para salir", 130, 300, 20, MAROON);
	//DrawText("r", 130, 300, 20, MAROON);

	EndDrawing();
}