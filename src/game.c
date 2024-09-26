#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <stdbool.h>

#define mapWidth 150
#define mapHeight 25
char map[mapHeight][mapWidth + 1];

typedef struct SMario {
    float x, y; // движение
    float width, height; // ширина и высота
    float vertSpeed; // Вертикальная скорость
    BOOL IsFly;
    char cType;
    float horizSpeed;
} TMario;

TMario mario;
TMario* brick = NULL;
int brickLength;

TMario* moving = NULL;
int movingLength;

int level = 1;

void ClearMap() {
    for (int i = 0; i < mapWidth; i++) 
         map[0][i] = '.';  // заполнение массива от 1 до 80
    map[0][mapWidth] = '\0';
    for (int j = 1; j < mapHeight; j++)
        strcpy(map[j], map[0]);
}

void ShowMap(int offsetX) {
    for (int j = 0; j < mapHeight; j++)
        printf("%.*s\n", mapWidth, map[j] + offsetX);
}

void SetMarioPos(TMario* mario, float xPos, float yPos) {
    mario->x = xPos;
    mario->y = yPos;
}

void InitMario(TMario* mario, float xPos, float yPos, float oWidth, float oHeight, char inType) {
    SetMarioPos(mario, xPos, yPos);
    mario->width = oWidth;
    mario->height = oHeight;
    mario->vertSpeed = 0; // начальная скорость
    mario->cType = inType;
    mario->horizSpeed = 0.2;
}

BOOL IsCollision(TMario o1, TMario o2);
void CreateLevel(int lvl);

void VertMoveMario(TMario* mario) {
    mario->IsFly = TRUE;
    mario->vertSpeed += 0.05;
    SetMarioPos(mario, mario->x, mario->y + mario->vertSpeed);
    for (int i = 0; i < brickLength; i++)
        if (IsCollision(*mario, brick[i])) {
            mario->y -= mario->vertSpeed;
            mario->vertSpeed = 0;
            mario->IsFly = FALSE;
            if (brick[i].cType == '+') {
                ++level;
                if (level > 2) level = 1;
                CreateLevel(level);
                Sleep(1500);
            }
            break;
        }
}

void HorizonMoveMario(TMario* mario) {
    mario[0].x += mario[0].horizSpeed;

    for (int i = 0; i < brickLength; i++)
        if (IsCollision(mario[0], brick[i])) {
            mario[0].x -= mario[0].horizSpeed;
            mario[0].horizSpeed = -mario[0].horizSpeed;
            return;
        }
        TMario tmp = *mario;
        VertMoveMario(&tmp);
        if (tmp.IsFly == TRUE) {
        mario[0].x -= mario[0].horizSpeed;
            mario[0].horizSpeed = -mario[0].horizSpeed;
        }

}

BOOL IsPosInMap(int x, int y) {
    return ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
}

void PutMarioOnMap(TMario mario) {
    int ix = (int)round(mario.x);
    int iy = (int)round(mario.y);
    int iWidth = (int)round(mario.width);
    int iHeight = (int)round(mario.height);

    for (int i = ix; i < (ix + iWidth); i++)
        for (int j = iy; j < (iy + iHeight); j++)
            if (IsPosInMap(i, j)) {
                map[j][i] = mario.cType;
            }
}

void setCur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HorizonMoveMap(float dx) {
    mario.x -= dx;
    for (int i = 0; i < brickLength; i++) {
        if (IsCollision(mario, brick[i])) {
            mario.x += dx;
            return;
        }
    }
    for (int i = 0; i < brickLength; i++)
        brick[i].x += dx;
    for (int i = 0; i < movingLength; i++)
        moving[i].x += dx;
}

BOOL IsCollision(TMario o1, TMario o2) {
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
        ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
}

void CreateLevel(int lvl) {
    InitMario(&mario, 35, 10, 3, 3, '&');
    if (lvl == 1) {
        // Определяем количество объектов
        brickLength =  6;
        // Выделяем или перераспределяем память для объектов
        brick = realloc(brick, sizeof(*brick) * brickLength);

        // Инициализация всех объектов уровня
        InitMario(brick + 0, 20, 20, 40, 5, '#');
        InitMario(brick + 1, 60, 15, 10, 10, '#');
        InitMario(brick + 2, 80, 20, 20, 5, '#');
        InitMario(brick + 3, 120, 15, 10, 10, '#');
        InitMario(brick + 4, 150, 20, 40, 5, '#');
        InitMario(brick + 5, 180, 15, 5, 5, '+'); // Объект завершения уровня
        movingLength = 1;
        moving = realloc(moving, sizeof(*moving) * movingLength);
        InitMario(moving+0, 25, 10, 3, 2, 'o');
    }else if (lvl == 2) {
        // Определяем количество объектов
        brickLength = 4;
        // Выделяем или перераспределяем память для объектов
        brick = realloc(brick, sizeof(*brick) * brickLength);

        // Инициализация всех объектов уровня
        InitMario(brick + 0, 20, 20, 40, 5, '#');
        InitMario(brick + 1, 80, 20, 15, 5, '#');
        InitMario(brick + 2, 120, 15, 15, 10, '#');
        InitMario(brick + 3, 160, 10, 15, 15, '+');
        }
}