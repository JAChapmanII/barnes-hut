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

#include <sstream>
using std::stringstream;

#include <limits>
using std::numeric_limits;

#include <cmath>

#include "error_tester.hpp"
using std::string;

void ErrorTester::run()
{
}

long double* calculateRMSE( ParticleSystem* bf, ParticleSystem* ps );

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
	bruteForce.printDimensions();

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
{ //{{{
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
} //}}}

