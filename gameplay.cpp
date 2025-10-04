#include "gameplay.h"

bool CanPlaceShip(int map[ROWS][COLS], int x, int y, int size, bool horizontal) {
    int startX, startY, endX, endY;

    if (horizontal) {
        if (x + size > COLS) return false;
        startX = (x > 0) ? x - 1 : x;
        startY = (y > 0) ? y - 1 : y;
        endX = (x + size < COLS) ? x + size : COLS - 1;
        endY = (y < ROWS - 1) ? y + 1 : ROWS - 1;
    }
    else {
        if (y + size > ROWS) return false;
        startX = (x > 0) ? x - 1 : x;
        startY = (y > 0) ? y - 1 : y;
        endX = (x < COLS - 1) ? x + 1 : COLS - 1;
        endY = (y + size < ROWS) ? y + size : ROWS - 1;
    }

    for (int r = startY; r <= endY; ++r) {
        for (int c = startX; c <= endX; ++c) {
            if (map[r][c] == S) {
                return false;
            }
        }
    }
    return true;
}

void PlaceShip(int map[ROWS][COLS], int x, int y, int size, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < size; i++) map[y][x + i] = S;
    }
    else {
        for (int i = 0; i < size; i++) map[y + i][x] = S;
    }
}

void DeleteShip(int map[ROWS][COLS], int x, int y, int* shipsAvailable) {
    if (map[y][x] != S) return;
    bool horizontal = false;
    if (x + 1 < COLS && map[y][x + 1] == S) horizontal = true;
    else if (x - 1 >= 0 && map[y][x - 1] == S) horizontal = true;
    int startX = x, startY = y;
    if (horizontal) {
        while (startX > 0 && map[y][startX - 1] == S) startX--;
    }
    else {
        while (startY > 0 && map[startY - 1][x] == S) startY--;
    }
    int length = 0;
    if (horizontal) {
        int cx = startX;
        while (cx < COLS && map[y][cx] == S) {
            map[y][cx] = E;
            length++;
            cx++;
        }
    }
    else {
        int cy = startY;
        while (cy < ROWS && map[cy][x] == S) {
            map[cy][x] = E;
            length++;
            cy++;
        }
    }
    if (length >= 1 && length <= 4) {
        shipsAvailable[length - 1]++;
    }
}

void PlaceShips(int map[ROWS][COLS]) {
    int shipsAvailable[SHIPS_VARIETIES] = { 4, 3, 2, 1 };
    int cursorX = 0, cursorY = 0;
    int shipSize = 4;
    while (shipSize >= 1 && shipsAvailable[shipSize - 1] == 0) {
        shipSize--;
    }

    bool horizontal = true;
    while (shipsAvailable[0] + shipsAvailable[1] + shipsAvailable[2] + shipsAvailable[3] > 0) {
        system("cls");
        ShowMap(0, 0, map, 0, 0, false, false);
        ShowShipsList(shipsAvailable, 25, 0);
        SetCursorPosition(25, 6);
        SetColor(WHITE, BLACK);
        cout << "Current ship: " << shipSize << "-cell" << (horizontal ? " (H)" : " (V)");
        SetCursorPosition(25, 7);
        cout << "Press numbers from 1 to 4 to choose size of the ship";
        SetCursorPosition(25, 8);
        cout << "Press ENTER to place your ship";
        SetCursorPosition(25, 9);
        cout << "Press 'R' to rotate your ship";
        SetCursorPosition(25, 10);
        cout << "Press 'D' to delete ship";
        SetCursorPosition(cursorX * 2, cursorY);
        SetColor(WHITE, BLACK);
        cout << "[]";

        int key = _getch();
        if (key == ARROW_PREFIX) {
            key = _getch();
            if (key == UP && cursorY > 0) cursorY--;
            else if (key == DOWN && cursorY < ROWS - 1) cursorY++;
            else if (key == LEFT && cursorX > 0) cursorX--;
            else if (key == RIGHT && cursorX < COLS - 1) cursorX++;
        }
        else if (key == r_key || key == R_key) {
            horizontal = !horizontal;
        }

        else if ((key == D_key || key == d_key) && map[cursorY][cursorX] == S) {
            DeleteShip(map, cursorX, cursorY, shipsAvailable);
        }

        else if (key == ENTER) {
            if (shipSize >= 1 && shipsAvailable[shipSize - 1] > 0 &&
                CanPlaceShip(map, cursorX, cursorY, shipSize, horizontal)) {
                PlaceShip(map, cursorX, cursorY, shipSize, horizontal);
                shipsAvailable[shipSize - 1]--;
                if (shipsAvailable[shipSize - 1] == 0) {
                    while (shipSize > 1) {
                        shipSize--;
                        if (shipsAvailable[shipSize - 1] > 0) {
                            break;
                        }
                    }
                }
            }
        }
        else if (key >= '1' && key <= '4') {
            int newSize = key - '0';
            if (shipsAvailable[newSize - 1] > 0) {
                shipSize = newSize;
            }
        }
    }
}

void RandomMap(int compMap[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            compMap[i][j] = E;
        }
    }
    int shipsAvailable[SHIPS_VARIETIES] = { 4, 3, 2, 1 };
    while (shipsAvailable[0] + shipsAvailable[1] + shipsAvailable[2] + shipsAvailable[3] > 0) {
        int shipSize = 1 + rand() % 4;
        if (shipsAvailable[shipSize - 1] > 0) {
            int x = rand() % COLS;
            int y = rand() % ROWS;
            bool horizontal = rand() % 2;
            if (CanPlaceShip(compMap, x, y, shipSize, horizontal)) {
                PlaceShip(compMap, x, y, shipSize, horizontal);
                shipsAvailable[shipSize - 1]--;
            }
        }
    }
}

