#include "utils.h"
#include "saveAndLoad.h"
#include "menu.h"
#include "gameplay.h"

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
                    RESUME_GAME1:
                    ShowMaps(playerMap, computerVisibleMap, theme, cursorX, cursorY, false, true, playerTurn);
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
                                    goto RESUME_GAME1;
                                    system("cls");
                                }
                                break; }
                            case RESUME: { goto RESUME_GAME1; break; }
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
                int computer1Map[ROWS][COLS] = { {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E},
                                                 {E, E, E, E, E, E, E, E, E, E} };
				int computer2Map[ROWS][COLS];
                cout << "Choose game dufficulty:" << endl;
                difficulty = ShowDifficulties(theme);
                placement = ShipPlacesOption(theme);
                switch (placement) {
                case MANUAL: {
                    PlaceShips(computer1Map); break;
                }

                case RANDOM: {
                    RandomMap(computer1Map); break;
                }
                }
                RandomMap(computer2Map);
                bool computer1Turn = true;
                int cursorX = 0, cursorY = 0;
                int status = 0;
                while (true) {
                RESUME_GAME2:
                    ShowMaps(computer1Map, computer2Map, theme, cursorX, cursorY, false, false, computer1Turn);
                    SetColor(theme, BLACK);
                    cout << "\nComputers 1 ships remain - " << playerShips << "\tComputers 2 ships remain - " << computerShips << endl;
                    cout << "JUST WATCH THE GAME";
                    if (computer1Turn) {
                        ComputersTurn(difficulty, computer2Map, computer1Turn, computerShips);
                        if (_kbhit()) {
                            int key = _getch();
                            if (key == ESC) {
                                int escMenu = ShowEscMenu(theme);
                                switch (escMenu) {
                                case NEW_GAME: { goto START_NEW_GAME; break; }
                                case SAVE_QUIT: {
                                    system("cls");
                                    SetColor(theme, BLACK);
                                    cout << "YOU CAN'T SAVE GAME IN COMPUTER VS COMPUTER MODE";
                                    Sleep(2000);
                                    system("cls");
                                    goto RESUME_GAME2;
                                }
                                case RESUME: { goto RESUME_GAME2; break; }
                                }
                            }
                            break;
                        }
                    }
                    else ComputersTurn(difficulty, computer1Map, computer1Turn, playerShips);
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
                }}
			case BACK_TO_MAIN_MENU: continue;
            }
            break;
        }
        case LOAD_GAME: {
            bool fileExist = LoadMapsFromFile(playerMap, computerMap, computerVisibleMap,
                                        theme, difficulty, playerShips, computerShips);
            if(fileExist) goto START_GAME;
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