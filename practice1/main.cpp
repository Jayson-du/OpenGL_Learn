#include "practice1.h"

#define DRAW        5

int main(int argc, char* argv[])
{
    switch (DRAW)
    {
    case 0:
    {
        drawWindow();                           // »­´°Ìå
        break;
    }
    case 1:
    {
        drawTriangle();                         // »­Èý½ÇÐÎ
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
    case 4:
    {
        drawCoordinate();
        break;
    }
    case 5:
    {
        drawCamera();
        break;
    }
    default:
    {
        break;
    }
    }
    
    return 0;
}


