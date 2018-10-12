#include "include/raylib.h"

#include "menu.h"
#include "game.h"

namespace Juego 
{

	Vector2 mousePoint;
	Rectangle rec1;
	Rectangle rec2;
	void MenuUpdate()
	{
		MenuDraw();
		mousePoint = GetMousePosition();
		rec1.x = screenWidth / 2 - 50;
		rec1.y = screenHeight / 2 ;
		rec1.height = 100;
		rec1.width = 50;

		rec2.x = screenWidth / 2 - 50;
		rec2.y = screenHeight / 2 + 80;
		rec2.height = 100;
		rec2.width = 50;

		if (CheckCollisionPointRec(mousePoint, rec1))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = Game;
			}
		}
		if (CheckCollisionPointRec(mousePoint, rec2))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = Credits;
			}
		}



	}

	void MenuDraw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);


		DrawRectangle(rec1.x, rec1.y, rec1.height, rec1.width, BLACK);
		DrawRectangle(rec2.x, rec2.y, rec2.height, rec2.width, BLACK);

		DrawText("jugar", rec1.x + 18, rec1.y + 10, 20, WHITE);
		DrawText("Creditos", rec2.x + 5, rec2.y + 10, 20, WHITE);
		DrawText("Asteroids", screenWidth / 2 - 120, 100, 50, MAROON);

		EndDrawing();
	}

}