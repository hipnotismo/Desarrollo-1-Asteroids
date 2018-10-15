#include "include/raylib.h"

#include "menu.h"
#include "game.h"

namespace Juego 
{

	Vector2 mousePoint;
	Rectangle rec1;
	Rectangle rec2;
	Rectangle rec3;

	void MenuUpdate()
	{
		MenuDraw();
		mousePoint = GetMousePosition();
		rec1.x = screenWidth / 2 - 50;
		rec1.y = screenHeight / 2 ;
		rec1.height = 50;
		rec1.width = 100;

		rec2.x = screenWidth / 2 - 50;
		rec2.y = screenHeight / 2 + 80;
		rec2.height = 50;
		rec2.width = 100;

		rec3.x = screenWidth / 2 - 50;
		rec3.y = screenHeight / 2 + 160;
		rec3.height = 50;
		rec3.width = 100;

		if (CheckCollisionPointRec(mousePoint, rec1))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = game;
			}
		}
		if (CheckCollisionPointRec(mousePoint, rec2))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = credits;
			}
		}
		if (CheckCollisionPointRec(mousePoint, rec3))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = controls;
			}
		}


	}

	void MenuDraw()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);


		DrawRectangle(rec1.x, rec1.y, rec1.width, rec1.height, BLACK);
		DrawRectangle(rec2.x, rec2.y, rec2.width, rec2.height, BLACK);
		DrawRectangle(rec3.x, rec3.y, rec3.width, rec3.height, BLACK);

		DrawText("jugar", rec1.x + 18, rec1.y + 10, 18, WHITE);
		DrawText("Creditos", rec2.x + 5, rec2.y + 10, 18, WHITE);
		DrawText("Controles", rec3.x + 5, rec3.y + 10, 18, WHITE);
		DrawText("Asteroids", screenWidth / 2 - 120, 100, 50, MAROON);

		EndDrawing();
	}

}