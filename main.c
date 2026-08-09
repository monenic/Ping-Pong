#include <raylib.h>

void Reset(float *ballX, float *ballY, float *ballSpeedX, 
           float *ballSpeedY, float *playerSpeed, 
           float *cpuSpeed, float *cpuError, int screenWidth, int screenHeight) {

    *ballX = screenWidth / 2.0f;
    *ballY = screenHeight / 2.0f;

    *ballSpeedX = GetRandomValue(0, 1) == 0 ? -4.0f : 4.0f;
    *ballSpeedY = GetRandomValue(0, 1) == 0 ? -4.0f : 4.0f;

    *playerSpeed = 4.3f;
    *cpuSpeed = 4.3f;

    *cpuError = GetRandomValue(-60, 60);
}

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    float ballX = screenWidth / 2.0f;
    float ballY = screenHeight / 2.0f;

    float ballSpeedX = GetRandomValue(0, 1) == 0 ? -4.0f : 4.0f;
    float ballSpeedY = GetRandomValue(0, 1) == 0 ? -4.0f : 4.0f;

    int ballRadius = 15;

    Rectangle player = {1240, 350, 5, 100};
    float playerSpeed = 4.3f;

    Rectangle cpu = {40, 350, 5, 100};
    float cpuSpeed = 4.3f;

    int cpuVision = 640;
    float cpuError = 0.0f;

    int playerScore = 0;
    int cpuScore = 0;

    float cpuReactionTimer = 0.0f;
    float cpuReactionDelay = 0.05f;
    float cpuTarget = ballY;

    InitWindow(screenWidth, screenHeight, "Ping Pong");
    SetTargetFPS(120);

    while (!WindowShouldClose()) {

        cpuReactionTimer += GetFrameTime();

        if (cpuReactionTimer >= cpuReactionDelay) {
            cpuTarget = ballY + cpuError;
            cpuReactionTimer = 0.0f;
        }

        //movimentar jogador
        if (IsKeyDown(KEY_W) && player.y > 0) {
            player.y -= playerSpeed;
        }

        if (IsKeyDown(KEY_S) && player.y + player.height < screenHeight) {
            player.y += playerSpeed;
        }

        //movimentar cpu
        if (ballX < cpuVision) {
            if (cpuTarget > cpu.y + cpu.height / 2 && cpu.y + cpu.height < screenHeight) {
                cpu.y += cpuSpeed;
            }

            if (cpuTarget < cpu.y + cpu.height / 2  && cpu.y > 0) {
                cpu.y -= cpuSpeed;
            }
        }
        
        //movimento da bola 
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        //pontuacao cpu
        if (ballX + ballRadius >= screenWidth) {
            cpuScore += 1;
            Reset(&ballX, &ballY, &ballSpeedX, &ballSpeedY, &playerSpeed, &cpuSpeed, &cpuError, screenWidth, screenHeight);
        }

        //pontuacao player
        if (ballX - ballRadius <= 0) {
            playerScore += 1;
            Reset(&ballX, &ballY, &ballSpeedX, &ballSpeedY, &playerSpeed, &cpuSpeed, &cpuError, screenWidth, screenHeight);
        }

        //colisao com teto e chao
        if (ballY + ballRadius >= screenHeight || ballY - ballRadius <= 0) {
            ballSpeedY *= -1;
        }

        //checa colisao com o jogador
        if (CheckCollisionCircleRec((Vector2){ballX, ballY}, ballRadius, player)) {
            if (ballY < player.y) {
                ballSpeedY *= -1;
                ballY = player.y - ballRadius;
            }
            else if (ballY > player.y + player.height) {
                ballSpeedY *= -1;
                ballY = player.y + player.height + ballRadius;
            }
            else if (ballSpeedX > 0) {
                ballSpeedX *= -1;
                ballX = player.x - ballRadius;

                if (ballSpeedX > 0) {
                    ballSpeedX += 0.5;
                }
                else {
                    ballSpeedX -= 0.5;
                }

                if (ballSpeedY > 0) {
                    ballSpeedY += 0.5;
                }
                else {
                    ballSpeedY -= 0.5;
                }

                playerSpeed += 0.5;
                cpuSpeed += 0.5;
            }
        }

        //checa colisao com a cpu
        if (CheckCollisionCircleRec((Vector2){ballX, ballY}, ballRadius, cpu)) {
            if (ballY < cpu.y) {
                ballSpeedY *= -1;
                ballY = cpu.y - ballRadius;
            }
            else if (ballY > cpu.y + cpu.height) {
                ballSpeedY *= -1;
                ballY = cpu.y + cpu.height + ballRadius;
            }
            else if (ballSpeedX < 0) {
                ballSpeedX *= -1;
                ballX = cpu.x + cpu.width + ballRadius;

                if (ballSpeedX > 0) {
                    ballSpeedX += 0.5;
                }
                else {
                    ballSpeedX -= 0.5;
                }

                if (ballSpeedY > 0) {
                    ballSpeedY += 0.5;
                }
                else {
                    ballSpeedY -= 0.5;
                }

                playerSpeed += 0.5;
                cpuSpeed += 0.5;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(ballX, ballY, ballRadius, WHITE);

        DrawRectangleRec(player, RED);
        DrawRectangleRec(cpu, BLUE);

        DrawText(TextFormat("%d", cpuScore), screenWidth / 4 - 20, 50, 50, BLUE);
        DrawText(TextFormat("%d", playerScore), 3 * screenWidth / 4 - 20, 50, 50, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}