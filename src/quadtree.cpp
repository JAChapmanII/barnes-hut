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

#include "quadtree.hpp"

#include <iostream>
using std::cerr;

#include <algorithm>
using std::swap;

Quadtree::Quadtree(float iL, float iR, float iT, float iB, Particle* iMe) :
	left( iL ), //{{{
	right( iR ),
	top( iT ),
	bottom( iB ),
	numChildren( 0 ),
	me( iMe ),
	mChildren( NULL )
{
	if( this->left > this->right )
		swap( this->left, this->right );
	if( this->bottom > this->top )
		swap( this->bottom, this->top );
} //}}}

void Quadtree::add(Particle* node)
{ //{{{
	if( node == NULL )
		return;

	if( this->getQuadrant( node ) == -1 )
	{
		cerr << "Node does not fit here\n";
		return;
	}

	if( this->me == NULL )
	{
		this->me = node;
		return;
	}

	if( this->numChildren == 0 )
	{
		this->makeChildren();

		this->mChildren[ this->getQuadrant( node ) ]->add( node );
		this->mChildren[ this->getQuadrant( this->me ) ]->add( this->me );

		float x, y, m;
		x = (this->me->x + node->x)/2.0;
		y = (this->me->y + node->y)/2.0;
		m = (this->me->m + node->m)/2.0;
		this->me = new Particle( x, y, m );
		return;
	}

	this->mChildren[ this->getQuadrant( node ) ]->add( node );
	this->me->x = (this->me->x * (float)this->numChildren + node->x)
		/ ((float)this->numChildren + 1.0);
	this->me->y = (this->me->y * (float)this->numChildren + node->y)
		/ ((float)this->numChildren + 1.0);
	this->me->m = (this->me->m * (float)this->numChildren + node->m)
		/ ((float)this->numChildren + 1.0);
	this->numChildren++;
} //}}}

void Quadtree::add(Quadtree* tree)
{ //{{{
} //}}}

void Quadtree::clear()
{ //{{{
	if( this->me == NULL )
		return;
	delete this->me;
	this->me = NULL;

	if( this->numChildren == 0 )
		return;
	for( unsigned int i = 0; i < 4; ++i )
	{
		delete this->mChildren[ i ];
		this->mChildren[ i ] = NULL;
	}
} //}}}

Particle* Quadtree::getMe()
{ //{{{
	return this->me;
} //}}}

unsigned int Quadtree::getQuadrant( Particle* node ) const
{ //{{{
	if( node == NULL )
		return -1;

	if(( node->x < this->left ) || ( node->x >= this->right ) ||
		( node->y < this->bottom ) || ( node->y >= this->top ))
		return -1;

	if( node->x < (this->left + this->right)/2.0 )
	{
		if( node->y < (this->top + this->bottom)/2.0 )
			return 2;
		return 1;
	}
	else
	{
		if( node->y < (this->top + this->bottom)/2.0 )
			return 3;
		return 0;
	}

	return -1;
} //}}}

void Quadtree::makeChildren()
{ //{{{
	if( this->me == NULL )
		return;

	if( this->numChildren != 0 )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			delete this->mChildren[ i ];
			this->mChildren[ i ] = NULL;
		}
	}

	this->mChildren = new Quadtree*[ 4 ];
	float midX = (this->left + this->right)/2.0;
	float midY = (this->bottom + this->top)/2.0;
	this->mChildren[ 0 ] = new Quadtree(
			midX, this->right,
			midY, this->top, NULL );
	this->mChildren[ 1 ] = new Quadtree(
			this->left, midX,
			midY, this->top, NULL );
	this->mChildren[ 2 ] = new Quadtree(
			this->left, midX,
			this->bottom, midY, NULL );
	this->mChildren[ 3 ] = new Quadtree(
			midX, this->right,
			this->bottom, midY, NULL );
} //}}}

float Quadtree::getLeft()
{ //{{{
	return this->left;
} //}}}

float Quadtree::getRight()
{ //{{{
	return this->right;
} //}}}

float Quadtree::getTop()
{ //{{{
	return this->top;
} //}}}

float Quadtree::getBottom()
{ //{{{
	return this->bottom;
} //}}}

