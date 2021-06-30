#include "pillets.h"

Pillets::Pillets(int initialRow, int initialColumn)
{
        QPixmap pilletImage("Pillet.png");
        pilletImage = pilletImage.scaledToWidth(40);
        pilletImage = pilletImage.scaledToHeight(40);
        setPixmap(pilletImage);
        setPos(40 + 40 * initialColumn, 40 + 40 * initialRow);
}
