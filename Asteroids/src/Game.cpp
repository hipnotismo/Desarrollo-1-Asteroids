#include "game.h"

#include <cmath>

#include "include/raylib.h"

#include "menu.h"
#include "credits.h"
#include "controls.h"


namespace Game 
{

	GameScreen Screens;

	struct Player 
	{
		Vector2 position;
		Vector2 speed;
		float acceleration;
		float rotation;
		Vector3 collider;
		Color color;
	};

	struct Shoot 
	{
		Vector2 position;
		Vector2 speed;
		float radius;
		float rotation;
		int lifeSpawn;
		bool active;
		Color color;
	};

	struct Meteor 
	{
		Vector2 position;
		Vector2 speed;
		float radius;
		bool active;
		Color color;
	};

	int screenWidth = 800;
	int screenHeight = 450;

	static bool gameOver;
	static bool pause;
	static bool Music_pause;
	static bool victory;

	static float shipHeight;
	static float PLAYER_BASE_SIZE = 20.0f;
	static float  PLAYER_SPEED = 60000.0f;

	

	static int midMeteorsCount;
	static int smallMeteorsCount;
	static int destroyedMeteorsCount;
	static int frameWidth;
	static int frameHeight;
	static const int PLAYER_MAX_SHOOTS = 10;
	static const int METEORS_SPEED  = 40;
	static const int MAX_BIG_METEORS = 4;
	static const int  MAX_MEDIUM_METEORS = 8;
	static const int MAX_SMALL_METEORS = 16;

	static Player player;
	static Shoot shoot[PLAYER_MAX_SHOOTS];
	static Meteor bigMeteor[MAX_BIG_METEORS];
	static Meteor mediumMeteor[MAX_MEDIUM_METEORS];
	static Meteor smallMeteor[MAX_SMALL_METEORS];

	static Sound shot;
	static Music music;
	static Rectangle sourceRec;
	static Rectangle volver;
	static Rectangle music_rec;
	static Rectangle destRec;
	static Vector2 origin;
	static Vector2 point;

	
	int core()
	{

		InitWindow(screenWidth, screenHeight, "Asteroids");
		
		InitGame();

		while (!WindowShouldClose())
		{
			Change();

		}
		UnloadSound(shot);
		UnloadMusicStream(music);
		UnloadGame();

		CloseAudioDevice();
		CloseWindow();

		return 0;
	}

	void InitGame()
	{

		//sound
		InitAudioDevice();
		shot = LoadSound("res/Jump.wav");
		music = LoadMusicStream("res/background.ogg");
		PlayMusicStream(music);


		int posx, posy;
		int velx, vely;
		bool correctRange = false;
		victory = false;
		pause = false;
		Music_pause = false;

		//static Shoot shoot[PLAYER_MAX_SHOOTS];


		shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);

		point = GetMousePosition();

		sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight };
		destRec = { (float)screenWidth / 2, (float)screenHeight / 2, (float)frameWidth * 2, (float)frameHeight * 2 };
		origin = { (float)frameWidth, (float)frameHeight };

		// Initialization player
		player.position = { (float)screenWidth / 2, (float)screenHeight / 2 - (float)shipHeight / 2 };
		player.speed = { 0, 0 };
		player.acceleration = 0;
		player.rotation = 0;
		player.collider = { player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight / 2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight / 2.5f), 12 };
		player.color = LIGHTGRAY;

		destroyedMeteorsCount = 0;

		// Initialization shoot
		for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
		{
			shoot[i].position = { 0, 0 };
			shoot[i].speed = { 0, 0 };
			shoot[i].radius = 2;
			shoot[i].active = false;
			shoot[i].lifeSpawn = 0;
			shoot[i].color = WHITE;
		}

		for (int i = 0; i < MAX_BIG_METEORS; i++)
		{
			posx = GetRandomValue(0, screenWidth);

			while (!correctRange)
			{
				if (posx > screenWidth / 2 - 150 && posx < screenWidth / 2 + 150) posx = GetRandomValue(0, screenWidth);
				else correctRange = true;
			}

			correctRange = false;

			posy = GetRandomValue(0, screenHeight);

			while (!correctRange)
			{
				if (posy > screenHeight / 2 - 150 && posy < screenHeight / 2 + 150)  posy = GetRandomValue(0, screenHeight);
				else correctRange = true;
			}

			bigMeteor[i].position = { (float)posx, (float)posy };

			correctRange = false;
			velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
			vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

			while (!correctRange)
			{
				if (velx == 0 && vely == 0)
				{
					velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
					vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
				}
				else correctRange = true;
			}

			bigMeteor[i].speed = { (float)velx, (float)vely };
			bigMeteor[i].radius = 40;
			bigMeteor[i].active = true;
			bigMeteor[i].color = BLUE;
		}

		for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
		{
			mediumMeteor[i].position = { -100, -100 };
			mediumMeteor[i].speed = { 0, 0 };
			mediumMeteor[i].radius = 20;
			mediumMeteor[i].active = false;
			mediumMeteor[i].color = BLUE;
		}

		for (int i = 0; i < MAX_SMALL_METEORS; i++)
		{
			smallMeteor[i].position = { -100, -100 };
			smallMeteor[i].speed = { 0, 0 };
			smallMeteor[i].radius = 10;
			smallMeteor[i].active = false;
			smallMeteor[i].color = BLUE;
		}

		midMeteorsCount = 0;
		smallMeteorsCount = 0;
	}

	void UpdateGame()
	{
		if (!gameOver)
		{
			if (IsKeyPressed('P')) pause = !pause;

			if (!pause)
			{
				UpdateMusicStream(music);

				music_rec.x = 50;
				music_rec.y = 50;
				music_rec.height = 50;
				music_rec.width = 190;

				float AngleRad = atan2(GetMousePosition().y - player.position.y, GetMousePosition().x - player.position.x);
				float AngleDeg = (180 / PI) * AngleRad;
				player.rotation = AngleDeg+90;


				// Player logic: speed
				player.speed.x = sin(player.rotation*DEG2RAD)*PLAYER_SPEED * GetFrameTime() ;
				player.speed.y = cos(player.rotation*DEG2RAD)*PLAYER_SPEED * GetFrameTime() ;

				// Player logic: acceleration
				if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
				{
					if (player.acceleration < 10) player.acceleration += 1 * GetFrameTime() ;
				}
				else
				{
					if (player.acceleration > 0) player.acceleration -= 0.5f * GetFrameTime() ;
					else if (player.acceleration < 0) player.acceleration = 0 * GetFrameTime() ;
				}
				

				// Player logic: movement
				player.position.x += (player.speed.x*player.acceleration) * GetFrameTime() ;
				player.position.y -= (player.speed.y*player.acceleration) * GetFrameTime() ;

				// Collision logic: player vs walls
				if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
				else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
				if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
				else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

				// Player shoot logic
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
				{
					for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
					{
						if (!shoot[i].active)
						{
							shoot[i].position = { player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight) } ;
							shoot[i].active = true;
							shoot[i].speed.x = (1.5*sin(player.rotation*DEG2RAD)*PLAYER_SPEED + player.speed.x) * GetFrameTime();
							shoot[i].speed.y = (1.5*cos(player.rotation*DEG2RAD)*PLAYER_SPEED + player.speed.y) * GetFrameTime();
							shoot[i].rotation = player.rotation;
							break;
							PlaySound(shot);

						}
					}
				}

				// Shoot life timer
				for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
				{
					if (shoot[i].active) shoot[i].lifeSpawn++;
				}

				// Shot logic
				for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
				{
					if (shoot[i].active)
					{
						// Movement
						shoot[i].position.x += shoot[i].speed.x * GetFrameTime() ;
						shoot[i].position.y -= shoot[i].speed.y * GetFrameTime() ;

						// Collision logic: shoot vs walls
						if (shoot[i].position.x > screenWidth + shoot[i].radius)
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
						}
						else if (shoot[i].position.x < 0 - shoot[i].radius)
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
						}
						if (shoot[i].position.y > screenHeight + shoot[i].radius)
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
						}
						else if (shoot[i].position.y < 0 - shoot[i].radius)
						{
							shoot[i].active = false;
							shoot[i].lifeSpawn = 0;
						}

						// Life of shoot
						if (shoot[i].lifeSpawn >= 12000)
						{
							shoot[i].position = { 0, 0 };
							shoot[i].speed = { 0, 0 };
							shoot[i].lifeSpawn = 0;
							shoot[i].active = false;
						}
					}
				}

				// Collision logic: player vs meteors
				player.collider = { player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight / 2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight / 2.5f), 12 };

				for (int a = 0; a < MAX_BIG_METEORS; a++)
				{
					if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, bigMeteor[a].position, bigMeteor[a].radius) && bigMeteor[a].active) gameOver = true;
				}

				for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
				{
					if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active) gameOver = true;
				}

				for (int a = 0; a < MAX_SMALL_METEORS; a++)
				{
					if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active) gameOver = true;
				}

				// Meteors logic: big meteors
				for (int i = 0; i < MAX_BIG_METEORS; i++)
				{
					if (bigMeteor[i].active)
					{
						// Movement
						bigMeteor[i].position.x += bigMeteor[i].speed.x * GetFrameTime() ;
						bigMeteor[i].position.y += bigMeteor[i].speed.y * GetFrameTime() ;

						// Collision logic: meteor vs wall
						if (bigMeteor[i].position.x > screenWidth + bigMeteor[i].radius) bigMeteor[i].position.x = -(bigMeteor[i].radius);
						else if (bigMeteor[i].position.x < 0 - bigMeteor[i].radius) bigMeteor[i].position.x = screenWidth + bigMeteor[i].radius;
						if (bigMeteor[i].position.y > screenHeight + bigMeteor[i].radius) bigMeteor[i].position.y = -(bigMeteor[i].radius);
						else if (bigMeteor[i].position.y < 0 - bigMeteor[i].radius) bigMeteor[i].position.y = screenHeight + bigMeteor[i].radius;
					}
				}

				// Meteors logic: medium meteors
				for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
				{
					if (mediumMeteor[i].active)
					{
						// Movement
						mediumMeteor[i].position.x += mediumMeteor[i].speed.x * GetFrameTime() ;
						mediumMeteor[i].position.y += mediumMeteor[i].speed.y * GetFrameTime() ;

						// Collision logic: meteor vs wall
						if (mediumMeteor[i].position.x > screenWidth + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
						else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = screenWidth + mediumMeteor[i].radius;
						if (mediumMeteor[i].position.y > screenHeight + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
						else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = screenHeight + mediumMeteor[i].radius;
					}
				}

				// Meteors logic: small meteors
				for (int i = 0; i < MAX_SMALL_METEORS; i++)
				{
					if (smallMeteor[i].active)
					{
						// Movement
						smallMeteor[i].position.x += smallMeteor[i].speed.x * GetFrameTime() ;
						smallMeteor[i].position.y += smallMeteor[i].speed.y * GetFrameTime() ;

						// Collision logic: meteor vs wall
						if (smallMeteor[i].position.x > screenWidth + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
						else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = screenWidth + smallMeteor[i].radius;
						if (smallMeteor[i].position.y > screenHeight + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
						else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = screenHeight + smallMeteor[i].radius;
					}
				}

				// Collision logic: player-shoots vs meteors
				for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
				{
					if ((shoot[i].active))
					{
						for (int a = 0; a < MAX_BIG_METEORS; a++)
						{
							if (bigMeteor[a].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, bigMeteor[a].position, bigMeteor[a].radius))
							{
								shoot[i].active = false;
								shoot[i].lifeSpawn = 0;
								bigMeteor[a].active = false;
								destroyedMeteorsCount++;

								for (int j = 0; j < 2; j++)
								{
									if (midMeteorsCount % 2 == 0)
									{
										mediumMeteor[midMeteorsCount].position = { bigMeteor[a].position.x, bigMeteor[a].position.y };
										mediumMeteor[midMeteorsCount].speed = { cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1};
									}
									else
									{
										mediumMeteor[midMeteorsCount].position = { bigMeteor[a].position.x, bigMeteor[a].position.y };
										mediumMeteor[midMeteorsCount].speed = { cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED};
									}

									mediumMeteor[midMeteorsCount].active = true;
									midMeteorsCount++;
								}
								//bigMeteor[a].position = (Vector2){-100, -100};
								bigMeteor[a].color = RED;
								a = MAX_BIG_METEORS;
							}
						}

						for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
						{
							if (mediumMeteor[b].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, mediumMeteor[b].position, mediumMeteor[b].radius))
							{
								shoot[i].active = false;
								shoot[i].lifeSpawn = 0;
								mediumMeteor[b].active = false;
								destroyedMeteorsCount++;

								for (int j = 0; j < 2; j++)
								{
									if (smallMeteorsCount % 2 == 0)
									{
										smallMeteor[smallMeteorsCount].position = { mediumMeteor[b].position.x, mediumMeteor[b].position.y };
										smallMeteor[smallMeteorsCount].speed = { cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1};
									}
									else
									{
										smallMeteor[smallMeteorsCount].position = { mediumMeteor[b].position.x, mediumMeteor[b].position.y };
										smallMeteor[smallMeteorsCount].speed = { cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED};
									}

									smallMeteor[smallMeteorsCount].active = true;
									smallMeteorsCount++;
								}
								//mediumMeteor[b].position = (Vector2){-100, -100};
								mediumMeteor[b].color = GREEN;
								b = MAX_MEDIUM_METEORS;
							}
						}

						for (int c = 0; c < MAX_SMALL_METEORS; c++)
						{
							if (smallMeteor[c].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, smallMeteor[c].position, smallMeteor[c].radius))
							{
								shoot[i].active = false;
								shoot[i].lifeSpawn = 0;
								smallMeteor[c].active = false;
								destroyedMeteorsCount++;
								smallMeteor[c].color = YELLOW;
								// smallMeteor[c].position = (Vector2){-100, -100};
								c = MAX_SMALL_METEORS;
							}
						}
					}
				}
			}

			else
				//Menu de pausa
			{				
				if (IsKeyPressed(KEY_M))
				{
					Music_pause = !Music_pause;

					if (Music_pause) PauseMusicStream(music);
					else ResumeMusicStream(music);					
				}
			}
			if (IsKeyPressed(KEY_B))
			{
				Screens = menu;

			}

			if (destroyedMeteorsCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS) victory = true;
		}
		else
		{
			//Cambiar para que valla a una pantalla de game over
			if (IsKeyPressed(KEY_ENTER))
			{
				InitGame();
				gameOver = false;
			}
		}
	}

	void DrawGame()
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		if (!gameOver)
		{
			// Draw spaceship
			Vector2 v1 = { player.position.x + sinf(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cosf(player.rotation*DEG2RAD)*(shipHeight) };
			Vector2 v2 = { player.position.x - cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE / 2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE / 2) };
			Vector2 v3 = { player.position.x + cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE / 2), player.position.y + sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE / 2) };
			DrawTriangle(v1, v2, v3, MAROON);

			// Draw meteors
			for (int i = 0; i < MAX_BIG_METEORS; i++)
			{
				if (bigMeteor[i].active) DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, DARKGRAY);
				else DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
			}

			for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
			{
				if (mediumMeteor[i].active) DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, GRAY);
				else DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
			}

			for (int i = 0; i < MAX_SMALL_METEORS; i++)
			{
				if (smallMeteor[i].active) DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, GRAY);
				else DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
			}

			// Draw shoot
			for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
			{
				if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, BLACK);
			}

			if (victory) DrawText("VICTORY", screenWidth / 2 - MeasureText("VICTORY", 20) / 2, screenHeight / 2, 20, LIGHTGRAY);

			if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
			if (pause) DrawText("M durante pausa para desactivar la musica", 20, 260, 20, BLACK);
			if (pause) DrawText("B para volver al menu", 20, 280, 20, BLACK);
		


		}
		else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

		EndDrawing();
	}

	void UnloadGame()
	{
		// TODO: Unload all dynamic loaded data (textures, sounds, models...)
	}

	void Change()
	{
		switch (Screens)
		{
		case menu:
		{
			MenuUpdate();

		} break;
		case credits:
		{
			CreditsUpdate();
		} break;

		case game:
		{
			UpdateDrawFrame();
		} break;
		case controls:
		{
			controlsUpdate();
		} break;

		}
	}

	void UpdateDrawFrame()
	{
		UpdateGame();
		DrawGame();
	}
}