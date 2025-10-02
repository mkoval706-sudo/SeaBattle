#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "windows.h"
#include "conio.h"
using namespace std;

#define ARROW_PREFIX 224
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define ENTER 13
#define SPACE 32
#define R_key 82
#define r_key 114
#define D_key 68
#define d_key 100
#define MAP_X 30
#define MAP_Y 2

const int defaultColor = 11; //CYAN 
const int ROWS = 10, COLS = 10;
const int MAX_SHIPS = 10;
const int MAIN_MENU_ITEMS = 4;
const int GAME_MODE_ITEMS = 3;
const int FINAL_MENU_ITEMS = 3;
const int ESC_MENU_ITEMS = 3;
const int GAME_DIFFICULTY = 2;
const int SHIPS_VARIETIES = 4;
const int PLACEMENT_OPTIONS = 2;
const int COLOR_OPTIONS = 8;
const int DOTS_COUNT = 3;
const int DOTS_DELAY_MS = 400;
const int CLEAR_DELAY_MS = 300;

const string fileName = "SavedFiles.txt";
const string fileWithTheme = "SavedTheme.txt";

enum Colors {
    BLACK = 0,
    DARK_BLUE = 1,
    DARK_GREEN = 2,
    DARK_CYAN = 3,
    DARK_RED = 4,
    DARK_MAGENTA = 5,
    DARK_YELLOW = 6,
    GREY = 7,
    DARK_GREY = 8,
    BLUE = 9,
    GREEN = 10,
    CYAN = 11,
    RED = 12,
    MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

enum Cell {
	E, //Пустo
	S, //Корабель
	H, //Попадання
	M, //Промах
};

enum Menu {
	NEW_GAME,
	LOAD_GAME,
	OPTIONS,
	EXIT,
    BACK = 1,
    CLOSE_GAME = 2,
    SAVE_QUIT = 1,
    RESUME = 2
};

enum GameMode {
	PLAYER_VS_COMPUTER,
	COMPUTER_VS_COMPUTER,
	BACK_TO_MAIN_MENU
};

enum Placement {
	MANUAL,
	RANDOM
};

enum Difficulty {
    CASUAL,
    HARD
};

enum Status {
    LOSER = 1,
    WINNER
};

void SetColor(int text, int background) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; 
    SetConsoleCursorInfo(out, &cursorInfo);
}

void ShowGameName(int color) { 
    SetColor(color, BLACK);
    cout << "         SEA BATTLE" << endl;
	string press = "Press enter to start the game";
    cout << press;
    while (_kbhit() != true) {
        string dot[DOTS_COUNT] = { ".", "..", "..." };
        int i = 0;
		for (i = 0; i < DOTS_COUNT; i++) {
			cout << dot[i];
			Sleep(400);
            SetCursorPosition(press.length(), 1);
			cout << "   ";
            SetCursorPosition(press.length(), 1);
		}
        Sleep(400);
    } 
    cin.ignore();
	system("cls");
} 

int ShowMenus(string* list, const int size, int color) {
    SetColor(color, BLACK);
    int choice = 0;
    while (true) {
        SetCursorPosition(0, 1);
        for (int i = 0; i < size; i++) {
            if (i == choice) {
                SetColor(BLACK, color);
                cout << list[i] << endl;
                SetColor(color, BLACK);
            }
            else {
                cout << list[i] << endl;
            }
        }
        int key = _getch();
        if (key == ARROW_PREFIX) {
            key = _getch();
            switch (key) {
            case DOWN: {
                choice++;
                if (choice == size) choice = 0;
                break;
            }
            case UP: {
                choice--;
                if (choice == -1) choice = size-1;
                break;
            }
            }
        }
        else if (key == ENTER) {
            system("cls");
            break;
        }
    }
    return choice;
}

bool SaveTheme(int color) {
    ofstream saved(fileWithTheme);
    if (!saved.is_open()) return false;
    saved << color;
    saved.close();
    return true;
}

int LoadColorFromFile(int defaultColor) {
    ifstream saved(fileWithTheme);
    if (!saved.is_open()) {
        return defaultColor;
    }
    int color = defaultColor;
    int value;
    while (saved >> value) {
        color = value;
    }
    saved.close();
    return color;
}

