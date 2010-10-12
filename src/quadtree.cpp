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
using std::cout;

#include <algorithm>
using std::swap;

#include <limits>
using std::numeric_limits;

#include <cmath>

static const unsigned int NOT_A_QUADRANT = 5;
static const long double QUAD_LEEWAY = 8.0 * numeric_limits<long double>::epsilon();

Quadtree::Quadtree(long double iL, long double iR,
		long double iB, long double iT, Particle* iMe) :
	left( iL ), //{{{
	right( iR ),
	top( iT ),
	bottom( iB ),
	tau( 0.5 ),
	parent( false ),
	me( iMe ),
	mChildren( NULL )
{
	if( this->left > this->right )
		swap( this->left, this->right );
	if( this->bottom > this->top )
		swap( this->bottom, this->top );
} //}}}

Quadtree::Quadtree( ParticleSystem* ps ) :
	left( 0 ), //{{{
	right( 0 ),
	top( 0 ),
	bottom( 0 ),
	tau( 0.5 ),
	parent( false ),
	me( NULL ),
	mChildren( NULL )
{
	// Figure out the sides of the Quadtree {{{
	this->left = ps->getLeft() - QUAD_LEEWAY;
	this->right = ps->getRight() + QUAD_LEEWAY;
	this->bottom = ps->getBottom() - QUAD_LEEWAY;
	this->top = ps->getTop() + QUAD_LEEWAY;

	long double w = this->right - this->left;
	long double h = this->top - this->bottom;
	if( w > h )
	{
		this->bottom -= (w - h)/2.0;
		this->top += (w - h)/2.0;
	}
	else if( h > w )
	{
		this->left -= (h - w)/2.0;
		this->right += (h - w)/2.0;
	} //}}}

	this->add( ps );
} //}}}

Quadtree::~Quadtree()
{ //{{{
	this->clear();
} //}}}

void Quadtree::add(Particle* node)
{ //{{{
	if( node == NULL )
		return;

	if( this->getQuadrant( node ) == NOT_A_QUADRANT )
	{
		cerr << "Node does not fit here\n";
		cerr << this->left << " " << this->right << "\n"
			<< this->bottom << " " << this->top << "\n"
			<< (*node) << "\n";
		return;
	}

	if( this->me == NULL )
	{
		this->me = node;
		return;
	}

	if( !this->parent )
	{
		this->makeChildren();

		this->mChildren[ this->getQuadrant( node ) ]->add( node );
		this->mChildren[ this->getQuadrant( this->me ) ]->add( this->me );

		this->me = new Particle( 0, 0, 0 );
		this->recalculateMe();
		return;
	}

	this->mChildren[ this->getQuadrant( node ) ]->add( node );
	this->recalculateMe();
} //}}}

void Quadtree::add( ParticleSystem* ps )
{ //{{{
	if( ps == NULL )
		return;

	for( unsigned int i = 0; i < ps->getSize(); i++ )
		this->add( ps->getParticle( i ) );
} //}}}

void Quadtree::clear()
{ //{{{
	this->me = NULL;

	if( !this->parent )
		return;

	for( unsigned int i = 0; i < 4; ++i )
	{
		if( this->mChildren[ i ] == NULL )
			continue;
		delete this->mChildren[ i ];
		this->mChildren[ i ] = NULL;
	}
	this->parent = false;
} //}}}

void Quadtree::update( Particle* p ) const
{ //{{{
	if(( this->me == NULL ) || ( p == NULL ) || ( this->me == p ))
		return;

	long double dx = this->me->x - p->x;
	long double dy = this->me->y - p->y;
	long double d2 = dx * dx + dy * dy;
	long double d = sqrt( d2 );
	long double d3 = d * d2;

	if( !this->parent )
	{
		if( fabs( this->me->m ) < 2.0*numeric_limits<long double>::epsilon() )
			return;
		long double gm = p->m * this->me->m;
		p->fx += dx * gm / d3;
		p->fy += dy * gm / d3;
		return;
	}

	long double s = this->right - this->left;
	if(( fabs( this->me->m ) < 2.0*numeric_limits<long double>::epsilon() ) ||
		( (s / d) >= this->tau ) ||
		( this->getQuadrant( p ) != NOT_A_QUADRANT ))
	{
		this->mChildren[ 0 ]->update( p );
		this->mChildren[ 1 ]->update( p );
		this->mChildren[ 2 ]->update( p );
		this->mChildren[ 3 ]->update( p );
		return;
	}
	else
	{
		long double gm = p->m * this->me->m;
		p->fx += dx * gm / d3;
		p->fy += dy * gm / d3;
		return;
	}
} //}}}

void Quadtree::update( ParticleSystem* ps ) const
{ //{{{
	if( ps == NULL )
		return;

	for( unsigned int i = 0; i < ps->getSize(); i++ )
		this->update( ps->getParticle( i ) );
} //}}}

Particle* Quadtree::getMe()
{ //{{{
	return this->me;
} //}}}

void Quadtree::recalculateMe()
{ //{{{
	if( !this->parent )
		return;

	this->me->x = 0; this->me->y = 0; this->me->m = 0;
	for( unsigned int i = 0; i < 4; i++ )
	{
		if( this->mChildren[ i ]->getMe() != NULL )
			this->me->m += this->mChildren[ i ]->getMe()->m;
	}

	if( fabs( this->me->m ) < 2.0*numeric_limits<long double>::epsilon() )
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
		return NOT_A_QUADRANT;

	if(( node->x < this->left ) || ( node->x >= this->right ) ||
		( node->y < this->bottom ) || ( node->y >= this->top ))
		return NOT_A_QUADRANT;

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

	return NOT_A_QUADRANT;
} //}}}

void Quadtree::makeChildren()
{ //{{{
	if( this->me == NULL )
		return;

	if( this->parent )
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
	this->mChildren[ 0 ]->setTau( this->tau );
	this->mChildren[ 1 ] = new Quadtree(
			this->left, midX,
			midY, this->top, NULL );
	this->mChildren[ 1 ]->setTau( this->tau );
	this->mChildren[ 2 ] = new Quadtree(
			this->left, midX,
			this->bottom, midY, NULL );
	this->mChildren[ 2 ]->setTau( this->tau );
	this->mChildren[ 3 ] = new Quadtree(
			midX, this->right,
			this->bottom, midY, NULL );
	this->mChildren[ 3 ]->setTau( this->tau );

	this->parent = true;
} //}}}

long double Quadtree::getLeft() const
{ //{{{
	return this->left;
} //}}}

long double Quadtree::getRight() const
{ //{{{
	return this->right;
} //}}}

long double Quadtree::getTop() const
{ //{{{
	return this->top;
} //}}}

long double Quadtree::getBottom() const
{ //{{{
	return this->bottom;
} //}}}

long double Quadtree::getTau() const
{ //{{{
	return this->tau;
} //}}}

void Quadtree::setTau( long double nTau )
{ //{{{
	this->tau = nTau;
	if( !this->parent )
		return;
	for( unsigned int i = 0; i < 4; i++ )
		this->mChildren[ i ]->setTau( this->tau );
} //}}}

Quadtree* Quadtree::getChild( unsigned int indice )
{ //{{{
	return this->mChildren[ indice % 4 ];
} //}}}

bool Quadtree::isParent() const
{ //{{{
	return this->parent;
} //}}}

void Quadtree::printDimensions() const
{ //{{{
	cout << "\t[" << this->left << ", " << this->right << "] ["
		<< this->bottom << ", " << this->top << "]\n";
} //}}}

