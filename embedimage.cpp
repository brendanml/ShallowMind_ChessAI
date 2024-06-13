#include "raylib.h"

int main ()
{
	Image img = LoadImage("assets/pawns.png");
	ExportImageAsCode(img, "pawns.h");
	UnloadImage(img);

    img = LoadImage("assets/pieces.png");
    ExportImageAsCode(img, "pieces.h");
    UnloadImage(img);
}