void Options(int& color) {
    SetColor(color, BLACK);
    cout << "Options menu" << endl;
    SetCursorPosition(0, 2);
    cout << "Change text color -->" << endl;
	SetCursorPosition(30, 2);
    cout << "<--" << endl;
    string colors[COLOR_OPTIONS] = { "WHITE  ", "YELLOW ", "MAGENTA", "RED    ", "CYAN   ", "GREEN  ", "BLUE   ", "GREY   " };
	int colorsCode[COLOR_OPTIONS] = { WHITE, YELLOW, MAGENTA, RED, CYAN, GREEN, BLUE, GREY };
    int choice = 0;
    while (true) {
		int prev = (choice - 1 + 8) % 8;
		int next = (choice + 1) % 8;
        SetCursorPosition(22, 1);
        SetColor(color, BLACK);
		cout << colors[prev] << endl;
		SetColor(BLACK, colorsCode[choice]);
        SetCursorPosition(22, 2);
		cout << colors[choice]<< endl;
		SetColor(color, BLACK);
        SetCursorPosition(22, 3);
        cout << colors[next] << endl;
        int key = _getch();
        if (key == ARROW_PREFIX) {
            key = _getch();
            switch (key) {
            case DOWN: {
                choice++;
                if (choice == 8) choice = 0;
                break;
            }
            case UP: { 
                choice--;
                if (choice == -1) choice = 7;
                break;
            }
            }
        }
        else if (key == ENTER) {
            break;
        }
    }
    switch (choice) {
    case 0: {
        color = WHITE;
        SaveTheme(WHITE);
        break;
    }
    case 1: {
        color = YELLOW;
        SaveTheme(YELLOW);
        break;
    }
    case 2: {
        color = MAGENTA;
        SaveTheme(MAGENTA);
        break;
    }
    case 3: {
        color = RED;
        SaveTheme(RED);
        break;
    }

    case 4: {
        color = CYAN;
        SaveTheme(CYAN);
        break;
    }
    case 5: {
        color = GREEN;
        SaveTheme(GREEN);
        break;
    }
    case 6: {
        color = BLUE;
        SaveTheme(BLUE);
        break;
    }
    case 7: {
        color = GREY;
        SaveTheme(GREY);
        break;
    }
    }
	system("cls");
	Sleep(300);
}
 
void ShowMap(int x, int y, int map[ROWS][COLS], int cursorX, int cursorY, bool active, bool playerTurn) {
    SetCursorPosition(x, y);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (active && cursorX == j && cursorY == i && playerTurn) {
                if (map[i][j] == E) {
                    SetColor(BLACK, BLUE);
                }
                else if (map[i][j] == S) {
                    SetColor(BLACK, GREY);
                }
                else if (map[i][j] == H) {
                    SetColor(BLACK, RED);
                }
				else if (map[i][j] == M) {
					SetColor(BLACK, WHITE);
				}
                cout << "[]"; 
                continue;
            }
            if (map[i][j] == E) { 
                SetColor(BLUE, BLUE);
                cout << "  ";
            }
            else if (map[i][j] == S) { 
                SetColor(GREY, GREY);
                cout << "  ";
            }
            else if (map[i][j] == H) {
                SetColor(RED, RED);
                cout << "  ";
            }
			else if (map[i][j] == M) {
				SetColor(WHITE, WHITE);
				cout << "  ";
			}
        }
        ++y;
        SetCursorPosition(x, y);
    }
    SetColor(WHITE, BLACK);
}

void ShowMaps(int map1[ROWS][COLS], int map2[ROWS][COLS], int color, int cursorX, int cursorY, bool computerMap, bool playerTurn) {
	system("cls");
    SetColor(color, BLACK);
	SetCursorPosition(0, 0);
    cout << "      Player 1" << endl;
    ShowMap(0, 2, map1, cursorX, cursorY, !computerMap, true);
    SetColor(color, BLACK);
    SetCursorPosition(30, 0);
    cout << "      Player 2" << endl;
    ShowMap(30, 2, map2, cursorX, cursorY, computerMap, playerTurn);
}

