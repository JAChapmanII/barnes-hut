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

#include <cmath>

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
		m = this->me->m + node->m;
		x = (this->me->x * this->me->m + node->x * node->m)/m;
		y = (this->me->y * this->me->m + node->y * node->m)/m;
		this->me = new Particle( x, y, m );
		this->numChildren = 2;
		return;
	}

	this->mChildren[ this->getQuadrant( node ) ]->add( node );
	this->me->x = (this->me->x * this->me->m + node->x * node->m)
		/ (this->me->m + node->m);
	this->me->y = (this->me->y * this->me->m + node->y * node->m)
		/ (this->me->m + node->m);
	this->me->m += node->m;
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

void Quadtree::update( Particle* p )
{ //{{{
	if(( this->me == NULL ) || ( p == NULL ))
		return;

	// G = 6.6726 x 10-11N-m2/kg2
	static const float GRAVITY = 0.000000000066726;

	float dx = this->me->x - p->x;
	float dy = this->me->y - p->y;
	float d2 = dx * dx + dy * dy;

	if( this->numChildren == 0 )
	{
		float gm = GRAVITY * p->m * this->me->m;
		p->ax += dx * gm / d2;
		p->ay += dy * gm / d2;
		return;
	}

	static const float TAU = 0.5;
	float d = sqrt( d2 );
	if( ((this->right - this->left) / d) < TAU )
	{
		this->mChildren[ 0 ]->update( p );
		this->mChildren[ 1 ]->update( p );
		this->mChildren[ 2 ]->update( p );
		this->mChildren[ 3 ]->update( p );
		return;
	}
	else
	{
		float gm = GRAVITY * p->m * this->me->m;
		p->ax += dx * gm / d2;
		p->ay += dy * gm / d2;
		return;
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

