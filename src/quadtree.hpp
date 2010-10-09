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

class Quadtree
{
	public:
		class Node
		{ //{{{
			public:
				float x, y, m;
				Node() :
					x(0),
					y(0),
					m(0)
				{
				}

				Node( float iX, float iY, float iM ) :
					x( iX ),
					y( iY ),
					m( iM )
				{
				}
		}; //}}}

		Quadtree(float iL, float iR, float iT, float iB, Node* iMe);

		void add(Node* node);
		void add(Quadtree* tree);

		void clear();

		Node* getMe();

		unsigned int getQuadrant( Node* node ) const;

	private:
		void makeChildren();

		float left, right;
		float top, bottom;
		unsigned int numChildren;
		Node* me;
		Quadtree** mChildren;
};

#endif // QUADTREE_HPP
