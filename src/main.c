#include "game.c"
int main() {
    CreateLevel(level);
    int offsetX = 0;
    int viewportWidth = 80;
    system(" color 1F");
    do {
        ClearMap();
        if ((mario.IsFly == FALSE) && (GetKeyState(VK_SPACE) < 0)) mario.vertSpeed = -1;
        if (GetKeyState('A') < 0) HorizonMoveMap(1);
        if (GetKeyState('D') < 0) HorizonMoveMap(-1);

        if (mario.y > mapHeight || mario.x < 0 || mario.x > mapWidth) {
            CreateLevel(level);
            continue;
        }
        VertMoveMario(&mario);
        for (int i = 0; i < brickLength; i++)
            PutMarioOnMap(brick[i]);
        for (int i = 0; i < movingLength; i++) {
            VertMoveMario(moving + i);
            HorizonMoveMario(moving + i);
            PutMarioOnMap(moving[i]);
        }
        PutMarioOnMap(mario);
        offsetX = (int)mario.x - viewportWidth / 2;
        if (offsetX < 0) offsetX = 0;
        if (offsetX > mapWidth - viewportWidth) offsetX = mapWidth - viewportWidth;

        setCur(0, 0);
        ShowMap(offsetX);
        Sleep(10);
    } while(GetKeyState(VK_ESCAPE) >= 0);
}
