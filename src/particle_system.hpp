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
 * and into memory, providing array like access.
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
		 * @param hasForces : true if there are forces in the file
		 */
		ParticleSystem( std::string fileName, bool hasForces = false );

		/**
		 * Makes a duplicate of all particles in another system and puts them in
		 * this system upon creation.
		 * @param rhs : ParticleSystem to copy into this one
		 */
		ParticleSystem( const ParticleSystem& rhs );

		/**
		 * Deconstructor that deallocates all memory used by this.
		 */
		~ParticleSystem();

		/**
		 * Copy another particle system into this one, deep.
		 * @param rhs : other particle system
		 */
		ParticleSystem& operator=( const ParticleSystem& rhs );

		/**
		 * Load a file into this system, trashing existing particles.
		 * @param fileName : filename to load from
		 * @param hasForces : true if there are forces in the file
		 */
		void load( std::string fileName, bool hasForces = false );

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
		 * Zeroes the forces of all particles.
		 */
		void zeroForces();

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

		/**
		 * Prints the dimensions of this to cout.
		 */
		void printDimensions() const;

	private:
		/// The number of particles in this system.
		unsigned int mSize;
		/// A Nx3 matrix representing mass and position.
		Particle** mParticles;

		Particle* minXP;
		Particle* maxXP;
		Particle* minYP;
		Particle* maxYP;
};

#endif // PARTICLE_SYSTEM_HPP
