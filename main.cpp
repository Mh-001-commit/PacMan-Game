#include "game.h"
#include "ghost.h"
#include "pacman.h"
#include "pillets.h"
#include "superpillet.h"
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QTextStream>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsTextItem>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Create the view and the scene
    QGraphicsView view;
    view.setFixedSize(1100, 900);
    view.setWindowTitle("   PacMan Game");
    view.setBackgroundBrush(QBrush(Qt::black));
    view.setWindowIcon(QIcon("Icon.ico"));
    QGraphicsScene scene;
    view.setScene(&scene);
    // Read board data from text file
    QFile file("PacMan.txt");
    file.open(QIODevice::ReadOnly);
    int boardData[17][23];
    QTextStream stream(&file);
    QString temp;
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 23; j++)
        {
            stream >> temp; // cin >> temp;
            boardData[i][j] = temp.toInt();
        }
    // Draw the graphics of the board
    QPixmap bricksImage("Bricks.png");
    bricksImage = bricksImage.scaledToWidth(40);
    bricksImage = bricksImage.scaledToHeight(40);
    QPixmap voidImage("Void.png");
    voidImage = voidImage.scaledToWidth(40);
    voidImage = voidImage.scaledToHeight(40);
    QGraphicsPixmapItem boardImages[19][23];
    // Creating Pillets & SuperPillets & constructing the board
    Pillets** pillets = new Pillets*[105];
    SuperPillet** superpillets = new SuperPillet*[4];
    int pillets_count = 0;
    int superpillets_count = 0;
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 23; j++)
        {
            if (boardData[i][j] == -1)
            {
                boardImages[i][j].setPixmap(bricksImage);
                boardImages[i][j].setPos(40 + 40 * j, 40 + 40 * i);
                scene.addItem(&boardImages[i][j]);
            }
            else
            {
                // The cases where there are no ghosts, pacman, ghost house or superpillets
                if (boardData[i][j] != 62 &&boardData[i][j] !=  63 &&boardData[i][j] !=  64 &&boardData[i][j] !=  117 &&boardData[i][j] !=  41 &&boardData[i][j] !=  49 &&boardData[i][j] !=  50 &&boardData[i][j] !=  51 &&boardData[i][j] !=  52 &&boardData[i][j] !=  53 &&boardData[i][j] !=  61 &&boardData[i][j] !=  62 &&boardData[i][j] !=  63 &&boardData[i][j] !=  64 &&boardData[i][j] !=  65 &&boardData[i][j] !=  70 &&boardData[i][j] !=  71 &&boardData[i][j] !=  72 &&boardData[i][j] !=  73 &&boardData[i][j] !=  74 &&boardData[i][j] != 01 &&boardData[i][j] != 11 &&boardData[i][j] != 77 &&boardData[i][j] != 88)
                {
                    pillets[pillets_count] = new Pillets(i, j);
                    scene.addItem(pillets[pillets_count]);
                    pillets_count++;
                }
                // The determined locations for the superpillets
                else if (boardData[i][j] == 01 || boardData[i][j] == 11 || boardData[i][j] == 77 || boardData[i][j] == 88)
                {
                    superpillets[superpillets_count] = new SuperPillet(i,j);
                    scene.addItem(superpillets[superpillets_count]);
                    superpillets_count++;
                }
            }
        }
    // Creating the ghosts
    Ghost G1(9, 12, boardData);
    scene.addItem(&G1);
    Ghost G2(8, 12, boardData);
    scene.addItem(&G2);
    Ghost G3(10, 12, boardData);
    scene.addItem(&G3);
    // Making an array of pointers of ghosts to link to the pacman for the sake of the collision execution
    Ghost* ghost_arr[3];
    ghost_arr[0] = &G1;
    ghost_arr[1] = &G2;
    ghost_arr[2] = &G3;
    // Create the player
    QGraphicsTextItem info;
    scene.addItem(&info);
    QFont info_font;
    info_font.setPixelSize(25);
    info.setFont(info_font);
    info.setDefaultTextColor(Qt::white);
    PacMan PM(15, 12, boardData, &info, ghost_arr);
    scene.addItem(&PM);
    PM.setFlag(QGraphicsPixmapItem::ItemIsFocusable);
    PM.setFocus();
    // Connecting the ghosts to the pacman to ensure the execution of the collision state regardless of the moving object
    G1.connect(&PM);
    G2.connect(&PM);
    G3.connect(&PM);
    view.show();
    return a.exec();
}
