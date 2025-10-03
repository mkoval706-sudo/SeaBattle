#include "menu.h"

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
                if (choice == -1) choice = size - 1;
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
    string difficulties[GAME_DIFFICULTY] = { "CASUAL", "HARD" };
    int choice = ShowMenus(difficulties, GAME_DIFFICULTY, color);
    return choice;
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
        cout << colors[choice] << endl;
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

void ShowMaps(int map1[ROWS][COLS], int map2[ROWS][COLS], int color, int cursorX, int cursorY, bool playerMap, bool computerMap, bool playerTurn) {
    system("cls");
    SetColor(color, BLACK);
    SetCursorPosition(0, 0);
    cout << "      Player 1" << endl;
    ShowMap(0, 2, map1, cursorX, cursorY, playerMap, true);
    SetColor(color, BLACK);
    SetCursorPosition(30, 0);
    cout << "      Player 2" << endl;
    ShowMap(30, 2, map2, cursorX, cursorY, computerMap, playerTurn);
}

void ShowShipsList(int* shipsAvailable, int x, int y) {
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