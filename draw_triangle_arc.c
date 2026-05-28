#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "math.h"

Mesh TriangleArc() {
    Mesh mesh = { 0 };

    const int segments = 60;

    // Hệ thống dùng Indices: 
    // Số đỉnh chỉ cần bằng tổng số mặt cắt (segments + 1) nhân cho 3 đỉnh mỗi mặt cắt
    mesh.vertexCount = (segments + 1) * 3;
    // Mỗi segment có 3 mặt bên, mỗi mặt bên cấu thành từ 2 tam giác = 6 tam giác/segment
    mesh.triangleCount = segments * 6; 

    // Cấp phát bộ nhớ cho đỉnh (Vertices) và chỉ số nối (Indices)
    mesh.vertices = (float*)MemAlloc(mesh.vertexCount * 3 * sizeof(float));
    mesh.indices = (unsigned short*)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    float modelRadius = 2.0f; 

    // Định nghĩa mặt cắt gốc ban đầu (Vòng lặp i = 0 sẽ dùng làm mặt đáy đầu tiên)
    Vector3 initA = {0.0f, modelRadius - 0.3f * sqrtf(3.0f), 0.0f};
    Vector3 initB = {-0.3f, modelRadius, 0.0f};
    Vector3 initC = {0.3f, modelRadius, 0.0f};

    Vector3 axis = {0.0f, 0.0f, 1.0f}; // Trục xoay Z
    
    // --- BƯỚC 1: NẠP TẤT CẢ TỌA ĐỘ ĐỈNH VÀO MẢNG VERTICES ---
    int v = 0;
    for (int i = 0; i <= segments; i++)
    {
        // Xoay tịnh tiến dần dần từ mặt cắt gốc ban đầu
        Vector3 A = Vector3RotateByAxisAngle(initA, axis, (float)i * (360.0f / segments) * DEG2RAD);
        Vector3 B = Vector3RotateByAxisAngle(initB, axis, (float)i * (360.0f / segments) * DEG2RAD);
        Vector3 C = Vector3RotateByAxisAngle(initC, axis, (float)i * (360.0f / segments) * DEG2RAD);

        // Kéo dãn theo trục Z để tạo thành hình lò xo uốn lượn lùi dần
        A.z = B.z = C.z = (float)i * 0.05f;

        mesh.vertices[v++] = A.x; mesh.vertices[v++] = A.y; mesh.vertices[v++] = A.z;
        mesh.vertices[v++] = B.x; mesh.vertices[v++] = B.y; mesh.vertices[v++] = B.z;
        mesh.vertices[v++] = C.x; mesh.vertices[v++] = C.y; mesh.vertices[v++] = C.z;
    }

    // --- BƯỚC 2: ĐỊNH NGHĨA QUY TẮC NỐI TAM GIÁC VÀO MẢNG INDICES ---
    int idx = 0;
    for (int i = 0; i < segments; i++)
    {
        // Xác định ID vị trí của các đỉnh thuộc mặt cắt Hiện Tại (i)
        int A = i * 3 + 0;
        int B = i * 3 + 1;
        int C = i * 3 + 2;

        // Xác định ID vị trí của các đỉnh thuộc mặt cắt Kế Tiếp (i + 1)
        int nextA = (i + 1) * 3 + 0;
        int nextB = (i + 1) * 3 + 1;
        int nextC = (i + 1) * 3 + 2;

        // Quy tắc nối các mặt tam giác theo chuẩn CCW (Ngược chiều kim đồng hồ)

        // ===== SIDE AB =====
        mesh.indices[idx++] = A; mesh.indices[idx++] = nextA; mesh.indices[idx++] = B;
        mesh.indices[idx++] = B; mesh.indices[idx++] = nextA; mesh.indices[idx++] = nextB;

        // ===== SIDE BC =====
        mesh.indices[idx++] = B; mesh.indices[idx++] = nextB; mesh.indices[idx++] = C;
        mesh.indices[idx++] = C; mesh.indices[idx++] = nextB; mesh.indices[idx++] = nextC;

        // ===== SIDE CA =====
        mesh.indices[idx++] = C; mesh.indices[idx++] = nextC; mesh.indices[idx++] = A;
        mesh.indices[idx++] = A; mesh.indices[idx++] = nextC; mesh.indices[idx++] = nextA;
    }

    return mesh;
}

int main()
{
    InitWindow(1280, 720, "Optimized Triangle Arc 3D (Indexed)");

    Camera3D camera = { 0 };
    camera.target = (Vector3){ 0, 0, 1.2f }; // Đẩy target camera dịch theo trục Z một chút vì mô hình dài ra
    camera.up = (Vector3){ 0, 1, 0 };
    camera.fovy = 45;
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh mesh = TriangleArc();
    UploadMesh(&mesh, false);

    Model cube = LoadModelFromMesh(mesh);

    SetTargetFPS(60);

    float camAngle = 0.0f;
    float camRadius = 6.0f; 

    while (!WindowShouldClose())
    {
        camAngle = camAngle + GetFrameTime() * 0.5f; 
        if (camAngle > 6.28318f) camAngle -= 6.28318f;

        camera.position.x = cosf(camAngle) * camRadius;
        camera.position.z = (sinf(camAngle) * camRadius) + 1.2f; // Đồng bộ quay camera quanh tâm hình lăng trụ
        camera.position.y = 4.0f;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Vẽ mô hình đặc màu đỏ
                DrawModel(cube, (Vector3){0,0,0}, 1.0f, RED);
                
                // Vẽ các đường lưới dây liên kết
                //DrawModelWires(cube, (Vector3){0,0,0}, 1.0f, BLACK);
                
                // Số đỉnh đã giảm xuống tối thiểu nên CPU/GPU xử lý cực kỳ nhẹ nhàng
                for (int i = 0; i < mesh.vertexCount; i++)
                {
                    Vector3 vPos = {
                        mesh.vertices[i*3 + 0],
                        mesh.vertices[i*3 + 1],
                        mesh.vertices[i*3 + 2]
                    };
                    DrawSphere(vPos, 0.02f, BLUE); 
                }

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(cube);
    CloseWindow();
    return 0;
}