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
#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <string>
#include <ostream>

#include "particle.cpp"

/**
 * Utility class used for loading the descriptions of particles out of a file
 * and into memory.
 */
class ParticleSystem
{
	public:
		/**
		 * Construct a ParticleSystem without any particles in it.
		 */
		ParticleSystem();

		/**
		 * Construct a ParticleSystem and load particles from a file.
		 * @param fileName : filename to load from
		 */
		ParticleSystem( std::string fileName );

		/**
		 * Deconstructor that deallocates all memory used by this.
		 */
		~ParticleSystem();

		/**
		 * Load a file into this system, trashing existing particles.
		 * @param fileName : filename to load from
		 */
		void load( std::string fileName );

		/**
		 * Saves to a file.
		 * @param fileName : filename to save to
		 */
		void save( std::string fileName );

		/**
		 * Clears all memory allocated for this.
		 */
		void clear();

		/**
		 * Returns the size in particles of this system.
		 * @return : size of this system
		 */
		unsigned int getSize() const;

		/**
		 * Returns a long double[3] representing a particle.
		 * @param indici : indice of particle to return
		 * @return : long double[3] representing mParticles[indice]
		 */
		Particle* getParticle( unsigned int indice ) const;

		/**
		* Friend function used to print the internals of this to an ostream.
		* @param out : output stream
		* @param toPrint : the ParticleSystem that should be printed
		*/
		friend std::ostream& operator<<( std::ostream& out,
			const ParticleSystem& toPrint );

		/**
		 * Return the lowest x value of any particle.
		 * @return : leftmost particle's x
		 */
		long double getLeft() const;

		/**
		 * Return the highest x value of any particle.
		 * @return : rightmost particle's x
		 */
		long double getRight() const;

		/**
		 * Return the lowest y value of any particle.
		 * @return : bottom-most particle's y
		 */
		long double getBottom() const;

		/**
		 * Return the highest x valu of any particle.
		 * @return : top-most particle's y
		 */
		long double getTop() const;

	private:
		/// The number of particles in this system.
		unsigned int mSize;
		/// A Nx3 matrix representing mass and position.
		Particle** mParticles;

		Particle* minXP;
		Particle* mfxXP;
		Particle* minYP;
		Particle* mfxYP;

		ParticleSystem( const ParticleSystem& right );
		ParticleSystem& operator=( const ParticleSystem& right );
};

#endif // PARTICLE_SYSTEM_HPP
