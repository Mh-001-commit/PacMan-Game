#include "ghost.h"
#include <QDebug>

int Ghost::ghost_num = 0;
int Ghost::kills = 0;

Ghost::Ghost(int initialRow, int initialColumn, int d[][23])
{
    // Reading the data of the board to respond to the boundries correctly
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 23; j++)
            data[i][j] = d[i][j];
    for (int x = 0; x < 126; x++)
        for (int y = 0; y < 126; y++)
            graph[x][y] = inf;
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 23; j++)
        {
            if (data[i][j] != -1)
            {
                if (i + 1 <= 16)
                {
                    if (data[i + 1][j] != -1)
                        graph[data[i][j] - 1][data[i + 1][j] - 1] = 1;
                }
                if (i - 1 >= 0)
                {
                    if (data[i - 1][j] != -1)
                        graph[data[i][j] - 1][data[i - 1][j] - 1] = 1;
                }
                if (j + 1 <= 22)
                {
                    if (data[i][j + 1] != -1)
                        graph[data[i][j] - 1][data[i][j + 1] - 1] = 1;
                }
                if (j - 1 >= 0)
                {
                    if (data[i][j - 1] != -1)
                        graph[data[i][j] - 1][data[i][j - 1] - 1] = 1;
                }
                graph[data[i][j] - 1][data[i][j] - 1] = inf;
                graph[43][57] = 1;
                graph[57][43] = 1;
            }
        }
    // Initiallizing different pixmaps for inky, pinky and clyde that cycles based on the number of ghosts created
    QPixmap ghost1Image("Ghost1.png");
    ghost1Image = ghost1Image.scaledToWidth(40);
    ghost1Image = ghost1Image.scaledToHeight(40);
    QPixmap ghost2Image("Ghost2.png");
    ghost2Image = ghost2Image.scaledToWidth(40);
    ghost2Image = ghost2Image.scaledToHeight(40);
    QPixmap ghost3Image("Ghost3.png");
    ghost3Image = ghost3Image.scaledToWidth(40);
    ghost3Image = ghost3Image.scaledToHeight(40);
    vulnerable.load("Vulnerable.png");
    vulnerable = vulnerable.scaledToWidth(40);
    vulnerable = vulnerable.scaledToHeight(40);
    // Setting a different pixmap for each ghost created
    if (ghost_num%3 == 0)
    {
        normal_state = ghost3Image;
        setPixmap(normal_state);
    }
    else if (ghost_num%3 == 1)
    {
        normal_state = ghost1Image;
        setPixmap(normal_state);
    }
    else if (ghost_num%3 == 2)
    {
        normal_state = ghost2Image;
        setPixmap(normal_state);
    }
    // Initializing the coordinates of the ghosts
    row = initialRow;
    column = initialColumn;
    row_reset = initialRow;
    column_reset = initialColumn;
    setPos(40 * column, 40 * row);
    // Increasing the ghost counter to cycle the images
    ghost_num++;
    // resizing the paths vector to recieve the return value of the dijstra function
    paths.resize(126);
    // Adjusting the timer to contineously move the ghost
    timer_ghost = new QTimer(this);
    QObject::connect(timer_ghost, SIGNAL(timeout()), this, SLOT(move()));
    timer_ghost->start(400);
}

void Ghost:: move()
{
    // Ensuring the ghosts never move unless the game has started and the pacman has moved
    if (PM_address->is_moving())
        {
            location = data[row - 1][column - 1] - 1;
            PM_location = data[PM_address->getRow() - 1][PM_address->getColumn() - 1] - 1;
            // Checking if invincible mode is on or not
            if (PM_address->is_invinsible())
                {
                    setPixmap(vulnerable);
                    paths = dijkstra(graph, data[row_reset - 1][column_reset - 1] -1);
                }
            else
            {
                setPixmap(normal_state);
                paths = dijkstra(graph, PM_location);
            }
            // Checking the change in coordinates that conforms with the first node to move to
            if (data[row - 2][column - 1] == (paths[location][1] + 1))
                {
                    row--;
                }
            else if (data[row][column - 1] == (paths[location][1] + 1))

                {
                    row++;
                }
            else if (data[row - 1][column] == (paths[location][1] + 1))
                {
                    column++;
                    // Handling the teleportation of the ghosts
                    if (data[row - 1][column] == 58)
                        column = 1;
                }
            else if (data[row - 1][column - 2] == (paths[location][1] + 1))
                {
                    column--;
                    // Handling the teleportation of the ghosts
                    if (data[row - 1][column - 2] == 44)
                        column = 23;
                }
            setPos(40 *column, 40 * row);
        }
    // Checking for collision with the pacman
    QList<QGraphicsItem*> items = collidingItems();
    for (int i = 0; i < items.size(); i++)
    {
        if (typeid(*items[i]) == typeid(PacMan))
        {
            // If the pacman is not invinsible it will be reset losing a life, otherwise only the ghost will reset
            if (!PM_address->is_invinsible())
            {
                ghost_coll();
                PM_address->PacMan_coll();
                kills++;
            }
            else ghost_coll();
        }
    }
    // In the lost case and the win case the timer of the ghosts will stop in order to the movement and the game
    if ((kills==3) || (PM_address->getpilletcount() == 109) || PM_address->is_dead())
    {
        timer_ghost->stop();
    }
}

// Implementing a collision function for the ghost to execute from the pacman class using a pointer to the object of type Ghost
void Ghost::ghost_coll()
{
    row = row_reset;
    column = column_reset;
    setPos(40 *column, 40 * row);
}

/* Connecting the pacman and the ghosts since both needs connection but one is created before the other so it can't be a matter
of passing through the parameterized constructor as in the case of the pacman class */
void Ghost::connect(PacMan* PM_connect)
{
    PM_address = PM_connect;
}

std::vector<std::vector<int>> Ghost::dijkstra(int graph[126][126], int destination)
{
    int temp[126][126], costs[126], previous[126];
        for (int i = 0; i < 126; i++)
            for (int j = 0; j < 126; j++)
            {
                temp[i][j] = graph[i][j];
            }
        bool visited[126];
        // costs, previous, and visited
        for (int i = 0; i < 126; i++)
        {
            costs[i] = temp[destination][i];
            previous[i] = destination;
            visited[i] = false;
        }
        // startNode
        costs[destination] = 0;
        visited[destination] = true;

        int count = 1, nextNode, minimumCost;
        while (count < 126)
        {
            // Find the minimum cost in order to visit a node.
            minimumCost = inf;
            for (int i = 0; i < 126; i++)
                if ((costs[i] < minimumCost) && (visited[i] == false))
                {
                    minimumCost = costs[i];
                    nextNode = i;
                }
            // Visit the node.
            visited[nextNode] = true;
            // Update the costs of the children of the visited node.
            for (int i = 0; i < 126; i++)
                if ((minimumCost + temp[nextNode][i] < costs[i]) && (visited[i] == false))
                {
                    costs[i] = minimumCost + temp[nextNode][i];
                    previous[i] = nextNode;
                }
            count++;
        }
        // Fill the paths.
        int j;
        std::vector<std::vector<int>> paths;
        paths.resize(126);
        for (int i = 0; i < 126; i++)
        {
            paths[i].push_back(std::move(i));
            if (i != destination)
            {
                j = i;
                do
                {
                    j = previous[j];
                    paths[i].push_back(std::move(j));
                } while (j != destination);
            }
        }
        return paths;
}
