#include "Menu.h"

#include "include/raylib.h"

#include "Game.h"

namespace Juego 
{

	Vector2 mousePoint;
	Rectangle rec1;
	void MenuUpdate()
	{
		MenuDraw();
		mousePoint = GetMousePosition();
		rec1.x = screenWidth / 2 - 50;
		rec1.y = screenHeight / 2 + 30;
		rec1.height = 90;
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


		DrawRectangle(rec1.x, rec1.y, rec1.height, rec1.width, MAROON);
		//DrawText("Presione \"Escape\" para salir", 130, 300, 20, MAROON);
		//DrawText("r", 130, 300, 20, MAROON);

		EndDrawing();
	}

}