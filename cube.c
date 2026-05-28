#include "raylib.h"
#include "raymath.h"
#include "string.h"
#include "math.h"

#define PushVertex(p) \
    do { \
        mesh.vertices[v++] = p.x; \
        mesh.vertices[v++] = p.y; \
        mesh.vertices[v++] = p.z; \
    } while(0)

int main()
{
    InitWindow(800, 600, "Custom Cube");

    Camera3D camera = { 0 };
    camera.target = (Vector3){ 0, 0, 0 };
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh mesh = { 0 };

    const int segments = 10;
    const float width = 1.0f;

    mesh.vertexCount = segments * 18;
    mesh.triangleCount = segments * 6;

    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    
    int v = 0;
    //Hình lăng trụ cong có mặt đáy là hình tam giác vuông, mặt bên cong theo đường sin (trục y).
    for (int i = 0; i < segments; i++)
    {
        float x0 = i * 0.2f;
        float x1 = (i + 1) * 0.2f;

        Vector3 A  = {x0, sinf(x0),     0};
        Vector3 B  = {x0, sinf(x0)+1,   0};
        Vector3 C  = {x0, sinf(x0),     1};

        Vector3 A2 = {x1, sinf(x1),     0};
        Vector3 B2 = {x1, sinf(x1)+1,   0};
        Vector3 C2 = {x1, sinf(x1),     1};

        // ===== SIDE AB =====

        PushVertex(A);
        PushVertex(B);
        PushVertex(A2);

        PushVertex(B);
        PushVertex(B2);
        PushVertex(A2);

        // ===== SIDE BC =====

        PushVertex(B);
        PushVertex(C);
        PushVertex(B2);

        PushVertex(C);
        PushVertex(C2);
        PushVertex(B2);

        // ===== SIDE CA =====

        PushVertex(C);
        PushVertex(A);
        PushVertex(C2);

        PushVertex(A);
        PushVertex(A2);
        PushVertex(C2);
    }


    UploadMesh(&mesh, false);

    Model cube = LoadModelFromMesh(mesh);

    SetTargetFPS(60);

    Vector2 direction = { 1.0f, 0.0f };
    float angleInRadians = DEG2RAD * 30.0f; // raymath requires radians
    Vector2 rotatedDirection = Vector2Rotate(direction, angleInRadians);

    float angle = 0.0f, radius = 5.0f;

    while (!WindowShouldClose())
    {
        angle = angle + GetFrameTime()*1.0f;
        if (angle > 360.0f) angle -= 360.0f;

        camera.position.x = cosf(angle) * radius;
        camera.position.z = sinf(angle) * radius;
        camera.position.y = 3.0f;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawModel(cube, (Vector3){0,0,0}, 1.0f, RED);
        DrawModelWires(cube, (Vector3){0,0,0}, 1.0f, BLACK);
        for (int i = 0; i < mesh.vertexCount; i++)
        {
            Vector3 v =
            {
                mesh.vertices[i*3 + 0],
                mesh.vertices[i*3 + 1],
                mesh.vertices[i*3 + 2]
            };

            

            DrawSphere(v, 0.05f, BLUE);
        }

        DrawGrid(10, 1.0f);

        EndMode3D();

        EndDrawing();
    }

    UnloadModel(cube);

    CloseWindow();
}