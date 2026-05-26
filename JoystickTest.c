#include "raylib.h"
#include "stdio.h"
#include "math.h"
int main() 
{
    float radius = 100;
    Vector2 leftCenter = {200, 200}, rightCenter = {500, 200};
    InitWindow(800, 600, "Joystick");
    SetTargetFPS(60);
    while (!WindowShouldClose()) 
    {
        float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
        float leftStickLength = sqrt(leftStickX*leftStickX + leftStickY*leftStickY);

        float rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
        float rightStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
        float rightStickLength = sqrt(rightStickX*rightStickX + rightStickY*rightStickY);

        Vector2 leftPos = leftCenter, rightPos = rightCenter;

        if (leftStickLength > 0.1f)
        {
            leftPos.x = leftCenter.x + (leftStickX / leftStickLength) * radius;
            leftPos.y = leftCenter.y + (leftStickY / leftStickLength) * radius;
        }
        
        if (rightStickLength > 0.1f)
        {
            rightPos.x = rightCenter.x + (rightStickX / rightStickLength) * radius;
            rightPos.y = rightCenter.y + (rightStickY / rightStickLength) * radius;
        }
        
        BeginDrawing();
            ClearBackground(WHITE);

            if (IsGamepadAvailable(2))
            {
                DrawText("Gamepad connected!", 10, 10, 20, GREEN);
            }
            else
            {
                DrawText("No gamepad!", 10, 10, 20, RED);
            }

            // Analog Circle
            DrawCircleLinesV(leftCenter, radius, BLACK);
            DrawCircleLinesV(rightCenter, radius, BLACK);
            
            // Joystick Position
            DrawCircleV(leftPos, 15, BLUE);
            DrawCircleV(rightPos, 15, RED);
            
        EndDrawing();
    }

    CloseWindow();
}