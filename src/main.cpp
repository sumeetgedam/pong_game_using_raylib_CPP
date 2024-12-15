#include <raylib.h>
#include<iostream>

using namespace std;

Color green = Color{0, 228, 48, 255 };
Color darkGreen = Color{0, 117, 44, 255};
Color lightGreen = Color{129, 204, 184, 255};
Color yellow = Color{253, 249, 0, 255};


int playerScore = 0, cpuScore = 0;

class Ball{
    public:
        float x, y;
        int speedX, speedY;
        int radius;

        void Draw(){
            DrawCircle(x, y, radius, yellow);
        }

        void updateBallPosition(){
            x += speedX;
            y += speedY;
            
            if(y + radius >= GetScreenHeight() || y - radius <= 0){
                speedY *= -1;
            }

            if(x + radius >= GetScreenWidth()){
                cpuScore++;
                resetBall();
            }
            if(x - radius <= 0){
                playerScore++;
                resetBall();
            }
        }

        void resetBall(){
            x = GetScreenWidth() / 2;
            y = GetScreenHeight() / 2;

            int speedChoices[2] = {-1, 1};
            speedX *= speedChoices[GetRandomValue(0, 1)];
            speedY *= speedChoices[GetRandomValue(0, 1)];
        }
};

class Paddle{

    protected:
        void limitPaddleMovement(){
            if(y <= 0){
                y = 0;
            }
            if(y + height >= GetScreenHeight()){
                y = GetScreenHeight() - height;
            }
        }

    public:
        float x, y;
        float width, height;
        int speed;

        void Draw(){
            DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0,  WHITE); 
        }

        void updatePaddle(){
            if(IsKeyDown(KEY_UP)){
                y = y-speed;
            }
            if(IsKeyDown(KEY_DOWN)){
                y = y+speed;
            }
            limitPaddleMovement();
        }
};

class CPUPaddle: public Paddle{
    public: 

        void updatePaddle(int ballY){
            // if the ball position is above paddle 
            // move the paddle up
            if(y + height / 2 > ballY){
                y -= speed;
            }
            if(y + height / 2 <= ballY){
                y += speed;
            }
            limitPaddleMovement();
        }

};

Ball ball;
Paddle paddle;
CPUPaddle cpuPaddle;


int main() 
{
    cout << "Pong game started....." << endl;
    
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Ping Pong");

    SetTargetFPS(45);

    ball.radius = 20;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speedX = 7;
    ball.speedY = 7;

    paddle.x = screenWidth - 35;
    paddle.y = screenHeight / 2 - 60;
    paddle.width = 25;
    paddle.height = 120;
    paddle.speed = 6;

    cpuPaddle.x =  10;
    cpuPaddle.y = screenHeight / 2 - 60;
    cpuPaddle.width = 25;
    cpuPaddle.height = 120;
    cpuPaddle.speed = 6;



    while(WindowShouldClose() == false){
        
        BeginDrawing();

        ball.updateBallPosition();
        paddle.updatePaddle();
        cpuPaddle.updatePaddle(ball.y);

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle.x, paddle.y, paddle.width, paddle.height})){
            ball.speedX *= -1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpuPaddle.x, cpuPaddle.y, cpuPaddle.width, cpuPaddle.height})){
            ball.speedX *= -1;
        }

        ClearBackground(darkGreen);

        DrawRectangle(screenWidth / 2, 0, screenWidth / 2, screenHeight, green);
        DrawCircle(screenWidth / 2, screenHeight / 2, 150, lightGreen);

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        ball.Draw();

        // rectangle is drawn from the top left corner, so to center the same
        // we need to remove half the heigh of rectangel that is 60 in this case
        // DrawRectangle(10, screenHeight / 2 - 60, 25, 120, WHITE); 
        cpuPaddle.Draw();

        // now to get a 10 gap between at th second side
        // rectangle width is 24 and we need 10 gap so need to subtract 35 from screen width
        // DrawRectangle(screenWidth - 35, screenHeight / 2 - 60, 25, 120, WHITE);
        paddle.Draw();
        
        DrawText(TextFormat("CPU Score : %i", cpuScore), screenWidth / 4 - 140, 20, 40, WHITE);
        DrawText(TextFormat("Your Score : %i", playerScore), 3* screenWidth / 4 - 120, 20, 40, WHITE);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}