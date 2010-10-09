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

Quadtree::Quadtree(long double iL, long double iR, long double iT, long double iB, Particle* iMe) :
	left( iL ), //{{{
	right( iR ),
	top( iT ),
	bottom( iB ),
	isParent( false ),
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

	if( !this->isParent )
	{
		this->makeChildren();

		this->mChildren[ this->getQuadrant( node ) ]->add( node );
		this->mChildren[ this->getQuadrant( node ) ]->recalculateMe();
		this->mChildren[ this->getQuadrant( this->me ) ]->add( this->me );
		this->mChildren[ this->getQuadrant( this->me ) ]->recalculateMe();

		this->me = new Particle( 0, 0, 0 );
		this->recalculateMe();
		this->isParent = true;
		return;
	}

	this->mChildren[ this->getQuadrant( node ) ]->add( node );
	this->mChildren[ this->getQuadrant( node ) ]->recalculateMe();
	this->recalculateMe();
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

	if( !this->isParent )
		return;
	for( unsigned int i = 0; i < 4; ++i )
	{
		delete this->mChildren[ i ];
		this->mChildren[ i ] = NULL;
	}
} //}}}

void Quadtree::update( Particle* p )
{ //{{{
	if(( this->me == NULL ) || ( p == NULL ) || ( this->me == p ))
		return;

	// G = 6.6726 x 10-11N-m2/kg2
	//static const long double GRAVITY = 0.000000000066726;
	static const long double GRAVITY = 1;
	//static const long double TAU = 0.5;
	static const long double TAU = 0.0;

	long double dx = this->me->x - p->x;
	long double dy = this->me->y - p->y;
	long double d2 = dx * dx + dy * dy;
	long double d = sqrt( d2 );
	long double d3 = d * d2;

	if( !this->isParent )
	{
		if( this->me->m == 0 )
			return;
		long double gm = GRAVITY * p->m * this->me->m;
		p->ax += dx * gm / d3;
		p->ay += dy * gm / d3;
		return;
	}

	if(( this->me->m == 0 ) || ( ((this->right - this->left) / d) >= TAU ) ||
		( this->getQuadrant( p ) != -1 ))
	{
		this->mChildren[ 0 ]->update( p );
		this->mChildren[ 1 ]->update( p );
		this->mChildren[ 2 ]->update( p );
		this->mChildren[ 3 ]->update( p );
		return;
	}
	else
	{
		long double gm = GRAVITY * p->m * this->me->m;
		p->ax += dx * gm / d3;
		p->ay += dy * gm / d3;
		return;
	}
} //}}}

Particle* Quadtree::getMe()
{ //{{{
	return this->me;
} //}}}

void Quadtree::recalculateMe()
{ //{{{
	if( !this->isParent )
		return;

	this->me->x = 0; this->me->y = 0; this->me->m = 0;
	for( unsigned int i = 0; i < 4; i++ )
	{
		if( this->mChildren[ i ]->getMe() != NULL )
			this->me->m += this->mChildren[ i ]->getMe()->m;
	}

	if( this->me->m == 0 )
		return;

	Particle* tChild = NULL;
	for( unsigned int i = 0; i < 4; i++ )
	{
		tChild = this->mChildren[ i ]->getMe();
		if( tChild != NULL )
		{
			this->me->x += tChild->x * tChild->m;
			this->me->y += tChild->y * tChild->m;
		}
	}
	this->me->x /= this->me->m;
	this->me->y /= this->me->m;
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

	if( this->isParent )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			delete this->mChildren[ i ];
			this->mChildren[ i ] = NULL;
		}
	}

	this->mChildren = new Quadtree*[ 4 ];
	long double midX = (this->left + this->right)/2.0;
	long double midY = (this->bottom + this->top)/2.0;
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

long double Quadtree::getLeft()
{ //{{{
	return this->left;
} //}}}

long double Quadtree::getRight()
{ //{{{
	return this->right;
} //}}}

long double Quadtree::getTop()
{ //{{{
	return this->top;
} //}}}

long double Quadtree::getBottom()
{ //{{{
	return this->bottom;
} //}}}

