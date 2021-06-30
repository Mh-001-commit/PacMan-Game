#include "pacman.h"
#include "ghost.h"
#include "pillets.h"

PacMan::PacMan(int initialRow, int initialColumn, int d[17][23], QGraphicsTextItem* display, Ghost* ghost_arr[3])
{
    // Reading the board data for the sake of considering boundaries
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 23; j++)
            data[i][j] = d[i][j];
    // Connecting the pacman with all the ghosts to control them at collision using these pointers
    for (int i = 0; i < 3; i++)
    {
        ghost_array[i] = ghost_arr[i];
    }
    // Setting the graphics of the pacman
    QPixmap pacmanImage("PacMan.png");
    pacmanImage = pacmanImage.scaledToWidth(40);
    pacmanImage = pacmanImage.scaledToHeight(40);
    setPixmap(pacmanImage);
    row = initialRow;
    column = initialColumn;
    row_reset = initialRow;
    col_reset =initialColumn;
    setPos(40 * column, 40 * row);
    // Initializing the default motion state at rest
    invincible = false;
    U_direction = false;
    D_direction = false;
    R_direction = false;
    L_direction = false;
    // Adjusting the timer to continue the movement of the pacman
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(250);
    // Connecting the label and the display in the scene to update the content using the pointer label
    label = display;
}

void PacMan::setRow(int newRow)
{
    row = newRow;
}

void PacMan::setColumn(int newColumn)
{
    column = newColumn;
}

int PacMan::getRow()
{
    return row;
}

int PacMan::getColumn()
{
    return column;
}

void PacMan::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    // Incase the player hits the space bar when ready
    if (key == Qt::Key_Space && !is_moving())
        R_direction = true;
    // The regular cases using the arrows to control the pacman where one direction will be true to be used in the move function
    if (key == Qt::Key_Up)
    {
        U_direction = true;
        D_direction = false;
        R_direction = false;
        L_direction = false;
    }
    else if (key == Qt::Key_Down)
    {
        U_direction = false;
        D_direction = true;
        R_direction = false;
        L_direction = false;
    }
    else if (key == Qt::Key_Right)
    {
        U_direction = false;
        D_direction = false;
        R_direction = true;
        L_direction = false;
    }
    else if (key == Qt::Key_Left)
    {
        U_direction = false;
        D_direction = false;
        R_direction = false;
        L_direction = true;
    }
}

