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

#ifdef GUI
//{{{
#include <SFML/Graphics.hpp>
using namespace sf;

void drawParticleSystem( ParticleSystem* toDraw, RenderWindow& target );
void drawParticleSystem( ParticleSystem* toDraw, RenderWindow& target )
{ //{{{
	if( toDraw == NULL )
		return;

	long double r = toDraw->getRight(), l = toDraw->getLeft();
	float radius = (r - l) / target.GetWidth() * 3.0;

	Particle* tp;
	for( unsigned int i = 0; i < toDraw->getSize(); i++ )
	{
		tp = toDraw->getParticle( i );
		target.Draw( Shape::Circle( tp->x, tp->y, radius, Color::Black ) );
	}
} //}}}

void drawQuadtree( Quadtree* toDraw, RenderWindow& target, unsigned int depth = 0 );
void drawQuadtree( Quadtree* toDraw, RenderWindow& target, unsigned int depth )
{ //{{{
	if( toDraw == NULL )
		return;

	static const Color DEPTH_COLOR[3] = { Color::Red, Color::Black, Color::Blue };
	// Red Green Blue
	// Yellow Magenta Cyan
	Color color( DEPTH_COLOR[ depth % 3 ] );
	float thickness = 0.005;

	long double l = toDraw->getLeft(), r = toDraw->getRight(),
		  b = toDraw->getBottom(), t = toDraw->getTop();

	if( depth == 0 )
	{
		cerr << "Drawing outer square\n";
		target.Draw( Shape::Line( l, t, r, t, thickness, color ) );
		target.Draw( Shape::Line( r, t, r, b, thickness, color ) );
		target.Draw( Shape::Line( r, b, l, b, thickness, color ) );
		target.Draw( Shape::Line( l, b, l, t, thickness, color ) );
	}

	long double mX = (l + r)/2.0, mY = (b + t)/2.0;
	target.Draw( Shape::Line( l, mY, r, mY, thickness, color ) );
	target.Draw( Shape::Line( mX, b, mX, t, thickness, color ) );

	if( !toDraw->isParent() )
		return;

	for( unsigned int i = 0; i < 4; i++ )
		drawQuadtree( toDraw->getChild( i ), target, depth + 1 );
} //}}}
//}}}
#endif

void testRMSE( string fileName, long double tau, int argc );
void simulate( stirng fileName, long double tau, int argc );

void printDimensions( ParticleSystem &ps );
void printDimensions( Quadtree &qt );

int main( int argc, char** argv )
{
	// Print args, determine testRMSE {{{
	bool testRMSE = false;
	cout << "Arguments:\n";
	for( int i = 0; i < argc; i++ )
	{
		cout << "   " << i << ": " << argv[i] << '\n';
		if( (string)argv[i] == "-t" )
			testRMSE = true;
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

	if( testRMSE )
		testRMSE( fileName, tau );
	else
		simulate( fileName, tau );

	cout << "Exiting cleanly\n";
	return 0;
}

void simulate( stirng fileName, long double tau, int argc )
{ //{{{
	ParticleSystem mPS( fileName );
	if( mPS.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return;
	}
	printDimensions( mPS );

	cout << "Putting all particles into Quadtree, let's see if we SIGSEGV\n";
	Quadtree mQT( mPS );
	mQT.setTau( tau );
	printDimensions( mQT );

	cout << "Runnnig Barnes-Hut on all particles\n";
	for( unsigned int i = 0; i < mPS.getSize(); i++ )
		mQT.update( mPS.getParticle( i ) );

	if( argc > 3 )
		cout << mPS << '\n';

	cout << "Saving results\n";
	mPS.save( outputName );

#ifdef GUI
	//{{{
	if( argc < 5 )
		return 0;

	RenderWindow window( VideoMode( 1000, 1000 ), "B-H", Style::Close );
	View view( FloatRect( l, b, r, t ) );
	window.SetView( view );
	window.SetFramerateLimit( 30 );
	window.Clear( Color::White );

	drawQuadtree( &mQT, window );
	drawParticleSystem( &mPS, window );

	// Display the image until it's closed {{{
	Event event;
	while( window.IsOpened() )
	{
		while( window.GetEvent( event ) )
		{
			if( event.Type == Event::Closed )
				window.Close();
		}
		if( window.GetInput().IsKeyDown( Key::Escape ) )
			window.Close();

		window.Display();
	}
	// }}}
	//}}}
#endif

} //}}}

void testRMSE( string fileName, long double tau, int argc )
{ //{{{
	return;

	ParticleSystem bruteForce( fileName );
	if( bruteForce.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return;
	}
	printDimensions( bruteForce );

	cout << "Creating inital quadtree and running brute force simulation\n";
	Quadtree bfTree( bruteForce );
	bfTree.setTau( 0 );

	for( unsigned int i = 0; i < bruteForce.getSize(); i++ )
		bfTree.update( bruteForce.getParticle( i ) );

} //}}}

void printDimensions( ParticleSystem &ps )
{ //{{{
	cout << "\t[" << ps.getLeft() << ", " << ps.getRight() << "] ["
		<< ps.getBottom() << ", " << ps.getTop() << "]\n";
} //}}}

void printDimensions( Quadtree &qt )
{ //{{{
	cout << "\t[" << qt.getLeft() << ", " << qt.getRight() << "] ["
		<< qt.getBottom() << ", " << qt.getTop() << "]\n";
} //}}}

