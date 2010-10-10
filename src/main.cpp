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

#include <sstream>
using std::stringstream;

#include "particle_system.hpp"
#include "quadtree.hpp"

static const long double QUAD_LEEWAY = 0.00000000000001;

int main( int argc, char** argv )
{
	// Print args {{{
	cout << "Arguments:\n";
	for( int i = 0; i < argc; i++ )
	{
		cout << "   " << i << ": " << argv[i] << '\n';
	}
	//}}}

	// Determine input/output file names {{{
	string fileName("");
	string outputName("");
	if( argc < 2 )
	{
		cout << "Please input the initial particle descriptor file name: ";
		cin >> fileName; cout << '\n';
	}
	else
	{
		fileName = (string)(argv[ 1 ]);
	}
	outputName = fileName.substr( 0, fileName.find(".txt") ) + "_output.txt";
	cout << "Selected file: " << fileName
		<< " (output to: " << outputName << ")\n";
	// }}}

	// Determin tau {{{
	long double tau = 0.5;
	if( argc > 2 )
	{
		stringstream tmp( argv[ 2 ] );
		tmp >> tau;
	}
	cout << "Tau is: " << tau << "\n";
	//}}}

	ParticleSystem mPS( fileName );
	if( mPS.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return 0;
	}
	cout << "mPS: [" << mPS.getLeft() << ", " << mPS.getRight() << "] ["
		<< mPS.getBottom() << ", " << mPS.getTop() << "]\n";

	cout << "Putting all particles into Quadtree, let's see if we SIGSEGV\n";
	// Figure out the sides of the Quadtree {{{
	long double l = mPS.getLeft() - QUAD_LEEWAY,
		r = mPS.getRight() + QUAD_LEEWAY,
		b = mPS.getBottom() - QUAD_LEEWAY,
		t = mPS.getTop() + QUAD_LEEWAY;
	long double w = mPS.getRight() - mPS.getLeft();
	long double h = mPS.getTop() - mPS.getBottom();
	if( w > h )
	{
		b -= (w - h)/2.0;
		t += (w - h)/2.0;
	}
	else if( h > w )
	{
		l -= (h - w)/2.0;
		r += (h - w)/2.0;
	}
	//}}}
	Quadtree mQT( l, r, b, t, NULL );
	mQT.setTau( tau );
	cout << "\t[" << mQT.getLeft() << ", " << mQT.getRight() << "] ["
		<< mQT.getBottom() << ", " << mQT.getTop() << "]\n";

	for( unsigned int i = 0; i < mPS.getSize(); i++ )
	{
		mQT.add( mPS.getParticle( i ) );
	}

	cout << "Runnnig Barnes-Hut on all particles\n";
	for( unsigned int i = 0; i < mPS.getSize(); i++ )
	{
		mQT.update( mPS.getParticle( i ) );
	}

	cout << "Outputting results\n";
	cout << mPS << '\n';

	cout << "Saving results\n";
	mPS.save( outputName );

	cout << "Exiting cleanly\n";
	return 0;
}

