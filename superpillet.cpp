#include "superpillet.h"

SuperPillet::SuperPillet(int initialRow, int initialColumn)
{
        QPixmap SuperPilletImage("SuperPillet.png");
        SuperPilletImage = SuperPilletImage.scaledToWidth(40);
        SuperPilletImage = SuperPilletImage.scaledToHeight(40);
        setPixmap(SuperPilletImage);
        setPos(40 + 40 * initialColumn, 40 + 40 * initialRow);
}
