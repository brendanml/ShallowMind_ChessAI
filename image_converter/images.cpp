#include <raylib.h>

int main ()
{
	Image img = LoadImage("../Resources/pawns.png");
	ExportImageAsCode(img, "../Resources/pawns.h");
	UnloadImage(img);

    img = LoadImage("../Resources/pieces.png");
    ExportImageAsCode(img, "../Resources/pieces.h");
    UnloadImage(img);
}