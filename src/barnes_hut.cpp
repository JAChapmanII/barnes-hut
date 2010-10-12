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

#include <iostream>
using std::cerr;

#include "barnes_hut.hpp"

BarnesHut::BarnesHut( ParticleSystem* iPS, Quadtree* iQT ) :
	ps( iPS ), //{{{
	qt( iQT ),
	numThreads( 4 ),
	first( 0 ),
	last( 0 )
{
} //}}}

void BarnesHut::run()
{ //{{{
	if( first >= last )
		return;

	if(( this->ps == NULL ) || ( this->qt == NULL ))
	{
		cerr << "Tried to run Barnes-Hut with a null ps or qt\n";
		return;
	}

	if( this->numThreads == 0 )
	{
		for( unsigned int i = first; (i < this->last) && (i < this->ps->getSize());
				i++ )
		{
			this->qt->update( this->ps->getParticle( i ) );
		}

		return;
	}

	if( this->numThreads > (this->last - this->first) )
	{
		BarnesHut** workers = new BarnesHut*[ this->last - this->first ];
		for( unsigned int i = 0; i < (this->last - this->first); i++ )
		{
			workers[ i ] = new BarnesHut( this->ps, this->qt );
			workers[ i ]->setFirst( i + this->first );
			workers[ i ]->setLast( i + this->first + 1 );
			workers[ i ]->setNumberOfThreads( 0 );
			workers[ i ]->start();
		}

		for( unsigned int i = 0; i < (this->last - this->first); i++ )
		{
			workers[ i ]->wait();
			delete workers[ i ];
		}
		delete workers;

		return;
	}

	BarnesHut** workers = new BarnesHut*[ this->numThreads ];

	unsigned int ppt = (this->last - this->first) / this->numThreads;
	for( unsigned int i = 0; i < this->numThreads; i++ )
	{
		workers[ i ] = new BarnesHut( this->ps, this->qt );
		workers[ i ]->setFirst( this->first + i * ppt );
		workers[ i ]->setLast( this->first + (i + 1) * ppt );
		if( i == this->numThreads - 1 )
			workers[ i ]->setLast( this->last );
		workers[ i ]->setNumberOfThreads( 0 );
		workers[ i ]->start();
		workers[ i ]->wait( 10 );
	}

	for( unsigned int i = 0; i< this->numThreads; i++ )
	{
		workers[ i ]->wait();
		delete workers[ i ];
	}
	delete workers;

} //}}}

ParticleSystem* BarnesHut::getParticleSystem()
{ //{{{
	return this->ps;
} //}}}

Quadtree* BarnesHut::getQuadTree()
{ //{{{
	return this->qt;
} //}}}

unsigned int BarnesHut::getNumberOfThreads() const
{ //{{{
	return this->numThreads;
} //}}}

unsigned int BarnesHut::getFirst() const
{ //{{{
	return this->first;
} //}}}

unsigned int BarnesHut::getLast() const
{ //{{{
	return this->last;
} //}}}

void BarnesHut::setParticleSystem( ParticleSystem* nPS )
{ //{{{
	this->ps = nPS;
} //}}}

void BarnesHut::setQuadTree( Quadtree* nQT )
{ //{{{
	this->qt = nQT;
} //}}}

void BarnesHut::setNumberOfThreads( unsigned int num )
{ //{{{
	this->numThreads = num;
} //}}}

void BarnesHut::setFirst( unsigned int nFirst )
{ //{{{
	this->first = nFirst;
} //}}}

void BarnesHut::setLast( unsigned int nLast )
{ //{{{
	this->last = nLast;
} //}}}