bool IsShipSunk(int map[ROWS][COLS], int row, int col, bool visited[ROWS][COLS]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visited[row][col])
        return true;
    if (map[row][col] == S) {
        return false;
    }
    if (map[row][col] != H) {
        return true;
    }
    visited[row][col] = true;
    bool sunk = true;
    sunk &= IsShipSunk(map, row - 1, col, visited);
    sunk &= IsShipSunk(map, row + 1, col, visited);
    sunk &= IsShipSunk(map, row, col - 1, visited);
    sunk &= IsShipSunk(map, row, col + 1, visited);
    return sunk;
}

void MarkPerimeterAsMisses(int map[ROWS][COLS], int row, int col) {
    int dR[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dC[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int i = 0; i < 8; ++i) {
        int nR = row + dR[i];
        int nC = col + dC[i];

        if (nR >= 0 && nR < ROWS && nC >= 0 && nC < COLS) {
            if (map[nR][nC] == E) {
                map[nR][nC] = M;
            }
        }
    }
}

void SinkShip(int map[ROWS][COLS], int row, int col, bool visited[ROWS][COLS]) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visited[row][col] || map[row][col] != H)
        return;
    MarkPerimeterAsMisses(map, row, col);
    visited[row][col] = true;
    SinkShip(map, row - 1, col, visited);
    SinkShip(map, row + 1, col, visited);
    SinkShip(map, row, col - 1, visited);
    SinkShip(map, row, col + 1, visited);
}

void PlayersTurn(int key, int& cursorX, int& cursorY,
    int computerMap[ROWS][COLS],
    int computerVisibleMap[ROWS][COLS],
    bool& playerTurn, int& shipsCount)
{
    if (key == ARROW_PREFIX) {
        key = _getch();
        if (key == UP && cursorY > 0) cursorY--;
        else if (key == DOWN && cursorY < ROWS - 1) cursorY++;
        else if (key == LEFT && cursorX > 0) cursorX--;
        else if (key == RIGHT && cursorX < COLS - 1) cursorX++;
    }
    else if (key == ENTER) {
        int y = cursorY;
        int x = cursorX;
        if (computerVisibleMap[y][x] == E) {
            if (computerMap[y][x] == S) {
                computerMap[y][x] = H;
                computerVisibleMap[y][x] = H;
                bool visited[ROWS][COLS] = { false };
                if (IsShipSunk(computerMap, y, x, visited)) {
                    shipsCount--;
                    bool visited_sink[ROWS][COLS] = { false };
                    SinkShip(computerVisibleMap, y, x, visited_sink);

                    playerTurn = true;
                }
                else {
                    playerTurn = true;
                }
            }
            else {
                computerVisibleMap[y][x] = M;
                playerTurn = false;
            }
            SetCursorPosition(cursorX, cursorY);
            SetCursorPosition(x, y);
            cout << "  ";
        }
        Sleep(500);
    }
}

int GetRandomNum(int min, int max) {
    int num = min + rand() % (max - min + 1);
    return num;
}

void ComputersTurn(const int difficulty,
    int playerMap[ROWS][COLS],
    bool& playersTurn,
    int& shipsCount)
{
    int x = 0, y = 0;
    do {
        x = GetRandomNum(0, COLS - 1);
        y = GetRandomNum(0, ROWS - 1);
    } while (playerMap[y][x] == H || playerMap[y][x] == M);

    switch (difficulty)
    {
    case CASUAL:
    {
        if (playerMap[y][x] == S) 
        {
            playerMap[y][x] = H;
            bool visited[ROWS][COLS] = { false };
            if (IsShipSunk(playerMap, y, x, visited)) {
                shipsCount--;
                bool visited_sink[ROWS][COLS] = { false };
                SinkShip(playerMap, y, x, visited_sink);
            }
            playersTurn = playersTurn;
        }
        else if (playerMap[y][x] == E)
        {
            playerMap[y][x] = M;
            playersTurn = !playersTurn;
        }
        break;
    }

    case HARD:
    {
        bool foundTarget = false;
        for (int i = 0; i < ROWS && !foundTarget; i++) {
            for (int j = 0; j < COLS && !foundTarget; j++) {
                if (playerMap[i][j] == H) {
                    int dx[4] = { 1, -1, 0, 0 };
                    int dy[4] = { 0, 0, 1, -1 };

                    for (int d = 0; d < 4; d++) {
                        int ny = i + dy[d];
                        int nx = j + dx[d];
                        if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
                            if (playerMap[ny][nx] == E || playerMap[ny][nx] == S) {
                                x = nx;
                                y = ny;
                                foundTarget = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if (!foundTarget) {
            do {
                x = GetRandomNum(0, COLS - 1);
                y = GetRandomNum(0, ROWS - 1);
            } while (playerMap[y][x] == H || playerMap[y][x] == M);
        }
        if (playerMap[y][x] == S) 
        {
            playerMap[y][x] = H;
            bool visited[ROWS][COLS] = { false };
            if (IsShipSunk(playerMap, y, x, visited)) {
                shipsCount--;
                bool visited_sink[ROWS][COLS] = { false };
                SinkShip(playerMap, y, x, visited_sink);
            }
            playersTurn = playersTurn;
        }
        else if (playerMap[y][x] == E) 
        {
            playerMap[y][x] = M;
            playersTurn = !playersTurn;
        }
        break;
    }
    }

    Sleep(2000);
}
