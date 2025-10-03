#include "saveAndLoad.h"

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
    saved << difficulty << endl;
    saved << countShips1 << endl;
    saved << countShips2;
    saved.close();
    return true;
}

bool LoadMapsFromFile(int matr1[ROWS][COLS], int matr2[ROWS][COLS],
    int visibleMatr[ROWS][COLS], int color, int& difficulty,
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
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            saved >> visibleMatr[i][j];
        }
    }
    saved >> difficulty;
    saved >> countShips1;
    saved >> countShips2;
    saved.close();
    return true;
}