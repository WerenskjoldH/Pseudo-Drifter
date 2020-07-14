#ifndef MATRIX_H
#define MATRIX_H

/*
	File: matrix.h
	Description: This file is from my Ronin Math Library, it implements matrixes that can be used alongside the Ronin vector class

	https://github.com/WerenskjoldH/Ronin-Math-Libary
*/

#include <math.h>
#include <iostream>

#include "precision.h"

namespace rn {

	class matrix {

	private:
		real **ij;

		void initMatrix()
		{
			if (rowSize == NULL || columnSize == NULL)
			{
				std::cout << "Matrix was given no dimensions" << std::endl;
				return;
			}

			ij = new real*[rowSize];
			for (int x = 0; x < rowSize; x++)
				ij[x] = new real[columnSize];

			for (int x = 0; x < rowSize; x++)
				for (int y = 0; y < rowSize; y++)
					ij[x][y] = 0.f;
		}

	public:
		int rowSize, columnSize;

		// Constructors
		matrix() : rowSize(1), columnSize(1)
		{
			initMatrix();
		}

		matrix(int rows, int columns) : rowSize(rows), columnSize(columns)
		{
			initMatrix();
		}

		// Copy Constructor
		matrix(matrix& const m) : rowSize(m.rowSize), columnSize(m.columnSize)
		{
			ij = new real*[m.rowSize];
			for (int x = 0; x < m.rowSize; x++)
				ij[x] = new real[m.columnSize];

			for (int x = 0; x < rowSize; x++)
				for (int y = 0; y < rowSize; y++)
					ij[x][y] = m(x, y);
		}

		// Deconstructor
		~matrix()
		{
			for (int x = 0; x < rowSize; x++)
				free(ij[x]);

			delete[] ij;
		}

		// Operator Overloads
		matrix& operator=(const matrix& m)
		{
			try
			{
				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < rowSize; y++)
						ij[x][y] = m.ij[x][y];

				return *this;
			}
			catch (std::exception& e)
			{
				throw "Failure to set matrix equal: Make sure rows and column sizes are equal";
			}
		}

		void operator+=(matrix & const m)
		{
			try
			{
				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < rowSize; y++)
						ij[x][y] += m(x, y);
			}
			catch (std::exception &e)
			{
				throw "Failure to add matrices: Make sure rows and column sizes are equal";
			}
		}

		void operator-=(matrix & const m)
		{
			try {
				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < rowSize; y++)
						ij[x][y] -= m(x, y);
			}
			catch (std::exception &e)
			{
				throw "Failure to subtract matrices: Make sure rows and column sizes are equal";
			}
		}

		void operator*=(const real & s)
		{
			for (int x = 0; x < rowSize; x++)
				for (int y = 0; y < rowSize; y++)
					ij[x][y] *= s;
		}

		void operator/=(const real & s)
		{
			for (int x = 0; x < rowSize; x++)
				for (int y = 0; y < rowSize; y++)
					ij[x][y] /= s;
		}

		matrix operator/(const real s)
		{
			matrix temp = *this;

			for (int x = 0; x < rowSize; x++)
				for (int y = 0; y < rowSize; y++)
					temp.ij[x][y] /= s;

			return temp;
		}

		matrix operator-(matrix & const m)
		{
			try
			{
				matrix temp = *this;

				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < rowSize; y++)
						temp.ij[x][y] -= m(x, y);

				return temp;
			}
			catch (std::exception &e)
			{
				throw "Failure to subtract matrices: Make sure rows and column sizes are equal";
			}
		}

		matrix operator+(matrix & const m)
		{
			try
			{
				matrix temp = *this;

				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < rowSize; y++)
						temp.ij[x][y] += m(x, y);

				return temp;
			}
			catch (std::exception &e)
			{
				throw "Failure to add matrices: Make sure rows and column sizes are equal";
			}
		}

		matrix operator*(matrix & const m)
		{
			try
			{
				matrix temp(rowSize, m.columnSize);

				for (int x = 0; x < rowSize; x++)
					for (int y = 0; y < m.columnSize; y++)
						for (int i = 0; i < rowSize; i++)
							temp.ij[x][y] += ij[x][i] * m(i, y);

				return temp;
			}
			catch (std::exception &e)
			{
				throw "Failure to multiply matrices: Make sure rows and column sizes are equal";
			}
		}

		real& operator()(int row, int column)
		{
			return ij[row][column];
		}



	};

	inline matrix operator*(const real s, matrix& const m)
	{
		matrix temp = m;

		for (int x = 0; x < m.rowSize; x++)
			for (int y = 0; y < m.rowSize; y++)
				temp(x, y) *= s;

		return temp;
	}

	inline matrix operator*(matrix&  m, const real s)
	{

		matrix temp = m;

		for (int x = 0; x < m.rowSize; x++)
			for (int y = 0; y < m.rowSize; y++)
				temp(x, y) *= s;

		return temp;
	}

	// Maybe format in the future?
	inline std::ostream& operator<<(std::ostream& os, matrix& m)
	{
		for (int x = 0; x < m.rowSize; x++)
		{
			for (int y = 0; y < m.columnSize; y++)
				os << m(x, y) << " ";
			os << std::endl;
		}

		return os;
	}

	inline std::istream& operator>>(std::istream& is, matrix& m)
	{
		for (int x = 0; x < m.rowSize; x++)
			for (int y = 0; y < m.columnSize; y++)
				is >> m(x, y);

		return is;
	}
}
#endif