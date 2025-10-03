#pragma once
#include "utils.h"

bool SaveTheme(int);
int LoadColorFromFile(int);
bool SaveGameToFile(int matr1[ROWS][COLS], int matr2[ROWS][COLS], int visibleMatr[ROWS][COLS], int, int, int);
bool LoadMapsFromFile(int matr1[ROWS][COLS], int matr2[ROWS][COLS], int visibleMatr[ROWS][COLS], int, int&, int&, int&);