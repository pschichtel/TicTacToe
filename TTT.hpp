#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#ifndef __TTT_INCLUDED
#define __TTT_INCLUDED

class TTT
{
    int** matrix;
    int width;
    int height;
    int winLen;
    int player;
    int lastState;
    int currentRowLength;
    int startX;
    int startY;
    int searchWidth;
    int searchHeight;
    std::fstream log;

    int check(const int, const int, const int, const int);

    bool winLen_reached(int);

    void destroyMatrix(void);

    void init_members(int, int, int, int);

    void log_coord(int, int, int);

    std::string lfill(std::string, unsigned int, std::string);
    std::string lfill(int, unsigned int, std::string);

    std::string int2string(int);

public:
    TTT(void);
    TTT(int, int);
    TTT(int, int, int);
    TTT(int, int, int, int);

    virtual ~TTT();

    int set_winLen(int);

    int get_winLen(void);

    void makeMatrix(int, int);

    virtual void printField(void);

    std::string returnField(void);

    bool setCoord(int, int);

    int get_player(void);

    void swap_player(void);

    int checkGlobal(void);

    int checkCoord(int, int);

};

#endif