void ShowShipsList(int shipsAvailable[SHIPS_VARIETIES], int x, int y) {
    SetCursorPosition(x, y);
    cout << "Available ships:" << endl;
    SetCursorPosition(x, ++y);
    cout << "1-cell: " << shipsAvailable[0] << endl;
    SetCursorPosition(x, ++y);
    cout << "2-cell: " << shipsAvailable[1] << endl;
    SetCursorPosition(x, ++y);
    cout << "3-cell: " << shipsAvailable[2] << endl;
    SetCursorPosition(x, ++y);
    cout << "4-cell: " << shipsAvailable[3] << endl;

}

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

void DeleteShip(int map[ROWS][COLS], int x, int y, int shipsAvailable[SHIPS_VARIETIES]) {
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

int ShipPlacesOption(int color) {
    SetColor(color, BLACK);
	cout << "Select ship placement option:" << endl;
    string options[PLACEMENT_OPTIONS] = { "Manual", "Random" };
    int choice = 0;
    while (true) {
        SetCursorPosition(0, 1);
        for (int i = 0; i < PLACEMENT_OPTIONS; i++) {
            if (i == choice) {
                SetColor(BLACK, color);
                cout << options[i];
                SetColor(color, BLACK);
				SetCursorPosition(14, 1);
            }
            else {
                cout << options[i];
                SetCursorPosition(14, 1);
            }
        }
        int key = _getch();
        if (key == ARROW_PREFIX) {
            key = _getch();
            switch (key) {
            case LEFT: {
                choice++;
                if (choice == 2) choice = 0;
                break;
            }
            case RIGHT: {
                choice--;
                if (choice == -1) choice = 1;
                break;
            }
            }
        }
        else if (key == ENTER) {
            system("cls");
            break;
        }

    }
    return choice;
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

int SelectDifficulty(int color) {
    SetColor(color, BLACK);
    string modes[GAME_MODE_ITEMS] = { "Player vs Computer", "Computer vs Computer", "Back to main menu" };
    int choice = 0;
    while (true) {
        SetCursorPosition(0, 0);
        for (int i = 0; i < GAME_MODE_ITEMS; i++) {
            if (i == choice) {
                SetColor(BLACK, color);
                cout << modes[i] << endl;
                SetColor(color, BLACK);
            }
            else {
                cout << modes[i] << endl;
            }
        }
        int key = _getch();
        if (key == ARROW_PREFIX) {
            key = _getch();
            switch (key) {
            case DOWN: {
                choice++;
                if (choice == 3) choice = 0;
                break;
            }
            case UP: {
                choice--;
                if (choice == -1) choice = 2;
                break;
            }
            }
        }
        else if (key == 13) {
            system("cls");
            break;
        }
    }
    return choice;
}

int GetRandomNum(int min, int max) {
    int num = min + rand() % (max - min + 1);
    return num;
}

void ComputersTurn(const int difficulty,
    int playerMap[ROWS][COLS],
    bool& playersTurn,
    int& shipsCount) {
    int x = 0, y = 0;
    do {
        x = GetRandomNum(0, COLS - 1);
        y = GetRandomNum(0, ROWS - 1);
    } while (playerMap[y][x] == H || playerMap[y][x] == M);

    switch (difficulty) {
    case CASUAL: {
        if (playerMap[y][x] == S) {
            playerMap[y][x] = H;
            bool visited[ROWS][COLS] = { false };
            if (IsShipSunk(playerMap, y, x, visited)) {
                shipsCount--;
                bool visited_sink[ROWS][COLS] = { false };
                SinkShip(playerMap, y, x, visited_sink);
                playersTurn = false;
            }
            else {
                playersTurn = false; 
            }
        }
        else if (playerMap[y][x] == E) {
            playerMap[y][x] = M; 
            playersTurn = true; 
        }
        break;
    }
    case HARD: {
        bool foundTarget = false;
        if (!foundTarget) {
            do {
                x = GetRandomNum(0, COLS - 1);
                y = GetRandomNum(0, ROWS - 1);
            } while (playerMap[y][x] == H || playerMap[y][x] == M);
        }
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
        if (playerMap[y][x] == S) {
            playerMap[y][x] = H;
            bool visited[ROWS][COLS] = { false };
            if (IsShipSunk(playerMap, y, x, visited)) {
                shipsCount--;
                bool visited_sink[ROWS][COLS] = { false };
                SinkShip(playerMap, y, x, visited_sink);
            }
            playersTurn = false;
        }
        else if (playerMap[y][x] == E) {
            playerMap[y][x] = M;
            playersTurn = true;
        }
        break;
    }

    }
    Sleep(500);
}

int ShowMainMenu(int color) {
    string menu[MAIN_MENU_ITEMS] = { "New Game", "Load Game", "Options", "Exit" };
    int choice = ShowMenus(menu, MAIN_MENU_ITEMS, color);
    return choice;
}

int ShowGameModes(int color) {
    string modes[GAME_MODE_ITEMS] = { "Player vs Computer", "Computer vs Computer", "Back to main menu" };
    int choice = ShowMenus(modes, GAME_MODE_ITEMS, color);
    return choice;
}

int ShowEscMenu(int color) {
    system("cls");
    string options[ESC_MENU_ITEMS] = { "Start new game", "Save and quit", "Resume game" };
    int choice = ShowMenus(options, ESC_MENU_ITEMS, color);
    return choice;
}

int ShowDifficulties(int color) {
    string difficulties[GAME_DIFFICULTY] = { "CASUAL", "HARD"};
    int choice = ShowMenus(difficulties, GAME_DIFFICULTY, color);
    return choice;
}

int ShowFinalMenu(int loser, int color) {
    SetColor(color, BLACK);
    system("cls");
    loser == WINNER ? cout << "CONGRATULATIONS!\n" << "    YOU WIN!" : cout << "  LOSER  \n" << "YOU LOSE!";
    Sleep(2000);
    system("cls");
    string finalMenu[FINAL_MENU_ITEMS] = { "New Game","Back to title screen", "Close the game" };
    int choice = ShowMenus(finalMenu, FINAL_MENU_ITEMS, color);
    return choice;
}

bool SaveGameToFile(int matr1[ROWS][COLS], int matr2[ROWS][COLS],
    int visibleMatr[ROWS][COLS], int difficulty, int countShips1, int countShips2) {
    ofstream saved(fileName);
    if (!saved.is_open()) return false;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            saved << matr1[i][j] << " ";
        }
        saved << endl;
    }
    saved << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            saved << matr2[i][j] << " ";
        }
        saved << endl;
    }
    saved << endl;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            saved << visibleMatr[i][j] << " ";
        }
        saved << endl;
    }
    saved << difficulty<<endl;
    saved << countShips1<<endl;
    saved << countShips2;
    saved.close();
    return true;
}

