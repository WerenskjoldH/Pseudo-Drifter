#ifndef CONSOLE_COLORIZER_H
#define CONSOLE_COLORIZER_H

// If you don't want to use the ColorText class, then disable it here
#define USING_COLOR_TEXT true

#define SET_FONT_COLOR(c) ConsoleColorizer::getInstance().setTextColor(c)
#define RESET_CONSOLE_FONT() ConsoleColorizer::getInstance().setTextColor(15)
#define WRITE_CONSOLE_ERROR(errorLocation, errorLevel, errorDescription) ConsoleColorizer::getInstance().writeError(errorLocation, errorLevel, errorDescription)

#if USING_COLOR_TEXT
#define INLINE_COLOR_FONT(s, c) ColorText(s, c)
#endif

#define FONT_BLACK		0
#define FONT_BLUE		1
#define FONT_GREEN		2
#define FONT_CYAN		3
#define FONT_RED		4
#define FONT_MAGENTA	5
#define FONT_BROWN		6
#define FONT_YELLOW		14
#define FONT_WHITE		15

#include <Windows.h>
#include <iostream>

class ConsoleColorizer
{
public:
	static ConsoleColorizer& getInstance()
	{
		// This will instantiate on the first call and then always return the same instance ( also able to be destroyed! )
		static ConsoleColorizer instance;
		return instance;
	}

	// This is a C++11 feature that prohibits the copying of a method
	// We are able to ensure the method is inaccessible in all cases as the compiler will not even bother to generate the function
	// P.S. this is only allowed for this case, if you want a purely virual method, then set "= 0"
	ConsoleColorizer(ConsoleColorizer  const&)	 = delete;
	void operator=(ConsoleColorizer const&) = delete;

	// c is a value from 0 to 15:
	/*
		0	-	BLACK
		1	-	BLUE
		2	-	GREEN
		3	-	CYAN
		4	-	RED
		5	-	MAGENTA
		6	-	BROWN
		7	-	LIGHT GRAY
		8	-	DARK GRAY
		9	-	LIGHT BLUE
		10	-	LIGHT GREEN
		11	-	LIGHT CYAN
		12	-	LIGHT RED
		13	-	LIGHT MAGENTA
		14	-	YELLOW
		15	-	WHITE
	*/
	void setTextColor(int c = 15)
	{
		if (c < 0 || c > 15)
		{
			WRITE_CONSOLE_ERROR("ConsoleColorizer","WARNING" , "Please keep text color in the integer set [0, 15]");
		}

		currentColor = c;
		SetConsoleTextAttribute(consoleHandler, c);
	}

	// This function is primarily used for setting inline color changes that don't change the current stored color
	void temporaryTextColor(int c = 15)
	{
		if (c < 0 || c > 15)
		{
			WRITE_CONSOLE_ERROR("ConsoleColorizer","WARNING" , "Please keep text color in the integer set [0, 15]");
		}

		SetConsoleTextAttribute(consoleHandler, c);
	}

	// If you have set a temporary color, you will need to call this after to restore the console to the prior color
	void resetTemporaryTextColor()
	{
		SetConsoleTextAttribute(consoleHandler, currentColor);
	}

	void writeError(const char* errorLocation, const char* errorLevel, const char* errorDescription)
	{
		SetConsoleTextAttribute(consoleHandler, 4);
		printf("ERROR::%s::%s::%s\n", errorLocation, errorLevel, errorDescription);
		SET_FONT_COLOR(currentColor);
	}

private:
	HANDLE consoleHandler;

	int currentColor = 15;

	ConsoleColorizer()
	{
		consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	}


};

#if USING_COLOR_TEXT

// This can be used to inline color text in an outstream
class ColorText
{
public:
	std::string text;
	int color;

	ColorText(const char* text, int color) : text{ text }, color{ color }
	{}
};

inline std::ostream& operator<<(std::ostream& os, const ColorText& ct)
{
	ConsoleColorizer::getInstance().temporaryTextColor(ct.color);

	std::cout << ct.text;

	ConsoleColorizer::getInstance().resetTemporaryTextColor();

	return os;
}

#endif

#endif