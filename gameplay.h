#pragma once
#include "utils.h"
#include "menu.h"

bool CanPlaceShip(int map[ROWS][COLS], int, int, int, bool);
void PlaceShip(int map[ROWS][COLS], int, int, int, bool);
void DeleteShip(int map[ROWS][COLS], int, int, int*);
void PlaceShips(int map[ROWS][COLS]);
void RandomMap(int compMap[ROWS][COLS]);
bool IsShipSunk(int map[ROWS][COLS], int, int, bool visited[ROWS][COLS]);
void MarkPerimeterAsMisses(int map[ROWS][COLS], int, int);
void SinkShip(int map[ROWS][COLS], int, int, bool visited[ROWS][COLS]);
void PlayersTurn(int, int&, int&, int computerMap[ROWS][COLS], int computerVisibleMap[ROWS][COLS], bool&, int&);
int GetRandomNum(int, int);
void ComputersTurn(const int, int playerMap[ROWS][COLS], bool&, int&);