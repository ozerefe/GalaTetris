#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool CheckButtonPressed(Vector2 mousePoint, Rectangle button)
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, button);
}

bool CheckMenuButtonPressed(Vector2 mousePoint, Rectangle button)
{
    return CheckButtonPressed(mousePoint, button);
}

bool CheckPausedScreenButtonPressed(Vector2 mousePoint, Rectangle continueButton, Rectangle exitButton)
{
    return CheckButtonPressed(mousePoint, continueButton) || CheckButtonPressed(mousePoint, exitButton);
}

int main()
{
    InitWindow(500, 620, "GALATetris");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Texture2D menuBackground = LoadTexture("Ark/fot.png");

    Game game = Game();

    bool inMenu = true;
    bool isPaused = false;

    int selectedOption = 0; 

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);

        if (inMenu)
        {
            // Menu codes
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();

                Rectangle playButton = {10, 530, 170, 60};
                if (CheckMenuButtonPressed(mousePoint, playButton))
                {
                    inMenu = false;
                }

                Rectangle exitButton = {330, 530, 170, 60};
                if (CheckMenuButtonPressed(mousePoint, exitButton))
                {
                    CloseWindow();
                }

                
                for (int i = 0; i < 3; ++i)
                {
                    Rectangle optionButton = {10, 10 + i * 40, 30, 30};
                    if (CheckButtonPressed(mousePoint, optionButton))
                    {
                        selectedOption = i;
                       
                    }
                }
            }
        }
        else
        {
            
            if (!isPaused)
            {
                game.HandleInput();
                if (EventTriggered(0.2))
                {
                    game.MoveBlockDown();
                }
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                isPaused = !isPaused;
            }

            if (isPaused)
            {
                int alpha = 255;

                BeginDrawing();
                ClearBackground(BLANK);
                DrawTexture(menuBackground, 0, 0, Fade(WHITE, alpha));
                DrawTextEx(font, "Oyun duraklatıldı", {GetScreenWidth() / 2 - MeasureText("Oyun duraklatıldı", 20) / 2, GetScreenHeight() / 2}, 20, 2, Fade(WHITE, alpha));
                DrawText("Press ENTER to resume or ESCAPE to exit", GetScreenWidth() / 2 - MeasureText("Press ENTER to resume or ESCAPE to exit", 20) / 2, GetScreenHeight() / 2 + 30, 20, Fade(WHITE, alpha));
                EndDrawing();

                alpha -= 5;

                if (IsKeyPressed(KEY_ENTER))
                {
                    isPaused = false;
                }

                if (IsKeyPressed(KEY_ESCAPE))
                {
                    CloseWindow();
                }
            }
            else
            {
                ClearBackground(RED);
            }
        }

        BeginDrawing();
        ClearBackground(inMenu ? WHITE : RED);

        if (inMenu)
        {
            DrawTexture(menuBackground, 0, 0, WHITE);

            DrawTextEx(font, "    TETRIS", {180, 15}, 60, 2, YELLOW);

            DrawTextEx(font, "GALA", {180, 15}, 60, 2, RED);
            DrawRectangleRounded({10, 530, 170, 60}, 0.3, 6, YELLOW);
            DrawTextEx(font, "Play Game", {25, 540}, 25, 2, BLACK);
            DrawRectangleRounded({330, 530, 170, 60}, 0.3, 6, YELLOW);
            DrawTextEx(font, "Exit Game", {345, 540}, 25, 2, BLACK);

          
            for (int i = 0; i < 3; ++i)
            {
                Rectangle optionRect = {10, 10 + i * 40, 30, 30};
                DrawRectangleRoundedLines(optionRect, 0.3, 6, 2, BLACK);

                if (selectedOption == i)
                {
                    DrawRectangleRounded(optionRect, 0.3, 6, Fade(RED, 0.5));
                }
                else
                {
                    DrawRectangleRounded(optionRect, 0.3, 6, YELLOW);
                }

                char optionText[2];
                sprintf(optionText, "%d", i);
                DrawText(optionText, optionRect.x + 10, optionRect.y + 7, 20, BLACK);
            }
            DrawTextEx(font, "Difficulty", {10, 125}, 20, 2, RED);
            DrawTextEx(font, "Ozer Efe ENGIN", {10,400}, 15, 2, RED);
        
            DrawText("Gerçekleri tarih yazar, tarih de GALATetris", 10, GetScreenHeight() - 20, 20, WHITE);
        }
        else
        {
            DrawTextEx(font, "Score", {365, 15}, 38, 2, BLACK);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, BLACK);
            if (game.gameOver)
            {
                DrawTextEx(font, "GAME OVER", {320, 450}, 38, 2, BLACK);
            }
            DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, YELLOW);
            char scoreText[10];
            sprintf(scoreText, "%d", game.score);
            Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, BLACK);
            DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, YELLOW);
            game.Draw();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
