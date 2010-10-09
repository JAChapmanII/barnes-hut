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

Quadtree::Quadtree(float iL, float iR, float iT, float iB, Particle* iMe) :
	left( iL ), //{{{
	right( iR ),
	top( iT ),
	bottom( iB ),
	numChildren( 0 ),
	me( iMe ),
	mChildren( NULL )
{
} //}}}

void Quadtree::add(Particle* node)
{ //{{{
	if( node == NULL )
		return;

	if(( node->x < this->left ) || ( node->x >= this->right ) ||
		( node->y >= this->top ) || ( node->y < this->bottom ))
	{
		cerr << "Particle does not fit into this Quadtree\n";
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
		( node->y >= this->top ) || ( node->y < this->bottom ))
		return -1;

	unsigned int quad = 0;
	if( node->x >= (this->left + this->right)/2.0 )
		quad += 2;

	if( node->y < (this->top + this->bottom)/2.0 )
		quad += 1;

	return quad;
} //}}}

void Quadtree::makeChildren()
{ //{{{
	if( this->me == NULL )
		return;

	this->clear();

	this->mChildren = new Quadtree*[ 4 ];
	this->mChildren[ 0 ] = new Quadtree(
			(this->left + this->right)/2.0, this->right,
			this->top, (this->top + this->bottom)/2.0, NULL );
	this->mChildren[ 1 ] = new Quadtree(
			this->left, (this->left + this->right)/2.0f,
			this->top, (this->top + this->bottom)/2.0, NULL );
	this->mChildren[ 2 ] = new Quadtree(
			this->left, (this->left + this->right)/2.0f,
			(this->top + this->bottom)/2.0, this->bottom, NULL );
	this->mChildren[ 3 ] = new Quadtree(
			(this->left + this->right)/2.0, this->right,
			(this->top + this->bottom)/2.0, this->bottom, NULL );
} //}}}

