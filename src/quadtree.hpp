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

#include "particle.cpp"

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
		 * @param iT : top coordinate
		 * @param iB : bottom coordinate
		 * @param iMe : initial point to contain
		 * @note : iMe should only be NULL for empty nodes
		 */
		Quadtree(float iL, float iR, float iT, float iB, Particle* iMe);

		/**
		 * Add a node to this tree.
		 * @param node : node to be added
		 */
		void add(Particle* node);
		/**
		 * Add an entire Quadtree to this one.
		 * @param tree : tree to be added
		 */
		void add(Quadtree* tree);

		/**
		 * Delete all contents of this.
		 */
		void clear();

		/**
		 * Return the point this Quadtree represents.
		 * @return : point representing average of all nodes in this
		 */
		Particle* getMe();

		/**
		 * Return the quadrant a node shoud be point into in this tree.
		 * @param node : node to find quadrant of
		 * @return : quadrant where node should go
		 */
		unsigned int getQuadrant( Particle* node ) const;

	private:
		/**
		 * Allocates space for and creates children Quadtrees
		 */
		void makeChildren();

		float left, right;
		float top, bottom;
		unsigned int numChildren;
		Particle* me;
		Quadtree** mChildren;
};

#endif // QUADTREE_HPP
