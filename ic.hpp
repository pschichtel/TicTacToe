#ifndef INCLUDE_GUARD_IC_HPP
#define INCLUDE_GUARD_IC_HPP

///// Includes /////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500
#include <string>
#include <tchar.h>
#include <windows.h>
////////////////////////////////////////////////////////////////////////////////

///// Macros ///////////////////////////////////////////////////////////////////
#ifndef CONSOLE_FULLSCREEN_MODE
#define CONSOLE_FULLSCREEN_MODE 1
#endif
#ifndef CONSOLE_WINDOWED_MODE
#define CONSOLE_WINDOWED_MODE 2
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
////////////////////////////////////////////////////////////////////////////////

namespace ic
{
	///// Color ////////////////////////////////////////////////////////////////
	typedef int Color;
	////////////////////////////////////////////////////////////////////////////

	///// TextColor ////////////////////////////////////////////////////////////
	enum TextColor
	{
		FG_BLACK		= 0,
		FG_DARKRED		= FOREGROUND_RED,
		FG_DARKGREEN	= FOREGROUND_GREEN,
		FG_DARKBLUE		= FOREGROUND_BLUE,
		FG_OCHER		= FOREGROUND_RED | FOREGROUND_GREEN,
		FG_VIOLET		= FOREGROUND_RED | FOREGROUND_BLUE,
		FG_TURQUOISE	= FOREGROUND_GREEN | FOREGROUND_BLUE,
		FG_GREY			= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

		FG_DARKGREY		= FOREGROUND_INTENSITY | FG_BLACK,
		FG_RED			= FOREGROUND_INTENSITY | FG_DARKRED,
		FG_GREEN		= FOREGROUND_INTENSITY | FG_DARKGREEN,
		FG_BLUE			= FOREGROUND_INTENSITY | FG_DARKBLUE,
		FG_YELLOW		= FOREGROUND_INTENSITY | FG_OCHER,
		FG_PINK			= FOREGROUND_INTENSITY | FG_VIOLET,
		FG_LIGHTBLUE	= FOREGROUND_INTENSITY | FG_TURQUOISE,
		FG_WHITE		= FOREGROUND_INTENSITY | FG_GREY
	};
	////////////////////////////////////////////////////////////////////////////

	///// BgColor //////////////////////////////////////////////////////////////
	enum BgColor
	{
		BG_BLACK		= 0,
		BG_DARKRED		= BACKGROUND_RED,
		BG_DARKGREEN	= BACKGROUND_GREEN,
		BG_DARKBLUE		= BACKGROUND_BLUE,
		BG_OCHER		= BACKGROUND_RED | BACKGROUND_GREEN,
		BG_VIOLET		= BACKGROUND_RED | BACKGROUND_BLUE,
		BG_TURQUOISE	= BACKGROUND_GREEN | BACKGROUND_BLUE,
		BG_GREY			= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

		BG_DARKGREY		= BACKGROUND_INTENSITY | BG_BLACK,
		BG_RED			= BACKGROUND_INTENSITY | BG_DARKRED,
		BG_GREEN		= BACKGROUND_INTENSITY | BG_DARKGREEN,
		BG_BLUE			= BACKGROUND_INTENSITY | BG_DARKBLUE,
		BG_YELLOW		= BACKGROUND_INTENSITY | BG_OCHER,
		BG_PINK			= BACKGROUND_INTENSITY | BG_VIOLET,
		BG_LIGHTBLUE	= BACKGROUND_INTENSITY | BG_TURQUOISE,
		BG_WHITE		= BACKGROUND_INTENSITY | BG_GREY
	};
	////////////////////////////////////////////////////////////////////////////

	///// Console //////////////////////////////////////////////////////////////
	class Console
	{
		// Origin of the coordinate system
		static const COORD origin;


		// Handle: Console window
		HWND hWnd;
		// Handle: Standard output device
		HANDLE hConsoleOutput;
		// Trigger: Window buffered mode
		bool wndBufMode;