bool LoadMapsFromFile(int matr1[ROWS][COLS], int matr2[ROWS][COLS],
    int visibleMatr[ROWS][COLS], int color, int& mode, int& difficulty,
    int& countShips1, int& countShips2) {
    ifstream saved(fileName);
    if (!saved.is_open()) {
        SetColor(color, BLACK);
        cout << "No saved game available." << endl;
        return false;
    }
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            saved >> matr1[i][j];
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            saved >> matr2[i][j];
    mode = PLAYER_VS_COMPUTER;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!(saved >> visibleMatr[i][j])) {
                mode = COMPUTER_VS_COMPUTER;
                break;
            }
        }
    }
    saved >> difficulty;
    saved >> countShips1;
    saved >> countShips2;
    saved.close();
    return true;
}

int main()
{   
    srand(time(0));
    ShowConsoleCursor(false);
    int theme = LoadColorFromFile(defaultColor);
    ShowGameName(theme);
    while (true) {
        int menuChoice = ShowMainMenu(theme);
        int mode;
        int playerMap[ROWS][COLS], computerMap[ROWS][COLS], computerVisibleMap[ROWS][COLS];
        int difficulty;
        int placement;
        int playerShips = 10, computerShips = 10;
        switch (menuChoice) {
        case NEW_GAME: {
            START_NEW_GAME:
            mode = ShowGameModes(theme);
            switch (mode) {
            case PLAYER_VS_COMPUTER: {
                for (int i = 0; i < ROWS;++i) {
                    for (int j = 0; j < COLS; ++j) {
                        playerMap[i][j] = E;
                        computerVisibleMap[i][j] = E;
                    }
                }
                computerMap[ROWS][COLS];
                SetColor(theme, BLACK);
                cout << "Choose game dufficulty:" << endl;
                difficulty = ShowDifficulties(theme);
                placement = ShipPlacesOption(theme);
                switch (placement) {
                case MANUAL: {
                    PlaceShips(playerMap); break;
                }

                case RANDOM: {
                    RandomMap(playerMap); break;
                }
                }
                RandomMap(computerMap);
                START_GAME:
                bool playerTurn = true;
                int cursorX = 0, cursorY = 0;
                int status = 0;
                while (true) {  
                    RESUME_GAME:
                    ShowMaps(playerMap, computerVisibleMap, theme, cursorX, cursorY, true, playerTurn);
                    SetColor(theme, BLACK);
                    cout << "\nYour ships remain - " << playerShips << "\tOponent ships remain - " << computerShips << endl;
                    cout << "ARROWS - MOVEMENT ON MAP, ENTER - SHOOT";
                    if (playerTurn) {
                        int key = _getch();
                        PlayersTurn(key, cursorX, cursorY, computerMap, computerVisibleMap, playerTurn, computerShips); 
                        if (key == ESC) { 
                            int escMenu = ShowEscMenu(theme);
                            switch (escMenu) {
                            case NEW_GAME: { goto START_NEW_GAME; break; }
                            case SAVE_QUIT:  { 
                                if (SaveGameToFile(playerMap, computerMap, computerVisibleMap, difficulty, playerShips, computerShips) && SaveTheme(theme)) {
                                    system("cls");
                                    SetColor(theme, BLACK);
                                    cout << "GAME WAS SAVED";
                                    Sleep(1000);
                                    system("cls");
                                }
                                else {
                                    system("cls");
                                    SetColor(theme, BLACK);
                                    cout << "ERROR WHILE OPENING FILE";
                                    Sleep(1000);
                                    goto RESUME_GAME;
                                    system("cls");
                                }
                                break; }
                            case RESUME: { goto RESUME_GAME; break; }
                            }
                            break; }
                    }
                    else ComputersTurn(difficulty, playerMap, playerTurn, playerShips);
                    if (playerShips == 0) {
                        status = LOSER; break;
                    }
                    else if (computerShips == 0) {
                        status = WINNER; break;
                    }
                }
                if (status != 0) {
                    int finalMenuChoice = ShowFinalMenu(status, theme);
                    switch (finalMenuChoice) {
                    case NEW_GAME: goto START_NEW_GAME;
                    case BACK: break;
                    case CLOSE_GAME: return 1;
                    }
                }

                break; }
            case COMPUTER_VS_COMPUTER: { 
				int computer1Map[ROWS][COLS];
				int computer2Map[ROWS][COLS];
                int placement = ShipPlacesOption(theme);
                switch (placement) {
                case MANUAL: {
                    PlaceShips(computer1Map); break;
                }

                case RANDOM: {
                    RandomMap(computer1Map); break;
                }
                }
                RandomMap(computer2Map);
                ShowMaps(computer1Map, computer2Map, theme, 0, 0, false, false);
                return 0;
                break; }
			case BACK_TO_MAIN_MENU: continue;
            }
            break;
        }
        case LOAD_GAME: {
            
            bool fileExist = LoadMapsFromFile(playerMap, computerMap, computerVisibleMap, 
                theme, mode, difficulty, playerShips, computerShips);
            if (fileExist) {
                if (mode == PLAYER_VS_COMPUTER) {
                    goto START_GAME;
                }
            }
            break;
        }
        case OPTIONS: { 
            Options(theme);
			ShowGameName(theme);   
            break;
        }
        case EXIT: {
            system("cls");
            return 0;
        }
        default: {
            break;
        }
        }
    }
    return 0;
}

//Створіть гру «Морський Бій».
//Головні особливості реалізації гри :
//Режими гри :
//Людина - комп'ютер.
//Комп'ютер - комп'ютер.
//Розміщення кораблів :
//Гравець розставляє вручну.
//Комп'ютер розставляє замість гравця.
//Комп'ютер може грати у двох режимах:
//Випадковий постріл.
//Інтелектуальна гра.Комп'ютер будує стратегію гри, а не стріляє випадково.
//Відображати поточний стан ігрового поля гравця та комп'ютера.
//Можливість зупинки, припинення, нового запуску гри.