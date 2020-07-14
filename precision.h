#ifndef PRECISION_H
#define PRECISION_H

/*
	File: precision.h
	Description: This file is from my Ronin Math Library, it allows the control of the librarys precision through a typedef
					I have also decided to include the toDegrees() and toRadians() functions in order to account for precision changes

	https://github.com/WerenskjoldH/Ronin-Math-Libary
*/

namespace rn 
{
	// Borrowing this straight from Ian Millington, he brings up a good point for how it can allow an ease in changing precision
	typedef float real;

	inline real toDegrees(real rads)
	{
		return rads * (180.f / 3.14159265f);
	}

	inline real toRadians(real degs)
	{
		return degs * (3.14159265f / 180.f);
	}

}

#endif