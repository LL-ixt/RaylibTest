#include "raylib.h"
#include "raymath.h"
#include "math.h"

int main()
{
    float radius = 700.0f;
    Vector2 center = {900.0f, 600.0f};
    Vector2 x = {0.0f, radius - 100.0f*sqrt(3.0f)};
    Vector2 y = {-100.0f, radius};
    Vector2 z = {100.0f, radius};
    float cur_angle = 0.0f;
    InitWindow(1920, 1280, "Test Rotation");
    Color transparent = {150, 150, 150, 100};
    SetTargetFPS(15);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), transparent);

            Vector2 temp_x = Vector2Rotate(x, cur_angle*DEG2RAD);
            Vector2 temp_y = Vector2Rotate(y, cur_angle*DEG2RAD);
            Vector2 temp_z = Vector2Rotate(z, cur_angle*DEG2RAD);
            cur_angle += GetFrameTime()*50.0f;
            DrawTriangle(Vector2Add(center, temp_x), Vector2Add(center, temp_y), Vector2Add(center, temp_z), YELLOW);
            
        EndDrawing();
    }
    CloseWindow();
}