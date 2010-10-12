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

#include "error_tester.hpp"
#include "barnes_hut.hpp"

#ifdef GUI
//{{{
#include <SFML/Graphics.hpp>
using namespace sf;

void drawRectangle( RenderWindow &target,
		float left, float bottom, float right, float top,
		float thickness = 0.005, Color color = Color::Magenta );
void drawRectangle( RenderWindow &target,
		float left, float bottom, float right, float top,
		float thickness, Color color )
{ //{{{
	target.Draw( Shape::Line( left, top, right, top, thickness, color ) );
	target.Draw( Shape::Line( right, top, right, bottom, thickness, color ) );
	target.Draw( Shape::Line( right, bottom, left, bottom, thickness, color ) );
	target.Draw( Shape::Line( left, bottom, left, top, thickness, color ) );
} //}}}

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
		target.Draw( Shape::Circle( tp->x, tp->y, radius,
			((tp->m < 0) ? Color::Red : Color::Blue )
					) );
	}
} //}}}

void drawQuadtree( Quadtree* toDraw, RenderWindow& target, unsigned int depth = 0 );
void drawQuadtree( Quadtree* toDraw, RenderWindow& target, unsigned int depth )
{ //{{{
	if( toDraw == NULL )
		return;

	Color color( Color::Black );
	float thickness = 0.005;

	long double l = toDraw->getLeft(), r = toDraw->getRight(),
		  b = toDraw->getBottom(), t = toDraw->getTop();

	// if this is the root node, draw a border around everything
	if( depth == 0 )
		drawRectangle( target, l, b, r, t, thickness, color );

	float zthick = 0.005; // if this is zero-sum cell, draw a magenta border
	if(( toDraw->getMe() != NULL ) &&
		( fabs( toDraw->getMe()->m ) < numeric_limits<long double>::epsilon() ))
		drawRectangle( target, l, b, r, t, zthick );

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

void simulate( string fileName, string outName, long double tau, int argc );

int main( int argc, char** argv )
{
	// Print args, determine doTest {{{
	bool doTest = false;
	cout << "Arguments:\n";
	for( int i = 0; i < argc; i++ )
	{
		cout << "   " << i << ": " << argv[i] << '\n';
		if( (string)argv[i] == "-t" )
			doTest = true;
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

	if( doTest )
	{
		ParticleSystem* bruteForce = ErrorTester::generateBruteForce( fileName );
		ErrorTester mET( fileName, tau );
		mET.setBruteForce( bruteForce );

		mET.setMinTau( 0.0001 );
		mET.setMaxTau( tau / 8.0 );
		mET.start();
		mET.wait();

		mET.setMinTau( tau / 8.0 );
		mET.setMaxTau( tau / 4.0 );
		mET.start();
		mET.wait();

		mET.setMinTau( tau / 4.0 );
		mET.setMaxTau( tau / 2.0 );
		mET.start();
		mET.wait();

		mET.setMinTau( tau / 2.0 );
		mET.setMaxTau( tau );
		mET.setBruteForce( bruteForce );
		mET.start();
		mET.wait();
	}
	else
		simulate( fileName, outputName, tau, argc );

	cout << "Exiting cleanly\n";
	return 0;
}

void simulate( string fileName, string outName, long double tau, int argc )
{ //{{{
	ParticleSystem mPS( fileName );
	if( mPS.getSize() < 1 )
	{
		cout << "No particles in file\n";
		return;
	}
	mPS.printDimensions();

	cout << "Putting all particles into Quadtree, let's see if we SIGSEGV\n";
	Quadtree mQT( &mPS );
	mQT.setTau( tau );
	mQT.printDimensions();

	cout << "Runnnig Barnes-Hut on all particles\n";
	BarnesHut mBH( &mPS, &mQT );
	mBH.setLast( mPS.getSize() );
	mBH.start();
	mBH.wait();

	if( argc > 3 )
		cout << mPS << '\n';

	cout << "Saving results\n";
	mPS.save( outName );

#ifdef GUI
	//{{{
	if( argc < 5 )
		return;

	RenderWindow window( VideoMode( 1000, 1000 ), "B-H", Style::Close );
	View view( FloatRect( mQT.getLeft(), mQT.getBottom(),
				mQT.getRight(), mQT.getTop() ) );
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

