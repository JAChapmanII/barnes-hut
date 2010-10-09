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
using std::cout;
using std::cerr;
using std::cin;

#include <string>
using std::string;

#include "particle_system.hpp"
#include "quadtree.hpp"

int main( int argc, char** argv )
{
	cout << "Arguments:\n";
	for( int i = 0; i < argc; i++ )
	{
		cout << "   " << i << ": " << argv[i] << '\n';
	}

	string fileName("");
	if( argc < 2 )
	{
		cout << "Please input the initial particle descriptor file name: ";
		cin >> fileName; cout << '\n';
	}
	else
	{
		fileName = (string)(argv[argc - 1]);
	}
	cout << "Selected file: " << fileName << "\n\n";

	ParticleSystem mPS( fileName );

	if( mPS.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return 0;
	}
	cout << mPS << '\n';

	Particle* cur;
	cur = mPS.getParticle( 0 );
	float minX = cur->x,
			maxX = cur->x,
			minY = cur->y,
			maxY = cur->y;
	for( unsigned int i = 1; i < mPS.getSize(); i++ )
	{
		cur = mPS.getParticle( i );
		if( cur->x < minX )
			minX = cur->x;
		if( cur->x > maxX )
			maxX = cur->x;
		if( cur->y < minY )
			minY = cur->y;
		if( cur->y > maxY )
			maxY = cur->y;
	}
	cout << "min/max X: " << minX << ", " << maxX << "\n";
	cout << "min/max Y: " << minY << ", " << maxY << "\n";

	cout << "Putting all particles into Quadtree, let's see if we SIGSEGV\n";
	Quadtree mQT( minX - 1.0, maxX + 1.0, maxY + 1.0, minY - 1.0, NULL );
	cout << "[" << mQT.getLeft() << ", " << mQT.getRight() << "] ["
		<< mQT.getBottom() << ", " << mQT.getTop() << "]\n";

	for( unsigned int i = 0; i < mPS.getSize(); i++ )
	{
		mQT.add( mPS.getParticle( i ) );
	}

	cout << "Yay! We didn't crash\n";

	return 0;
}

