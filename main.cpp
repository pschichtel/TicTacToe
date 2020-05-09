#include <iostream>
#include "TTT.hpp"

using namespace ic;
using namespace std;

string readLine(int);
int readNum(int);
int getNum(string);
int getNum(const char*);
int* getSize(void);
int* getCoord(string);
inline void printError(string);
inline void printCurrentPlayer(TTT*);
inline void printWinner(int);
string trim(string);
string ltrim(string);
string rtrim(string);
string int2string(int);
int main(int, char**);

string readLine(int bufSize = 512)
{
    bufSize++;
    string input;
    char* buffer = new char[bufSize];
    cin.getline(buffer, bufSize);
    input = buffer;
    delete buffer;
    fflush(stdin);

    return trim(input);
}

int readNum(int bufSize = 512)
{
    return getNum(readLine(bufSize));
}

int getNum(string str)
{
    return getNum(str.c_str());
}

int getNum(const char* str)
{
    return atoi(str);
}

int* getSize(void)
{
    int width;
    int height;
    int winLen;

    do
    {
        con.setColor(FG_GREEN);
        cout << "Die Breite: ";
        con.setColor(FG_WHITE);
        width = readNum();
        if (width < 3)
        {
            printError("Das Feld muss mindestens 3 Spalten breit sein!");
        }
    }
    while (width < 3);

    do
    {
        con.setColor(FG_GREEN);
        cout << "Die Hoehe: ";
        con.setColor(FG_WHITE);
        height = readNum();
        if (height < 3)
        {
            printError("Das Feld muss mindestens 3 zeilen hoch sein!");
        }
    }
    while (height < 3);

    do
    {
        con.setColor(FG_GREEN);
        cout << "Die Siegeslaenge: ";
        con.setColor(FG_WHITE);
        winLen = readNum();
        if (winLen < 2)
        {
            printError("Die Siegeslänge der Reihen muss mindestens 2 sein!");
        }
    }
    while (winLen < 2);
    cout << endl << endl;

    int* data = new int[3];
    data[0] = width;
    data[1] = height;
    data[2] = winLen;
    return  data;
}

int* getCoord(string delim)
{
    string input;
    int x;
    int y;
    bool cond;

    do
    {
        unsigned int delimPos;
        do
        {
            con.setColor(FG_GREEN);
            cout << "Geben Sie die Koordinate ein [X" << delim << "Y]: ";
            con.setColor(FG_WHITE);
            input = readLine();
            delimPos = input.find(delim);
            cond = delimPos == string::npos;
            if (cond)
            {
                printError("Ihr Eingabe enthaelt kein Trennzeichen (" + delim + ") !");
            }
        }
        while (cond);
        x = getNum(input.substr(0, delimPos));
        y = getNum(input.substr(delimPos + 1));
        cond = (y < 0 || x < 0);
        if (cond)
        {
            printError("Negative Koordinaten sind nicht erlaubt!");
        }
    }
    while (cond);

    int* coord = new int[2];
    coord[0] = x;
    coord[1] = y;

    return coord;
}

inline void printError(string msg)
{
    con.setColor(FG_RED);
    cout << endl << msg << endl;
    con.setColor(FG_WHITE);
}

inline void printCurrentPlayer(TTT* game)
{
    Color color;
    int player = game->get_player();
    if (player == 1)
    {
        color = FG_GREEN;
    }
    else
    {
        color = FG_RED;
    }
    con.setColor(FG_YELLOW);
    cout << "Spieler ";
    con.setColor(color);
    cout << player;
    con.setColor(FG_YELLOW);
    cout << " ist an der Reihe!" << std::endl;
    con.setColor(FG_WHITE);
}

inline void printWinner(int player)
{
    Color color;
    if (player == 1)
    {
        color = FG_GREEN;
    }
    else
    {
        color = FG_RED;
    }
    con.setColor(FG_GREEN);
    cout << endl << "Spieler ";
    con.setColor(color);
    cout << player;
    con.setColor(FG_GREEN);
    cout << " hat das Spiel gewonnen!" << std::endl;
    con.setColor(FG_WHITE);
}

string trim(string str)
{
    return rtrim(ltrim(str));
}

string ltrim(string str)
{
    int start = 0;
    int length = str.size();

    for (int i = 0; i < length; i++)
    {
        char sign = static_cast<char>(str[i]);
        if (sign >= 0 && sign <= 32)
        {
            start++;
        }
        else
        {
            break;
        }
    }

    return str.substr(start);
}

string rtrim(string str)
{
    int length = str.size();

    for (int i = length - 1; i >= 0; i--)
    {
        char sign = static_cast<char>(str[i]);
        if (sign >= 0 && sign <= 32)
        {
            length--;
        }
        else
        {
            break;
        }
    }

    return str.substr(0, length);
}

string lfill(string str, unsigned int length, string fillchar = " ")
{
    while (str.size() < length)
    {
        str = fillchar + str;
    }
    return str;
}

string rfill(string str, unsigned int length, string fillchar = " ")
{
    while (str.size() < length)
    {
        str += fillchar;
    }
    return str;
}

string int2string(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
}


int main(int argc, char** argv)
{
    con.setTitle("TicTacToe by Phillip Schichtel");

    int* data = getSize();
    int winWidth = data[0] * 2;
    int winHeight = data[1] + 10;
    if (winWidth < 50)
    {
        winWidth = 50;
    }

    TTT* game = new TTT(data[0], data[1], data[2], 1);

    delete data;

    con.clear();
    con.setCurPos(0, 4);
    con.setColor(FG_YELLOW);
    cout << "Wer als erstes eine " << game->get_winLen() << "er Reihe erreicht gewinnt!" << endl;
    con.setColor(FG_GREEN);
    cout << endl << "<ENTER> zum Fortfahren" << endl;
    cin.get();
    fflush(stdin);

    con.setWndSize(winWidth, winHeight);

    int winner = 0;

    while (true)
    {
        con.clear();
        con.setCurPos(0, 0);

        game->printField();

        if (winner > 0)
        {
            break;
        }

        printCurrentPlayer(game);

        bool cond;
        do
        {
            int* coord = getCoord(",");
            cond = game->setCoord(coord[0], coord[1]);

            if (!cond)
            {
                printError("Die Koordinate wurde bereits belegt oder liegt ausserhalb des Spielfelds!");
            }
            delete coord;
        }
        while (!cond);

        winner = game->checkGlobal();

        game->swap_player();
    }

    delete game;

    printWinner(winner);

    fflush(stdin);
    cin.get();
    return 0;
}
