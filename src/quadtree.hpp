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
#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "particle_system.hpp"

/**
 * Class representing a recursive space division into four quadrants.
 */
class Quadtree
{
	public:
		/**
		 * Create a quadtree representing a certain amount of space.
		 * @param iL : left hand coordinate
		 * @param iR : right hand coordinate
		 * @param iB : top coordinate
		 * @param iT : bottom coordinate
		 * @param iMe : initial point to contain
		 * @note : iMe should only be NULL for empty nodes
		 */
		Quadtree( long double iL, long double iR,
				long double iB, long double iT, Particle* iMe );

		/**
		 * Create a quadtree based on a particle system.
		 * @param ps : ParticleSystem to base this off of
		 */
		Quadtree( ParticleSystem* ps );

		/**
		 * Proper deconstructor that delets all associated memory.
		 */
		~Quadtree();

		/**
		 * Add a node to this tree.
		 * @param node : node to be added
		 */
		void add( Particle* node );

		/**
		 * Add a particle system to this tree.
		 * @param ps : ParticleSystem which should be added
		 */
		void add( ParticleSystem* ps );

		/**
		 * Delete all contents of this.
		 */
		void clear();

		/**
		 * Updates a particle's acceleration by using the Barnes-Hut algorithm.
		 * @param p : Particle to update
		 */
		void update( Particle* p ) const;

		/**
		 * Updates an entire particle system's particles with new forces.
		 * @param ps : ParticleSystem to update
		 */
		void update( ParticleSystem* ps ) const;

		/**
		 * Return the point this Quadtree represents.
		 * @return : point representing average of all nodes in this
		 */
		Particle* getMe();

		/**
		 * Recursively re-calculate a new me
		 */
		void recalculateMe();

		/**
		 * Return the quadrant a node shoud be point into in this tree.
		 * @param node : node to find quadrant of
		 * @return : quadrant where node should go
		 */
		unsigned int getQuadrant( Particle* node ) const;

		/**
		 * Returns left side.
		 * @return : left side
		 */
		long double getLeft() const;

		/**
		 * Returns right side.
		 * @return : right side
		 */
		long double getRight() const;

		/**
		 * Returns top side.
		 * @return : top side
		 */
		long double getTop() const;

		/**
		 * Returns bottom side.
		 * @return : bottom side
		 */
		long double getBottom() const;

		/**
		 * Returns the tau of this quadtree.
		 * @return : this's tau
		 */
		long double getTau() const;

		/**
		 * Sets this tau of this quadtree.
		 * @param nTau : new value for tau
		 */
		void setTau( long double nTau );

		/**
		 * Returns a pointer to a child.
		 * @param indice : which child to return
		 * @return : pointer to that child
		 */
		Quadtree* getChild( unsigned int indice );

		/**
		 * Returns true if this has children.
		 * @return : true if this has children
		 */
		bool isParent() const;

		/**
		 * Prints the dimensions of this to cout.
		 */
		void printDimensions() const;

	private:
		/**
		 * Allocates space for and creates children Quadtrees
		 */
		void makeChildren();

		long double left, right;
		long double top, bottom;
		long double tau;
		bool parent;
		Particle* me;
		Quadtree** mChildren;

		Quadtree( const Quadtree& rhs );
		Quadtree &operator=( const Quadtree& rhs );
};

#endif // QUADTREE_HPP
