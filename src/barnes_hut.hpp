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
#ifndef BARNES_HUT_HPP
#define BARNES_HUT_HPP

#include <QtCore/QThread>

#include "particle_system.hpp"
#include "quadtree.hpp"

class BarnesHut : public QThread
{
	public:
		/**
		 * Construct an object that multithreads the Barnes-Hut algorithm.
		 * @param iPS : particle system to act upon
		 * @param iQT : quadtree to use as refenerce
		 */
		BarnesHut( ParticleSystem* iPS = NULL, Quadtree* iQT = NULL );

		/**
		 * Run the Barnes-Hut algorithm on this.
		 */
		void run();

		/**
		 * Get the particle system associated with this3
		 */
		ParticleSystem* getParticleSystem();

		/**
		 * Get the quad tree associated wth this.
		 */
		Quadtree* getQuadTree();

		/**
		 * Return the number of threads this should use.
		 */
		unsigned int getNumberOfThreads() const;

		/**
		 * Return the indice of the first particle to act upon.
		 */
		unsigned int getFirst() const;

		/**
		 * Return the indice of the last particle to act upon.
		 */
		unsigned int getLast() const;

		/**
		 * Associate a new particle system with this.
		 * @param nPS : new particle system
		 */
		void setParticleSystem( ParticleSystem* nPS );

		/**
		 * Associate a new quad tree with this.
		 * @param nQT : new quad tree
		 */
		void setQuadTree( Quadtree* nQT );

		/**
		 * Set the number of threads this should use.
		 * @param num : number of threads
		 */
		void setNumberOfThreads( unsigned int num );

		/**
		 * Set the indice of the first particle to be acted upon.
		 * @param nFirst : new first indice
		 */
		void setFirst( unsigned int nFirst );

		/**
		 * Set the indice of the last particle to be acted upon.
		 * @param nLast : new last indice
		 */
		void setLast( unsigned int nLast );

	private:
		ParticleSystem* ps;
		Quadtree* qt;
		unsigned int numThreads;
		unsigned int first;
		unsigned int last;

		BarnesHut( const BarnesHut& rhs );
		BarnesHut& operator=( const BarnesHut& rhs );
};

#endif // BARNES_HUT_HPP
