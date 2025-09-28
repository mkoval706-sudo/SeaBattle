#include <iostream>
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
#define R_key 82
#define r_key 114
#define MAP_X 30
#define MAP_Y 2

const int ROWS = 10, COLS = 10;
const int MAX_SHIPS = 10;
const int MAIN_MENU_ITEMS = 4;
const int GAME_MODE_ITEMS = 3;
const int GAME_DIFFICULTY = 2;
const int SHIPS_VARIETIES = 4;
const int PLACEMENT_OPTIONS = 2;
const int COLOR_OPTIONS = 8;
const int DOTS_COUNT = 3;
const int DOTS_DELAY_MS = 400;
const int CLEAR_DELAY_MS = 300;

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
	EXIT
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
        SetCursorPosition(0, 0);
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
        else if (key == 13) {
            system("cls");
            break;
        }
    }
    return choice;
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
        break;
    }
    case 1: {
        color = YELLOW;
        break;
    }
    case 2: {
        color = MAGENTA;
        break;
    }
    case 3: {
        color = RED;
        break;
    }

    case 4: {
        color = CYAN;
        break;
    }
    case 5: {
        color = GREEN;
        break;
    }
    case 6: {
        color = BLUE;
        break;
    }
    case 7: {
        color = GREY;
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
		cout << "Press 'R' to rotate your ship";
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

void PlayersTurn(int key, int& cursorX, int& cursorY,int computerMap[ROWS][COLS], int computerVisibleMap[ROWS][COLS], bool& playerTurn) {
    
    if (key == ARROW_PREFIX) {
        key = _getch();
        if (key == UP && cursorY > 0) cursorY--;
        else if (key == DOWN && cursorY < ROWS - 1) cursorY++;
        else if (key == LEFT && cursorX > 0) cursorX--;
        else if (key == RIGHT && cursorX < COLS - 1) cursorX++;
    }
    
    else if (key == ENTER) {
        if (computerVisibleMap[cursorY][cursorX] == E) {
            if (computerMap[cursorY][cursorX] == S) {
                computerVisibleMap[cursorY][cursorX] = H;
            }
            else {
                computerVisibleMap[cursorY][cursorX] = M;
                playerTurn = false;
            }
            SetCursorPosition(cursorX, cursorY);
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

void ComputersTurn(const int difficulty, int playerMap[ROWS][COLS], bool& playersTurn) {
    switch (difficulty) {
    case CASUAL: {
        int x = GetRandomNum(0, COLS-1);
        int y = GetRandomNum(0, ROWS-1);
        if (playerMap[x][y] == S) playerMap[x][y] = H;
        else if (playerMap[x][y] == E) { playerMap[x][y] = M; playersTurn = true; }
        Sleep(500);
    }
    }
}

int main()
{   
    srand(time(0));
    ShowConsoleCursor(false);
    int theme = CYAN;
    ShowGameName(theme);
    string menu[MAIN_MENU_ITEMS] = { "New Game", "Load Game", "Options", "Exit" };
    string modes[GAME_MODE_ITEMS] = { "Player vs Computer", "Computer vs Computer", "Back to main menu" };
    string difficulties[GAME_DIFFICULTY] = { "CASUAL", "HARD",};
    while (true) {
        int menuChoice = ShowMenus(menu, MAIN_MENU_ITEMS, theme);
        switch (menuChoice) {
        case NEW_GAME: {
            int mode = ShowMenus(modes, GAME_MODE_ITEMS, theme);
            switch (mode) {
            case PLAYER_VS_COMPUTER: {
                int playerMap[ROWS][COLS] = { {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E},
                                              {E,E,E,E,E,E,E,E,E,E}, };
                int computerVisibleMap[ROWS][COLS] = { {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E},
                                                      {E,E,E,E,E,E,E,E,E,E}, };
                int computerMap[ROWS][COLS];
                int difficulty = ShowMenus(difficulties, GAME_DIFFICULTY, theme);
                int placement = ShipPlacesOption(theme);
                switch (placement) {
                case MANUAL: {
                    PlaceShips(playerMap); break;
                }

                case RANDOM: {
                    RandomMap(playerMap); break;
                }
                }
                RandomMap(computerMap);
                bool playerTurn = true;
                int cursorX = 0, cursorY = 0;

                while (true) {
                    ShowMaps(playerMap, computerVisibleMap, theme, cursorX, cursorY, true, playerTurn);
                    if (playerTurn) {
                        int key = _getch();
                        PlayersTurn(key, cursorX, cursorY, computerMap, computerVisibleMap, playerTurn); 
                        if (key == ESC) break;
                    }
                    else ComputersTurn(difficulty, playerMap, playerTurn);
                    
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