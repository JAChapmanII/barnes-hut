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
	// Print args, get fileName and print it {{{
	cout << "Arguments:\n";
	for( int i = 0; i < argc; i++ )
	{
		cout << "   " << i << ": " << argv[i] << '\n';
	}

	string fileName("");
	string outputName("");
	if( argc < 2 )
	{
		cout << "Please input the initial particle descriptor file name: ";
		cin >> fileName; cout << '\n';
	}
	else
	{
		fileName = (string)(argv[argc - 1]);
	}
	outputName = fileName.substr( 0, fileName.find(".txt") ) + "_output.txt";
	cout << "Selected file: " << fileName << "(output to: " << outputName << ")\n\n";
	// }}}

	ParticleSystem mPS( fileName );

	if( mPS.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return 0;
	}
	cout << mPS << '\n';
	cout << "mPS: [" << mPS.getLeft() << ", " << mPS.getRight() << "] ["
		<< mPS.getBottom() << ", " << mPS.getTop() << "]\n";

	cout << "Putting all particles into Quadtree, let's see if we SIGSEGV\n";
	long double l = 0, r = 0, b = 0, t = 0;
	long double w = mPS.getRight() - mPS.getLeft();
	long double h = mPS.getTop() - mPS.getBottom();
	if( w > h )
	{
		l = mPS.getLeft() - 1.0;
		r = mPS.getRight() + 1.0;
		b = mPS.getBottom() - 1.0 - (w - h)/2.0;
		t = mPS.getTop() + 1.0 + (w - h)/2.0;
	}
	else
	{
		b = mPS.getBottom() - 1.0;
		t = mPS.getTop() + 1.0;
		l = mPS.getLeft() - 1.0 - (h - w)/2.0;
		r = mPS.getRight() + 1.0 + (h - w)/2.0;
	}
	Quadtree mQT( l, r, b, t, NULL );
	cout << "[" << mQT.getLeft() << ", " << mQT.getRight() << "] ["
		<< mQT.getBottom() << ", " << mQT.getTop() << "]\n";

	for( unsigned int i = 0; i < mPS.getSize(); i++ )
	{
		cout << i << "\n";
		mQT.add( mPS.getParticle( i ) );
	}
	cout << "Yay! We didn't crash\n";

	cout << "Runnnig Barnes-Hut on all particles\n";
	for( unsigned int i = 0; i < mPS.getSize(); i++ )
	{
		mQT.update( mPS.getParticle( i ) );
	}

	cout << "Outputting results\n";
	cout << mPS << '\n';

	cout << "Saving results\n";
	mPS.save( outputName );

	cout << "Quiting\n";

	return 0;
}

