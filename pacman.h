#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QKeyEvent>
#include <QList>
#include <QGraphicsScene>
#include <QTimer>
#include <QThread>
#include <QGraphicsTextItem>
#include <QApplication>
#include "pillets.h"
#include "superpillet.h"
#include "ghost.h"

// Forward declaring the ghost class to use it in the pacman class
class Ghost;

class PacMan : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    int row;
    int column;
    int score = 0;
    // Data for win and lose cases
    int lives = 3;
    int pillet_count = 0;
    // Storing the imported board data
    int data[17][23];
    // Directions to be used in the move function as indicators and conditions
    bool U_direction,D_direction,R_direction,L_direction;
    // Showing whether pacman is invincible or not
    bool invincible;
    // Array of pointers to connect the pacman to the ghosts
    Ghost* ghost_array[3];
    // The timer for movement
    QTimer* timer;
    // Direction stored before changing for the special case when the player hits an unallowed direction
    Qt::Key dir;
    // The label to show the data of the game
    QGraphicsTextItem* label;
    int row_reset, col_reset;
public:
    PacMan(int initialRow, int initialColumn, int d[17][23], QGraphicsTextItem* display, Ghost* ghost_arr[3]);
    // Setters
    void setRow(int newRow);
    void setColumn(int newColumn);
    // The collision function for the ghosts controlling pacman at collision
    void PacMan_coll();
    // Getters
    int getRow();
    int getColumn();
    int getpilletcount();
    int getScore();
    // Functions to show ghosts the state of pacman without violation
    bool is_invinsible();
    bool is_moving();
    bool is_dead();
    // Different slots for timers either one shot or contineously
private slots:
    void keyPressEvent(QKeyEvent *event);
    void move();
    void PacMan_Normal();
};

#endif //PACMAN_H
