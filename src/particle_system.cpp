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

#include "particle_system.hpp"
using std::string;
using std::ostream;

#include <iostream>
using std::cerr;

#include <iomanip>
using std::fixed;
using std::setprecision;
using std::setw;

#include <fstream>
using std::ifstream;
using std::ofstream;

ParticleSystem::ParticleSystem() :
	mSize( 0 ), //{{{
	mParticles( NULL ),
	minXP( NULL ),
	mfxXP( NULL ),
	minYP( NULL ),
	mfxYP( NULL )
{
} //}}}

ParticleSystem::ParticleSystem( string fileName, bool hasForces ) :
	mSize( 0 ), //{{{
	mParticles( NULL ),
	minXP( NULL ),
	mfxXP( NULL ),
	minYP( NULL ),
	mfxYP( NULL )
{
	this->load( fileName, hasForces );
} //}}}

ParticleSystem::ParticleSystem( const ParticleSystem& rhs ) :
	mSize( rhs.mSize ), //{{{
	mParticles( NULL ),
	minXP( NULL ),
	mfxXP( NULL ),
	minYP( NULL ),
	mfxYP( NULL )
{
	this->mParticles = new Particle*[ this->mSize ];
	for( unsigned int i = 0; i < this->mSize; i++ )
	{
		this->mParticles[ i ] = new Particle(
			rhs.mParticles[ i ]->x, rhs.mParticles[ i ]->y,
			rhs.mParticles[ i ]->m );

		if((this->minXP == NULL) || ( this->mParticles[ i ]->x < this->minXP->x ))
			this->minXP = this->mParticles[ i ];
		if((this->mfxXP == NULL) || ( this->mParticles[ i ]->x > this->mfxXP->x ))
			this->mfxXP = this->mParticles[ i ];
		if((this->minYP == NULL) || ( this->mParticles[ i ]->y < this->minYP->y ))
			this->minYP = this->mParticles[ i ];
		if((this->mfxYP == NULL) || ( this->mParticles[ i ]->y > this->mfxYP->y ))
			this->mfxYP = this->mParticles[ i ];
	}
} //}}}

ParticleSystem::~ParticleSystem()
{ //{{{
	this->clear();
} //}}}

void ParticleSystem::load( string fileName, bool hasForces )
{ //{{{
	ifstream file( fileName.c_str() );
	if( !file.good() )
	{
		cerr << "ParticleSystem was passed bad file\n";
		return;
	}

	string line;
	while( !file.eof() )
	{
		getline( file, line );
		this->mSize++;
	}
	this->mSize--;

	file.close();
	file.open( fileName.c_str() );
	this->mParticles = new Particle*[ this->mSize ];
	for( unsigned int i = 0; i < this->mSize; i++ )
	{
		this->mParticles[ i ] = new Particle();
	}

	for( unsigned int l = 0; l < this->mSize; l++ )
	{
		file >> this->mParticles[ l ]->x;
		file >> this->mParticles[ l ]->y;
		file >> this->mParticles[ l ]->m;
		if( hasForces )
		{
			file >> this->mParticles[ l ]->fx;
			file >> this->mParticles[ l ]->fy;
		}

		if( !file.good() )
		{
			cerr << "Error loading on " << fileName << " on line " << l + 1 << "\n";
			this->clear();
			return;
		}

		if((this->minXP == NULL) || ( this->mParticles[ l ]->x < this->minXP->x ))
			this->minXP = this->mParticles[ l ];
		if((this->mfxXP == NULL) || ( this->mParticles[ l ]->x > this->mfxXP->x ))
			this->mfxXP = this->mParticles[ l ];
		if((this->minYP == NULL) || ( this->mParticles[ l ]->y < this->minYP->y ))
			this->minYP = this->mParticles[ l ];
		if((this->mfxYP == NULL) || ( this->mParticles[ l ]->y > this->mfxYP->y ))
			this->mfxYP = this->mParticles[ l ];
	}

	file.close();
} //}}}

void ParticleSystem::save( string fileName )
{ //{{{
	ofstream file( fileName.c_str() );
	if( !file.good() )
	{
		cerr << "Could not save to file\n";
		return;
	}

	for( unsigned int i = 0; i < this->mSize; ++i )
	{
		file
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->mParticles[ i ]->x << "\t"
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->mParticles[ i ]->y << "\t"
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->mParticles[ i ]->m << "\t"
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->mParticles[ i ]->fx << "\t"
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->mParticles[ i ]->fy << "\n";
	}

	file.close();
} //}}}

void ParticleSystem::clear()
{ //{{{
	for( unsigned int i = 0; i < this->mSize; i++ )
	{
		delete[] this->mParticles[ i ];
	}
	delete[] this->mParticles;
	this->mParticles = NULL;
	this->minXP = NULL;
	this->mfxXP = NULL;
	this->minYP = NULL;
	this->mfxYP = NULL;
	this->mSize = 0;
} //}}}

unsigned int ParticleSystem::getSize() const
{ //{{{
	return this->mSize;
} //}}}

Particle* ParticleSystem::getParticle( unsigned int indice ) const
{ //{{{
	if( indice >= this->mSize )
		return NULL;

	return this->mParticles[ indice ];
} //}}}

ostream& operator<<( ostream& out, const ParticleSystem& toPrint )
{ //{{{
	out << "<-- ParticleSystem -->\n";
	out << "this->mSize = " << toPrint.getSize() << "\n";
	for( unsigned int i = 0; i < toPrint.getSize(); i++ )
	{
		Particle* p = toPrint.getParticle( i );
		if( p != NULL )
			out << *p << "\n";
	}
	out << "<-- ParticleSystem -->\n";
} //}}}

long double ParticleSystem::getLeft() const
{ //{{{
	return this->minXP->x;
} //}}}

long double ParticleSystem::getRight() const
{ //{{{
	return this->mfxXP->x;
} //}}}

long double ParticleSystem::getBottom() const
{ //{{{
	return this->minYP->y;
} //}}}

long double ParticleSystem::getTop() const
{ //{{{
	return this->mfxYP->y;
} //}}}

