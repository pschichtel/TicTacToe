#include "TTT.hpp"

using namespace std;

TTT::TTT(void)
{
    this->init_members(0, 0, 0, 1);
}
TTT::TTT(int width, int height)
{
    this->init_members(width, height, 0, 1);
    this->makeMatrix(width, height);
}
TTT::TTT(int width, int height, int winLen)
{
    this->init_members(width, height, winLen, 1);
    this->makeMatrix(width, height);
}
TTT::TTT(int width, int height, int winLen, int startPlayer)
{
    this->init_members(width, height, winLen, startPlayer);
    this->makeMatrix(width, height);
}

TTT::~TTT()
{
    this->destroyMatrix();
    this->log.close();
}

int TTT::set_winLen(int winLen)
{
    if (this->matrix == NULL)
    {
        return -1;
    }
    else
    {
        int max = min(this->width, this->height);
        if (winLen > max)
        {
            this->winLen = max;
            return max;
        }
        else if (winLen < 2)
        {
            this->winLen = 2;
            return -1;
        }
        else
        {
            this->winLen = winLen;
            return winLen;
        }
    }
}

int TTT::get_winLen(void)
{
    return this->winLen;
}

void TTT::makeMatrix(int width, int height)
{
    this->width = width;
    this->height = height;
    this->matrix = new int*[width];
    for (int i = 0; i < width; i++)
    {
        this->matrix[i] = new int[height];
        for (int j = 0; j < height; j++)
        {
            this->matrix[i][j] = 0;
        }
    }
}