void PacMan::move()
{
    // The regular cases where each direction checks the precense of a boundary and advances
    if (U_direction && data[row - 2][column - 1] != -1)
    {
        row--;
        dir = Qt::Key_Up;
    }
    else if (D_direction && data[row][column - 1] != -1)
    {
        row++;
        dir = Qt::Key_Down;
    }
    else if (R_direction && data[row - 1][column] != -1)
    {
        column++;
        if (column == 23 && dir == Qt::Key_Right)
        {
            setPos(40 *column, 40 * row);
            QList<QGraphicsItem*> items2 = collidingItems();
            for (int i = 0; i < items2.size(); i++)
            {
                // Pillets case increase the score, remove the pillet and increase the number eaten to track the win case
                if (typeid(*items2[i]) == typeid(Pillets))
                {
                    scene()->removeItem(items2[i]);
                    score++;
                    pillet_count++;
                }
            }
            column = 1;
        }
        dir = Qt::Key_Right;
    }
    else if (L_direction && data[row - 1][column - 2] != -1)
    {
        column--;
        if (column == 1 && dir == Qt::Key_Left)
        {
            setPos(40 *column, 40 * row);
            QList<QGraphicsItem*> items3 = collidingItems();
            for (int i = 0; i < items3.size(); i++)
            {
                // Pillets case increase the score, remove the pillet and increase the number eaten to track the win case
                if (typeid(*items3[i]) == typeid(Pillets))
                {
                    scene()->removeItem(items3[i]);
                    score++;
                    pillet_count++;
                }
            }
            column = 23;
        }
        dir = Qt::Key_Left;
    }
    // The player hits a direction in the precense of a boundary so we ensure the pacman will go on and change direction when possible
    if (!U_direction && dir == Qt::Key_Up)
    {
        if (data[row - 2][column - 1] != -1)
            row--;
    }
    if (!D_direction && dir == Qt::Key_Down)
    {
        if (data[row][column - 1] != -1)
            row++;
    }
    if (!R_direction && dir == Qt::Key_Right)
    {
        if (data[row - 1][column] != -1)
            column++;
    }
    if (!L_direction && dir == Qt::Key_Left)
    {
        if (data[row - 1][column - 2] != -1)
            column--;
    }
    setPos(40 *column, 40 * row);
    // Detecting collision with the different objects
    QList<QGraphicsItem*> items = collidingItems();
    for (int i = 0; i < items.size(); i++)
    {
        // Pillets case increase the score, remove the pillet and increase the number eaten to track the win case
        if (typeid(*items[i]) == typeid(Pillets))
        {
            scene()->removeItem(items[i]);
            score++;
            pillet_count++;
        }
        // SuperPillets case make pacman invinsible but for only 7 seconds using the single shot function, increase the score and the number eaten
        else if (typeid(*items[i]) == typeid(SuperPillet))
        {
            scene()->removeItem(items[i]);
            score +=10;
            pillet_count++;
            invincible = true;
            QTimer::singleShot(7000, this, SLOT(PacMan_Normal()));
        }
        else if (typeid(*items[i]) == typeid(Ghost))
        {
            // If the pacman is invinsible it won't reset but the ghosts will
            if (invincible)
            {
                if (items[i] == ghost_array[0])
                    ghost_array[0]->ghost_coll();
                else if (items[i] == ghost_array[1])
                    ghost_array[1]->ghost_coll();
                else
                    ghost_array[2]->ghost_coll();
                score += 10;
            }
            else
            {
                // Checking which ghost killed the pacman to reset it along with pacman
                if (items[i] == ghost_array[0])
                {
                    lives--;
                    row = 15;
                    column = 12;
                    setPos(40* column, 40* row);
                    ghost_array[0]->ghost_coll();
                }
                else if (items[i] == ghost_array[1])
                {
                    lives--;
                    row = 15;
                    column = 12;
                    setPos(40* column, 40* row);
                    ghost_array[1]->ghost_coll();
                }
                else if (items[i] == ghost_array[2])
                {
                    lives--;
                    row = 15;
                    column = 12;
                    setPos(40* column, 40* row);
                    ghost_array[2]->ghost_coll();
                }
            }
        }
    }
    // Changing the label depending on whether it's ingame or before the game starts
    if (is_moving())
    {
        // During game
        label->setPos(50, 725);
        label->setPlainText("Score: " + QString::number(score) + "    Lives: " + QString::number(lives));
        if (is_invinsible())
            label->setPlainText(label->toPlainText() + "    Invincible mode activated.");
        // Lose case the timer and the game will stop
        if (lives == 0)
        {
            row = row_reset;
            column = col_reset;
            setPos(40* column, 40* row);
            label->setPlainText("Score: " + QString::number(score) + "    Lives: " + QString::number(lives) + "   Game Over.");
            timer->~QTimer();
        }
        // Win case all pillets are eaten and the game will stop
        if (pillet_count == 109)
        {
            row = row_reset;
            column = col_reset;
            setPos(40* column, 40* row);
            label->setPlainText("Score: " + QString::number(score) + "    Lives: " + QString::number(lives) + "   Congratulations, you won.");
            timer->~QTimer();
        }
    }
    // Before the game starts
    else
    {
        label->setPos(465, 480);
        label->setPlainText("Ready!");
    }
}

int PacMan::getScore()
{
    return score;
}

//The collision function to ensure the ghosts can control the pacman without violations
void PacMan::PacMan_coll()
{
    lives--;
    row = row_reset;
    column = col_reset;
    setPos(40* column, 40* row);
}

// The function to tell the ghosts whether to reset pacman or not
bool PacMan::is_invinsible()
{
    return invincible;
}

// To show the ghosts that the game has ended and pacman won
int PacMan::getpilletcount()
{
    return pillet_count;
}

// To show the ghosts that the game started and they can move
bool PacMan::is_moving()
{
    if (U_direction || D_direction || R_direction || L_direction)
        return true;
    else return false;
}

// The slot that stop pacman from being invincible after sometime
void PacMan::PacMan_Normal()
{
    invincible =false;
}

bool PacMan::is_dead()
{
  if (lives <= 0)
      return true;
  else return false;
}
