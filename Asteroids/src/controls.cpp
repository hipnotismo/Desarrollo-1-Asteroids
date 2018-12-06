#include "controls.h"

#include "include/raylib.h"

#include "game.h"



namespace Game
{
	static Vector2 mouse;
	static Rectangle rect1;

	void controlsUpdate()
	{
		controlsDraw();
		mouse = GetMousePosition();

		rect1.x = 25;
		rect1.y = 25;
		rect1.height = 50;
		rect1.width = 100;

		if (CheckCollisionPointRec(mouse, rect1))
		{

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
			{
				Screens = menu;
			}
		}

	}


	void controlsDraw()
	{
		BeginDrawing();

		DrawRectangle(rect1.x, rect1.y, rect1.width, rect1.height, BLACK);

		ClearBackground(RAYWHITE);
		DrawText("atras", rect1.x + 18, rect1.y + 10, 20, WHITE);
		DrawText("Click izquierdo para disparar", 20, 200, 20, BLACK);
		DrawText("Click derecho para moverse", 20, 220, 20, BLACK);
		DrawText("P para pausar", 20, 240, 20, BLACK);
		DrawText("M durante pausa para desactivar la musica", 20, 260, 20, BLACK);
		DrawText("B para vlver al menu", 20, 280, 20, BLACK);

		

		EndDrawing();
	}

}