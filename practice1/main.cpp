#include "practice1.h"

#define DRAW        1

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
    default:
    {
        break;
    }
    }
    
    return 0;
}
