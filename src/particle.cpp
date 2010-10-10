/** {{{
 * Copyright 2010 Jeff Chapman.
 *
 * This file is a part of Barnes-Hut
 *
 * Barnes-Hut is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Barnes-Hut is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Barnes-Hut.  If not, see <http://www.gnu.org/licenses/>.
 *
 */// }}}
#ifndef PARTICLE_CPP
#define PARTICLE_CPP

#include <ostream>
#include <iomanip>
using std::fixed;
using std::setprecision;
using std::setw;

/**
 * Class used to represent a point with an x, y and mass.
 */
class Particle
{
	public:
		long double x, y;
		long double m;
		long double fx, fy;

		Particle() :
			x(0),
			y(0),
			m(0),
			fx(0),
			fy(0)
		{
		}

		Particle( long double iX, long double iY, long double iM ) :
			x( iX ),
			y( iY ),
			m( iM ),
			fx(0),
			fy(0)
		{
		}

		/**
		* Friend function used to print the internals of this to an ostream.
		* @param out : output stream
		* @param toPrint : the Particle that should be printed
		*/
		friend std::ostream& operator<<( std::ostream& out,
			const Particle& toPrint )
		{
			out << "<"
				<< fixed << setprecision( 4 ) << setw( 8 ) << toPrint.x << ", "
				<< fixed << setprecision( 4 ) << setw( 8 ) << toPrint.y << "> ["
				<< fixed << setprecision( 4 ) << setw( 8 ) << toPrint.fx << ", "
				<< fixed << setprecision( 4 ) << setw( 8 ) << toPrint.fy << "] "
				<< fixed << setprecision( 4 ) << setw( 8 ) << toPrint.m;
		}
};

#endif // PARTICLE_CPP
