#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QTimer>
#include <QList>
#include <QVector>
#include <vector>
#include "pacman.h"

const int inf = 99999;

// Forward declaring the pacman class to use it in the ghost class
class PacMan;

class Ghost : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // Location-related variables
    int row;
    int column;
    int row_reset, column_reset;
    // The board data container along with the adjacency matrix container
    int data[17][23];
    int graph[126][126];
    //Storing the pixmaps to change them freely
    QPixmap normal_state;
    QPixmap vulnerable;
    // Static variables to track general data for the whole class
    static int ghost_num, kills;
    // Pointer for connection between the two classes
    PacMan* PM_address;
    // The timer for the movement of the ghosts
    QTimer* timer_ghost;
    // Data members necessary for the functionality of the dijkstra algorithm
    std::vector<std::vector<int>> paths;
    int location;
    int PM_location;
public:
    Ghost(int initialRow, int initialColumn, int d[17][23]);
    // The collision function for the pacman to reset the ghosts
    void ghost_coll();
    // The function to connect each ghost with pacman
    void connect(PacMan* PM_connect);
    std::vector<std::vector<int>> dijkstra(int graph[126][126], int startNode);
private slots:
    // The slot called by the timer
    void move();
};
#endif // GHOST_H
