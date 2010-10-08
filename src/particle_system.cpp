/** {{{
 * Copyright 2010 Jeff Chapman.
 *
 * This file is a part of aos
 *
 * aos is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aos.  If not, see <http://www.gnu.org/licenses/>.
 *
 */// }}}

#include "particle_system.hpp"
using std::string;
using std::ostream;

#include <iostream>
using std::cerr;

#include <fstream>
using std::ifstream;

ParticleSystem::ParticleSystem() :
	mSize( 0 ), //{{{
	mParticles( NULL )
{
} //}}}

ParticleSystem::ParticleSystem( string fileName ) :
	mSize( 0 ), //{{{
	mParticles( NULL )
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
		std::cerr << "ParticleSystem was passed bad file\n";
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
	this->mParticles = new float*[ this->mSize ];
	for( unsigned int i = 0; i < this->mSize; i++ )
	{
		this->mParticles[ i ] = new float[ 3 ];
	}

	for( unsigned int l = 0; l < this->mSize; l++ )
	{
		file >> this->mParticles[ l ][ 0 ];
		file >> this->mParticles[ l ][ 1 ];
		file >> this->mParticles[ l ][ 2 ];

		if( !file.good() )
		{
			cerr << "Error loading on " << fileName << " on line " << l + 1 << "\n";
			this->clear();
			return;
		}
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
	this->mSize = 0;
} //}}}

unsigned int ParticleSystem::getSize() const
{ //{{{
	return this->mSize;
} //}}}

float* ParticleSystem::getParticle( unsigned int indice ) const
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
		float* p = toPrint.getParticle( i );
		if( p != NULL )
			out << "< " << p[ 0 ] << ", " << p[ 1 ] << " > : " << p[ 2 ] << "\n";
	}
	out << "<-- ParticleSystem -->\n";
} //}}}

