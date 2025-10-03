#pragma once
#include "utils.h"
#include "saveAndLoad.h"

void ShowGameName(int);
int ShowMenus(string*, const int, int);
int ShowMainMenu(int);
int ShowGameModes(int);
int ShowEscMenu(int);
int ShowDifficulties(int);
int ShipPlacesOption(int);
int ShowFinalMenu(int, int);
void Options(int&);
void ShowMap(int, int, int map[ROWS][COLS], int, int, bool, bool);
void ShowMaps(int map1[ROWS][COLS], int map2[ROWS][COLS], int, int, int, bool, bool, bool);
void ShowShipsList(int*, int, int);
int SelectDifficulty(int);