void TTT::printField(void)
{
    std::cout << std::endl << std::endl;
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int state = this->matrix[x][y];
            std::cout << state << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

std::string TTT::returnField(void)
{
    std::string field = "";
    for (int y = height - 1; y >= 0; y--)
    {
        std::string tmp = "";
        for (int x = 0; x < width; x++)
        {
            tmp += " " + this->matrix[x][y];
        }
        field += tmp.substr(1) + "\n";
    }
    return field;
}

bool TTT::setCoord(int x, int y)
{
    if (x >= this->width || y >= this->height)
    {
        return false;
    }
    if (this->matrix[x][y] == 0)
    {
        this->matrix[x][y] = this->player;
        return true;
    }
    else
    {
        return false;
    }
}

int TTT::get_player(void)
{
    return this->player;
}
 void TTT::swap_player(void)
{
    if (this->player == 1)
    {
        this->player = 2;
    }
    else
    {
        this->player = 1;
    }
}

int TTT::check(const int startX, const int startY, const int searchWidth, const int searchHeight)
{
    const int xLimit = startX + searchWidth;
    const int yLimit = startY + searchHeight;

    /**
     * Von unten noch oben
     */
    for (int x = startX; x < xLimit; x++)
    {
        this->lastState = this->matrix[x][startY];
        this->currentRowLength = 0;
        for (int y = startY; y < yLimit; y++)
        {
            this->log_coord(1, x, y);
            if (this->winLen_reached(this->matrix[x][y]))
            {
                return this->lastState;
            }
            this->lastState = this->matrix[x][y];
        }
        this->currentRowLength = 0;
    }

    /**
     * Von links nach rechts
     */
    for (int y = startY; y < yLimit; y++)
    {
        this->lastState = this->matrix[startX][y];
        this->currentRowLength = 0;
        for (int x = startX; x < xLimit; x++)
        {
            this->log_coord(2, x, y);
            if (this->winLen_reached(this->matrix[x][y]))
            {
                return this->lastState;
            }
            this->lastState = this->matrix[x][y];
        }
        this->currentRowLength = 0;
    }

    /**
     * Diagonal /
     */
    for (int x = xLimit - 1; x >= startX; x--)
    {
        this->lastState = this->matrix[x][startY];
        this->currentRowLength = 0;
        for (int y = startY; y < yLimit; y++)
        {
            for (int i = 0; i < this->winLen; i++)
            {
                int X = x + i, Y = y + i;
                if (X < xLimit && Y < yLimit)
                {
                    this->log_coord(3, X, Y);
                    if (this->winLen_reached(this->matrix[X][Y]))
                    {
                        return this->lastState;
                    }
                    this->lastState = this->matrix[X][Y];
                }
            }
            this->currentRowLength = 0;
        }
    }

    /**
     * Diagonal \
     */
    for (int x = startX; x < xLimit; x++)
    {
        this->lastState = this->matrix[x][startY];
        this->currentRowLength = 0;
        for (int y = startY; y < yLimit; y++)
        {
            for (int i = 0; i < this->winLen; i++)
            {
                int X = x - i, Y = y + i;
                if ((X < xLimit && X >= startX) && Y < yLimit)
                {
                    this->log_coord(4, X, Y);
                    if (this->winLen_reached(this->matrix[X][Y]))
                    {
                        return this->lastState;
                    }
                    this->lastState = this->matrix[X][Y];
                }
            }
            this->currentRowLength = 0;
        }
    }
    return 0;
}

int TTT::checkGlobal()
{
    return this->check(0, 0, this->width, this->height);
}

int TTT::checkCoord(int x, int y)
{
    x -= this->winLen;
    y -= this->winLen;
    if (x < 0)
    {
        x = 0;
    }
    if (y < 0)
    {
        y = 0;
    }

    int searchWidth = 2 * this->winLen + 1;
    int searchHeight = searchWidth;
    if (x + searchWidth > this->width)
    {
        searchWidth = this->width - x;
    }
    if (y + searchHeight > this->height)
    {
        searchHeight = this->height - y;
    }
    return this->check(x, y, searchWidth, searchHeight);
}


// 2 >| 0 1 0
// 1 >| 0 1 0
// 0 >| 0 1 0
//    +------
//      ^ ^ ^
//      0 1 2
//
// winLen 3

bool TTT::winLen_reached(int state)
{
    if (state > 0)
    {
        if (state == this->lastState)
        {
            this->currentRowLength++;
            if (this->currentRowLength == this->winLen)
            {
                this->currentRowLength = 0;
                return true;
            }
            return false;
        }
        else
        {
            this->currentRowLength = 1;
            return false;
        }
    }
    else
    {
        this->currentRowLength = 0;
        return false;
    }
}

void TTT::destroyMatrix(void)
{
    for (int i = 0; i < width; i++)
    {
        delete this->matrix[i];
    }
    delete this->matrix;
}

void TTT::init_members(int width, int height, int winLen, int player)
{
    this->width = width;
    this->height = height;
    this->set_winLen(winLen);
    if (player <= 1)
    {
        this->player = 1;
    }
    else
    {
        this->player = 2;
    }
    this->matrix = NULL;
    this->lastState = 0;
    this->currentRowLength = 0;
    this->log.open("coord.log.txt", std::ios::out);
}

void TTT::log_coord(int mode, int x, int y)
{
    switch (mode)
    {
        case 1:
            this->log << "[|] ";
            break;
        case 2:
            this->log << "[-] ";
            break;
        case 3:
            this->log << "[/] ";
            break;
        case 4:
            this->log << "[\\] ";
            break;
    }


    std::string width = this->int2string(this->width);
    std::string height = this->int2string(this->height);
    std::string winLen = this->int2string(this->winLen);

    //this->log << x << "," << y << std::endl;
    this->log << this->lfill(x, width.size(), " ") << "," << this->lfill(y, height.size(), " ") << " " << this->matrix[x][y] << " " << this->lfill(this->currentRowLength, winLen.size(), " ") << std::endl;
}
std::string TTT::lfill(std::string str, unsigned int length, std::string fillchar = " ")
{
    while (str.size() < length)
    {
        str = fillchar + str;
    }
    return str;
}

std::string TTT::lfill(int str, unsigned int length, std::string fillchar = " ")
{
    return this->lfill(this->int2string(str), length, fillchar);
}

std::string TTT::int2string(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}
