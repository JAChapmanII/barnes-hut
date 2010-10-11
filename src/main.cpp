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

#include <limits>
using std::numeric_limits;

#include <cmath>

#include "particle_system.hpp"
#include "quadtree.hpp"

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

void testRMSE( string fileName, string outName, long double tau, int argc );
void simulate( string fileName, string outName, long double tau, int argc );

long double* calculateRMSE( ParticleSystem* bf, ParticleSystem* ps );

void printDimensions( ParticleSystem &ps );
void printDimensions( Quadtree &qt );

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
		testRMSE( fileName, outputName, tau, argc );
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

void testRMSE( string fileName, string outName, long double tau, int argc )
{ //{{{
	ParticleSystem bruteForce( outName, true );
	bool fromSave = true;
	if( bruteForce.getSize() < 1 )
	{
		fromSave = false;
		bruteForce.load( fileName );
		if( bruteForce.getSize() < 1 )
		{
			cout << "No particles in file\n";
			return;
		}
	}
	printDimensions( bruteForce );

	cout << "Creating inital quadtree and running brute force simulation\n";
	Quadtree bfTree( bruteForce );
	bfTree.setTau( 0 );

	if( fromSave )
	{
		cout << "Loaded brute force from file successfully\n";
	}
	else
	{
		for( unsigned int i = 0; i < bruteForce.getSize(); i++ )
			bfTree.update( bruteForce.getParticle( i ) );
		cout << "Bruteforce calculation has been done\n";
		bruteForce.save( outName );
	}

	const long double TAU_DELTA = 0.0001;
	unsigned int totalSteps = (int)(tau / TAU_DELTA);
	cout << "Stepping through tau up to " << tau
		<< " by " << TAU_DELTA << " with a total of " << totalSteps << " steps\n";

	long double ** RMSE = new long double*[ totalSteps + 1 ];
	for( long double ctau = TAU_DELTA; ctau <= tau; ctau += TAU_DELTA )
	{
		stringstream tmp;
		tmp << outName << "_" << ctau;
		cout << ctau << "\t";

		ParticleSystem ps( tmp.str(), true );
		if( ps.getSize() != bruteForce.getSize() )
		{
			ps = bruteForce;
			ps.zeroForces();

			Quadtree qt( ps );
			qt.setTau( ctau );

			for( unsigned int i = 0; i < ps.getSize(); i++ )
				qt.update( ps.getParticle( i ) );

			ps.save( tmp.str() );
		}

		RMSE[ (int)(ctau / TAU_DELTA) ] = calculateRMSE( &bruteForce, &ps );
		cout << RMSE[ (int)(ctau / TAU_DELTA) ][ 0 ] << " "
			<< RMSE[ (int)(ctau / TAU_DELTA ) ][ 1 ] << "\n";
	}
	cout << "\nDone\n";

	cout << "Outputting all RMSE values:\n";
	for( unsigned int i = 1; i < totalSteps; i++ )
		cout << RMSE[ i ][ 0 ] << "\t" << RMSE[ i ][ 1 ] << "\n";

	bool monotonicIncreasingX = true, monotonicIncreasingY = true;
	for( unsigned int i = 2; i < totalSteps; i++ )
	{
		if( RMSE[ i ][ 0 ] < RMSE[ i - 1 ][ 0 ] )
			monotonicIncreasingX = false;
		if( RMSE[ i ][ 1 ] < RMSE[ i - 1 ][ 1 ] )
			monotonicIncreasingY = false;
	}

	if( !monotonicIncreasingX )
		cerr << "X RMSE is not monotonic increasing\n";
	if( !monotonicIncreasingY )
		cerr << "Y RMSE is not monotonic increasing\n";

	cout << "Outputted results\n";
} //}}}

long double* calculateRMSE( ParticleSystem* bf, ParticleSystem* ps )
{
	long double* rmse = new long double[ 2 ];
	rmse[ 0 ] = rmse[ 1 ] = numeric_limits<long double>::infinity();
	if( bf->getSize() != ps->getSize() )
		return rmse;

	long double fx = 0.0, fy = 0.0;
	for( unsigned int i = 0; i < bf->getSize(); i++ )
	{
		long double tfx = 0.0, tfy = 0.0;
		tfx = bf->getParticle( i )->fx - ps->getParticle( i )->fx;
		tfx *= tfx;
		fx += tfx;
		tfy = bf->getParticle( i )->fy - ps->getParticle( i )->fy;
		tfy *= tfy;
		fy += tfy;
	}
	fx /= bf->getSize();
	fy /= bf->getSize();

	rmse[ 0 ] = sqrt( fx );
	rmse[ 1 ] = sqrt( fy );

	return rmse;
}

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

