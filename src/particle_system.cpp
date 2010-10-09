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
	maxXP( NULL ),
	minYP( NULL ),
	maxYP( NULL )
{
} //}}}

ParticleSystem::ParticleSystem( string fileName ) :
	mSize( 0 ), //{{{
	mParticles( NULL ),
	minXP( NULL ),
	maxXP( NULL ),
	minYP( NULL ),
	maxYP( NULL )
{
	this->load( fileName );
} //}}}

ParticleSystem::~ParticleSystem()
{ //{{{
	this->clear();
} //}}}

void ParticleSystem::load( string fileName )
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

		if( !file.good() )
		{
			cerr << "Error loading on " << fileName << " on line " << l + 1 << "\n";
			this->clear();
			return;
		}

		if((this->minXP == NULL) || ( this->mParticles[ l ]->x < this->minXP->x ))
			this->minXP = this->mParticles[ l ];
		if((this->maxXP == NULL) || ( this->mParticles[ l ]->x > this->maxXP->x ))
			this->maxXP = this->mParticles[ l ];
		if((this->minYP == NULL) || ( this->mParticles[ l ]->y < this->minYP->y ))
			this->minYP = this->mParticles[ l ];
		if((this->maxYP == NULL) || ( this->mParticles[ l ]->y > this->maxYP->y ))
			this->maxYP = this->mParticles[ l ];
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

	/*
	file.width( 6 );
	file.precision( 4 );
	file.setf( std::ios::right, std::ios::floatfield );
	file.setf( std::ios::fixed, std::ios::floatfield );
	file.setf( std::ios::showpoint, std::ios::floatfield );
	file.fill( ' ' )
	*/

	for( unsigned int i = 0; i < this->mSize; ++i )
	{
		file
			<< fixed << setprecision( 4 ) << setw( 7 ) << this->mParticles[ i ]->x << "\t"
			<< fixed << setprecision( 4 ) << setw( 7 ) << this->mParticles[ i ]->y << "\t"
			<< fixed << setprecision( 4 ) << setw( 7 ) << this->mParticles[ i ]->m << "\t"
			<< fixed << setprecision( 4 ) << setw( 7 ) << this->mParticles[ i ]->ax << "\t"
			<< fixed << setprecision( 4 ) << setw( 7 ) << this->mParticles[ i ]->ay << "\n";
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
	this->maxXP = NULL;
	this->minYP = NULL;
	this->maxYP = NULL;
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

float ParticleSystem::getLeft() const
{ //{{{
	return this->minXP->x;
} //}}}

float ParticleSystem::getRight() const
{ //{{{
	return this->maxXP->x;
} //}}}

float ParticleSystem::getBottom() const
{ //{{{
	return this->minYP->y;
} //}}}

float ParticleSystem::getTop() const
{ //{{{
	return this->maxYP->y;
} //}}}

