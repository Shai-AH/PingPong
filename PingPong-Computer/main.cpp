#include "raylib.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int player_score = 0;
int cpu_score = 0;
Color Green = {38, 185, 154, 255};
Color Dark_Green = {20, 60, 133, 255};
Color Light_Green = {129, 204, 184, 255};
Color Yellow = {243, 185, 154, 255};

int Rand(int maxrange){
    int num = (rand() % maxrange);
    return num;
}

class Ball{
public:
    float x, y;
    int speedX, speedY;
    int rad;
    Ball(float x, float y, int speedX, int speedY, int rad){
        this->x = x;
        this->y = y;
        this->speedX = speedX;
        this->speedY = speedY;
        this->rad = rad;
    }
    void Draw(){
        DrawCircle(x, y, rad, Yellow);
    }
    void Update(){
        x = x + speedX;
        y = y + speedY;
        if(y + rad >= GetScreenHeight() || y - rad <= 0){
            speedY *= -1;
        } //BOTTOM AND TOP COLLISION
        if(x + rad >= GetScreenWidth()){
            player_score++;
            resetBall();
        }
        else if(x - rad <= 0){
            cpu_score++;
            resetBall();
        } //RIGHT AND LEFT COLLISION
    }
    void resetBall(){
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        int resX = Rand(2);
        int resY = Rand(2);
        if(resX == 0) speedX *= -1;
        else speedX *= -1;
        if(resY == 0) speedY *= 1;
        else speedY *= -1;
    }
};

class Paddle{
protected:
    void LimitMovement(){
        if(posY <= 0){
            posY = 0;
        }
        if(posY + height >= GetScreenHeight()){
            posY = GetScreenHeight() - height;
        }
    }
public:
    float posX, posY;
    int width, height;
    int speed;
    Paddle(float px, float py, int w, int h){
        posX = px;
        posY = py;
        width = w;
        height = h;
        speed = 6;
    }
    void Draw(){
        DrawRectangleRounded((Rectangle{posX, posY, width, height}), 0.8, 0, WHITE);
    }
    void Update(){
        if(IsKeyDown(KEY_UP)){
            posY -= speed;
        }
        else if(IsKeyDown(KEY_DOWN)){
            posY += speed;
        }
        LimitMovement();
    }
};

class CPUpaddle : public Paddle{
public:
    CPUpaddle(float px, float py, int w, int h) : Paddle(px, py, w, h){
        speed = 5;
    };
    void Update(Ball ball){
        if(ball.x > GetScreenWidth() / 2){
            if(ball.y < posY + height/2){
                posY -= speed;
            }
            else if(ball.y > posY + height/2){
                posY += speed;
            }
            LimitMovement();
        }
    }
};

int main() {
    cout << "Starting the Game!" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "My Pong Game!");
    SetTargetFPS(120);

    Ball ball(screen_width/2, screen_height/2, 7, 7, 20);
    Paddle p1(10, screen_height/2 - 60, 25, 120);
    CPUpaddle p2(screen_width - 35, screen_height/2 - 60, 25, 120);

    while(WindowShouldClose() == false){

        ball.Update();
        p1.Update();
        p2.Update(ball);

        if(CheckCollisionCircleRec((Vector2{ball.x, ball.y}), ball.rad, (Rectangle{p1.posX, p1.posY, (float)p1.width, (float)p1.height}))){
            ball.speedX *= -1;
        }
        if(CheckCollisionCircleRec((Vector2{ball.x, ball.y}), ball.rad, (Rectangle{p2.posX, p2.posY, (float)p2.width, (float)p2.height}))){
            ball.speedX *= -1;
        }

        BeginDrawing();
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);
        ball.Draw();
        p1.Draw();
        p2.Draw();
        DrawText(TextFormat("%i",player_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",cpu_score), screen_width * 3/4 - 20, 20, 80, WHITE);
        EndDrawing();
    }



    CloseWindow();

    return 0;
}
