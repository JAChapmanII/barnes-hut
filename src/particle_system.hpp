/** {{{
 * Copyright 2010 Jeff Chapman.
 *
 * This file is a part of aos
 *
 * aos is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aos.  If not, see <http://www.gnu.org/licenses/>.
 *
 */// }}}
#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <string>
#include <ostream>

class ParticleSystem
{
	friend std::ostream& operator<<( std::ostream& out,
		const ParticleSystem& toPrint );

	public:
		ParticleSystem();
		ParticleSystem( std::string fileName );

		void load( std::string fileName );

	private:
		float** mParticles;
};

#endif // PARTICLE_SYSTEM_HPP
