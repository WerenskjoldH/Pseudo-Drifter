#ifndef PRECISION_H
#define PRECISION_H

#define PI 3.14159265f

namespace rn 
{
	// Borrowing this straight from Ian Millington, he brings up a good point for how it can allow an ease in changing precision
	typedef float real;

	real toDegrees(real rads)
	{
		return rads * (180.f / PI);
	}

	real toRadians(real degs)
	{
		return degs * (PI / 180.f);
	}

}

#endif