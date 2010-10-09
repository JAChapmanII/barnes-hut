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

/**
 * Class used to represent a point with an x, y and mass.
 */
class Particle
{
	public:
		float x, y;
		float m;
		float ax, ay;

		Particle() :
			x(0),
			y(0),
			m(0),
			ax(0),
			ay(0)
		{
		}

		Particle( float iX, float iY, float iM ) :
			x( iX ),
			y( iY ),
			m( iM ),
			ax(0),
			ay(0)
		{
		}
};

