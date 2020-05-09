///// Includes /////////////////////////////////////////////////////////////////
#include "ic.hpp"
////////////////////////////////////////////////////////////////////////////////

namespace ic
{
	///// Console //////////////////////////////////////////////////////////////
	const COORD Console::origin = {0,0};

	Console::Console ()
		: hWnd(GetConsoleWindow())
		, hConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE))
		, wndBufMode(false)
	{
		HMODULE kernel32 = GetModuleHandle(TEXT("kernel32.dll"));
		SetConsoleDisplayMode = reinterpret_cast<SETCONSOLEDISPLAYMODE>(GetProcAddress(kernel32,"SetConsoleDisplayMode"));

		disableWndBufMode();
		setWndPos(100,100);
		setWndSize(80,25);
		clear();
	}

	Console::~Console ()
	{
		enableWndBufMode();
	}

	Console& Console::getInstance ()
	{
		static Console instance;
		return instance;
	}

	void Console::hide ()
	{
		ShowWindow(hWnd,SW_HIDE);
	}

	void Console::show ()
	{
		ShowWindow(hWnd,SW_SHOW);
	}

	void Console::minimize ()
	{
		ShowWindow(hWnd,SW_MINIMIZE);
	}

	void Console::maximize ()
	{
		ShowWindow(hWnd,SW_MAXIMIZE);
	}

	void Console::restore ()
	{
		ShowWindow(hWnd,SW_NORMAL);
	}

	void Console::clearColor (Color color)
	{
		DWORD attrsWritten;
		FillConsoleOutputAttribute(hConsoleOutput,color,getWndBufSizeX()*getWndBufSizeY(),origin,&attrsWritten);
	}

	void Console::clearText (TCHAR character)
	{
		DWORD charsWritten;
		FillConsoleOutputCharacter(hConsoleOutput,character,getWndBufSizeX()*getWndBufSizeY(),origin,&charsWritten);
	}

	void Console::clear (Color color, TCHAR character)
	{
		clearColor(color);
		clearText(character);
	}

	TextColor Console::getTextColor () const
	{
		return getTextColor(getCSBI().wAttributes);
	}

	void Console::setTextColor (TextColor color)
	{
		SetConsoleTextAttribute(hConsoleOutput,color|getBgColor());
	}

	BgColor Console::getBgColor () const
	{
		return getBgColor(getCSBI().wAttributes);
	}

	void Console::setBgColor (BgColor color)
	{
		SetConsoleTextAttribute(hConsoleOutput,getTextColor()|color);
	}

	Color Console::getColor () const
	{
		return getCSBI().wAttributes;
	}

	void Console::setColor (Color color)
	{
		SetConsoleTextAttribute(hConsoleOutput,color);
	}

	int Console::getCurPosX () const
	{
		return getCSBI().dwCursorPosition.X;
	}

	int Console::getCurPosY () const
	{
		return getCSBI().dwCursorPosition.Y;
	}

	void Console::setCurPos (int x, int y)
	{
		COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hConsoleOutput,pos);
	}

	int Console::getCurSize () const
	{
		CONSOLE_CURSOR_INFO cci = getCCI();

		if(!cci.bVisible)
			return 0;

		return cci.dwSize;
	}

	void Console::setCurSize (int size)
	{
		CONSOLE_CURSOR_INFO cci;

		if(size > 0)
		{
			cci.bVisible = TRUE;
			cci.dwSize = size;
		}
		else
		{
			cci.bVisible = FALSE;
			cci.dwSize = 100;
		}

		SetConsoleCursorInfo(hConsoleOutput,&cci);
	}

	bool Console::isWndBufMode () const
	{
		return wndBufMode;
	}

	void Console::enableWndBufMode ()
	{
		SetConsoleMode(hConsoleOutput,ENABLE_PROCESSED_OUTPUT|ENABLE_WRAP_AT_EOL_OUTPUT);
		wndBufMode = true;
	}

	void Console::disableWndBufMode ()
	{
		SetConsoleMode(hConsoleOutput,ENABLE_PROCESSED_OUTPUT);
		setWndBufSize(getWndSizeX()+1,getWndSizeY()+1);
		wndBufMode = false;
	}

	bool Console::isWndFSMode () const
	{
		DWORD flags;
		GetConsoleDisplayMode(&flags);

		return flags & CONSOLE_FULLSCREEN_MODE;
	}

	void Console::enableWndFSMode ()
	{
		COORD newScreenBufferDimensions;
		SetConsoleDisplayMode(hConsoleOutput,CONSOLE_FULLSCREEN_MODE,&newScreenBufferDimensions);
	}

	void Console::disableWndFSMode ()
	{
		COORD newScreenBufferDimensions;
		SetConsoleDisplayMode(hConsoleOutput,CONSOLE_WINDOWED_MODE,&newScreenBufferDimensions);
	}

	int Console::getWndPosX () const
	{
		RECT rect;
		GetWindowRect(hWnd,&rect);

		return rect.left;
	}

	int Console::getWndPosY () const
	{
		RECT rect;
		GetWindowRect(hWnd,&rect);

		return rect.top;
	}

	void Console::setWndPos (int x, int y)
	{
		SetWindowPos(hWnd,0,x,y,0,0,SWP_NOACTIVATE|SWP_NOOWNERZORDER|SWP_NOSIZE|SWP_NOZORDER);
	}

	int Console::getWndBufSizeX () const
	{
		return getCSBI().dwSize.X;
	}

	int Console::getWndBufSizeY () const
	{
		return getCSBI().dwSize.Y;
	}

	void Console::setWndBufSize (int x, int y)
	{
		if(!wndBufMode)
			return;

		COORD size;
		size.X = x;
		size.Y = y;
		SetConsoleScreenBufferSize(hConsoleOutput,size);
	}

	int Console::getWndSizeX () const
	{
		return getCSBI().srWindow.Right - getCSBI().srWindow.Left + 1;
	}

	int Console::getWndSizeY () const
	{
		return getCSBI().srWindow.Bottom - getCSBI().srWindow.Top + 1;
	}

	void Console::setWndSize (int x, int y)
	{
		if(!wndBufMode)
		{
			zeroWndSize();

			COORD bufSize;
			bufSize.X = min(x,getMaxWndSizeX());
			bufSize.Y = min(y,getMaxWndSizeY());
			SetConsoleScreenBufferSize(hConsoleOutput,bufSize);
		}

		SMALL_RECT wndSize;
		wndSize.Top = 0;
		wndSize.Left = 0;
		wndSize.Right = min(x,getMaxWndSizeX()) - 1;
		wndSize.Bottom = min(y,getMaxWndSizeY()) - 1;
		SetConsoleWindowInfo(hConsoleOutput,TRUE,&wndSize);
	}

	int Console::getMaxWndSizeX () const
	{
		return GetLargestConsoleWindowSize(hConsoleOutput).X;
	}

	int Console::getMaxWndSizeY () const
	{
		return GetLargestConsoleWindowSize(hConsoleOutput).Y;
	}

	std::basic_string<TCHAR> Console::getTitle () const
	{
		const int MAX_TITLE_LEN = 64 * 1024;

		TCHAR title [MAX_TITLE_LEN];
		GetConsoleTitle(title,MAX_TITLE_LEN);

		return std::basic_string<TCHAR>(title);
	}

	void Console::setTitle (const std::basic_string<TCHAR>& title)
	{
		SetConsoleTitle(title.c_str());
	}

	CONSOLE_CURSOR_INFO Console::getCCI () const
	{
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hConsoleOutput,&cci);

		return cci;
	}

	CONSOLE_SCREEN_BUFFER_INFO Console::getCSBI () const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsoleOutput,&csbi);

		return csbi;
	}

	void Console::zeroWndSize ()
	{
		SMALL_RECT wndSize;
		wndSize.Top = 1;
		wndSize.Left = 1;
		wndSize.Right = 1;
		wndSize.Bottom = 1;
		SetConsoleWindowInfo(hConsoleOutput,TRUE,&wndSize);

		COORD bufSize;
		bufSize.X = 1;
		bufSize.Y = 1;
		SetConsoleScreenBufferSize(hConsoleOutput,bufSize);
	}

	Console& con = Console::getInstance();
	////////////////////////////////////////////////////////////////////////////
}