		// Already implemented functions without declaration in <windows.h>
		typedef BOOL (WINAPI *SETCONSOLEDISPLAYMODE) (HANDLE, DWORD, PCOORD);
		SETCONSOLEDISPLAYMODE SetConsoleDisplayMode;


		// Constructors
		Console ();
	public:
		~Console ();

		// Singleton access
		static Console& getInstance ();

		// Hide/Show console
		void hide ();
		void show ();

		// Minimize/Maximize/Restore console
		void minimize ();
		void maximize ();
		void restore ();

		// Clear console (Only color, only text or both)
		void clearColor (Color color = FG_WHITE | BG_BLACK);
		void clearText (TCHAR character = TEXT(' '));
		void clear (Color color = FG_WHITE | BG_BLACK, TCHAR character = TEXT(' '));

		// Get/Set: Color
		Color getColor () const;
		void setColor (Color color);

		// Get/Set: Text color
		TextColor getTextColor () const;
		void setTextColor (TextColor color);

		// Get/Set: Background color
		BgColor getBgColor () const;
		void setBgColor (BgColor color);

		// Get/Set: Cursor position
		int getCurPosX () const;
		int getCurPosY () const;
		void setCurPos (int x, int y);

		// Get/Set: Cursor size
		int getCurSize () const;
		void setCurSize (int size);

		// Enable/Disable: Window buffered mode
		bool isWndBufMode () const;
		void enableWndBufMode ();
		void disableWndBufMode ();

		// Enable/Disable: Window fullscreen mode
		bool isWndFSMode () const;
		void enableWndFSMode ();
		void disableWndFSMode ();

		// Get/Set: Window position
		int getWndPosX () const;
		int getWndPosY () const;
		void setWndPos (int x, int y);

		// Get/Set: Window buffer size
		int getWndBufSizeX () const;
		int getWndBufSizeY () const;
		void setWndBufSize (int x, int y);

		// Get/Set: Window size
		int getWndSizeX () const;
		int getWndSizeY () const;
		void setWndSize (int x, int y);

		// Get: Maximal window size
		int getMaxWndSizeX () const;
		int getMaxWndSizeY () const;

		// Get/Set: Title
		std::basic_string<TCHAR> getTitle () const;
		void setTitle (const std::basic_string<TCHAR>& title);


	private:
		// Helper
		CONSOLE_CURSOR_INFO getCCI () const;
		CONSOLE_SCREEN_BUFFER_INFO getCSBI () const;

		// Helper: getTextColor()/getBgColor()
		inline BgColor getBgColor (Color color) const { return static_cast<BgColor>(color & 0xF0); }
		inline TextColor getTextColor (Color color) const { return static_cast<TextColor>(color & 0x0F); }

		// Helper: setWndSize()
		void zeroWndSize ();

		// Forbidden
		Console (const Console&);
		Console& operator= (const Console&);
	};

	extern Console& con;
	////////////////////////////////////////////////////////////////////////////

	///// Shorties /////////////////////////////////////////////////////////////
	namespace shorties
	{
		inline void hide () { con.hide(); }
		inline void show () { con.show(); }

		inline void minimize () { con.minimize(); }
		inline void maximize () { con.maximize(); }
		inline void restore () { con.restore(); }

		inline void clrcol (Color color = FG_WHITE | BG_BLACK) { con.clearColor(color); }
		inline void clrtext (TCHAR character = TEXT(' ')) { con.clearText(character); }
		inline void clrscr (Color color = FG_WHITE | BG_BLACK, TCHAR character = TEXT(' ')) { con.clear(color,character); }

		inline void textcolor (TextColor color) { con.setTextColor(color); }
		inline void bgcolor (BgColor color) { con.setBgColor(color); }

		inline void gotoxy (int x, int y) { con.setCurPos(x,y); }
		inline void home () { con.setCurPos(0,0); }
		inline void cursize (int size) { con.setCurSize(size); }

		inline void resize (int x, int y) { con.setWndSize(x,y); }

		inline void title (const std::basic_string<TCHAR>& title) { con.setTitle(title); }
	}
	////////////////////////////////////////////////////////////////////////////
}

#endif
