#include "practice1.h"

#define DRAW        3

int main(int argc, char* argv[])
{
    switch (DRAW)
    {
    case 0:
    {
        drawWindow();                           // ������
        break;
    }
    case 1:
    {
        drawTriangle();                         // ��������
        break;
    }
    case 2:
    {
        drawDynamicTriangle();
        break;
    }
    case 3:
    {
        drawTexture();
        break;
    }
    default:
    {
        break;
    }
    }
    
    return 0;
}
