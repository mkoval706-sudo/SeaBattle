#pragma once
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

void SetColor(int, int);
void SetCursorPosition(int, int);
void ShowConsoleCursor(bool);
