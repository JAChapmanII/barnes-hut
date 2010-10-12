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

#include <fstream>
using std::ofstream;

#include <iomanip>
using std::fixed;
using std::setprecision;
using std::setw;

#include <limits>
using std::numeric_limits;

#include <cmath>

#include "error_tester.hpp"
using std::string;

ErrorTester::ErrorTester( std::string iFileName, long double iTau ) :
	fileName( iFileName ), //{{{
	minTau( 0.0 ),
	maxTau( iTau ),
	tauDelta( 0.0001 ),
	bruteForce( NULL ),
	RMSE( NULL )
{
	this->bruteForce = new ParticleSystem();
} //}}}

ErrorTester::~ErrorTester()
{ //{{{
	if( this->bruteForce != NULL )
	{
		delete this->bruteForce;
		this->bruteForce = NULL;
	}
} //}}}

void ErrorTester::run()
{ //{{{
	if( this->maxTau < this->minTau )
	{
		cerr << "Tau bounds do not allow for any steps\n";
		return;
	}
	if( this->bruteForce == NULL )
	{
		cerr << "Brute force calculation was not providided\n";
		return;
	}
	unsigned int totalSteps = 1 +
		(int)( ( this->maxTau - this->minTau ) / this->tauDelta );
	cout << "[" << this->minTau << ", " << this->maxTau << "] "
		<< this->tauDelta << " (" << totalSteps << ")\n";

	this->RMSE = new long double*[ totalSteps ];
	ParticleSystem* ctauPS = new ParticleSystem();
	(*ctauPS) = *bruteForce;
	Quadtree* ctauQT = new Quadtree( ctauPS );
	for( long double ctau = this->minTau; ctau < this->maxTau;
			ctau += this->tauDelta )
	{
		ctauPS->zeroForces();
		ctauQT->update( ctauPS );

		unsigned int i = (int)(ctau / this->tauDelta) -
			(int)((ctau - this->minTau)/ this->tauDelta);
		this->RMSE[ i ] = ErrorTester::calculateRMSE(
				this->bruteForce, ctauPS );
		cout
			<< fixed << setprecision( 4 ) << setw( 8 ) << ctau << '\t'
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->RMSE[ i ][ 0 ] << '\t'
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->RMSE[ i ][ 1 ] << '\n';
	}
	delete ctauPS;
	delete ctauQT;

	this->save();
} //}}}

void ErrorTester::save() const
{ //{{{
	stringstream tmp; tmp << this->fileName << "_" << this->minTau
		<< "_" << this->maxTau << "_" << this->tauDelta;
	cout << "Saving RMSE values to " << tmp.str() << "\n";
	ofstream outFile( tmp.str().c_str() );

	if( !outFile.good() )
	{
		cerr << "Could not save RMSE values\n";
		return;
	}

	unsigned int totalSteps = 1 +
		(int)( ( this->maxTau - this->minTau ) / this->tauDelta );
	for( long double ctau = this->minTau; ctau < this->maxTau;
			ctau += this->tauDelta )
	{
		unsigned int i = (int)(ctau / this->tauDelta) -
			(int)((ctau - this->minTau)/ this->tauDelta);
		outFile
			<< fixed << setprecision( 4 ) << setw( 8 ) << ctau << '\t'
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->RMSE[ i ][ 0 ] << '\t'
			<< fixed << setprecision( 4 ) << setw( 8 ) << this->RMSE[ i ][ 1 ] << '\n';
	}
} //}}}

ParticleSystem* ErrorTester::getBruteForce()
{ //{{{
	return this->bruteForce;
} //}}}

string ErrorTester::getFileName() const
{ //{{{
	return this->fileName;
} //}}}

long double ErrorTester::getMinTau() const
{ //{{{
	return this->minTau;
} //}}}

long double ErrorTester::getMaxTau() const
{ //{{{
	return this->maxTau;
} //}}}

long double ErrorTester::getTauDelta() const
{ //{{{
	return this->tauDelta;
} //}}}

void ErrorTester::setBruteForce( ParticleSystem* nBruteForce )
{ //{{{
	*(this->bruteForce) = *nBruteForce;
} //}}}

void ErrorTester::setFileName( string nFileName )
{ //{{{
	this->fileName = nFileName;
} //}}}

void ErrorTester::setMinTau( long double nTau )
{ //{{{
	this->minTau = nTau;
} //}}}

void ErrorTester::setMaxTau( long double nTau )
{ //{{{
	this->maxTau = nTau;
} //}}}

void ErrorTester::setTauDelta( long double nTauDelta )
{ //{{{
	this->tauDelta = nTauDelta;
} //}}}

ParticleSystem* ErrorTester::generateBruteForce( string fileName )
{ //{{{
	cout << "Beginning brute-force calculation\n";

	ParticleSystem* bfResult = new ParticleSystem( fileName );
	if( bfResult->getSize() < 1 )
	{
		cerr << "Brute-force calculation could not be completed\n";
		return NULL;
	}

	Quadtree BFTree( bfResult );
	BFTree.setTau( 0 );

	BFTree.update( bfResult );

	cout << "Brute-force calculation done\n";
	return bfResult;
} //}}}

long double* ErrorTester::calculateRMSE( ParticleSystem* bruteForce,
		ParticleSystem* BarnesHut )
{ //{{{
	long double* RMSE = new long double[ 2 ];
	RMSE[ 0 ] = RMSE[ 1 ] = numeric_limits<long double>::infinity();
	if( bruteForce->getSize() != BarnesHut->getSize() )
		return RMSE;

	long double sumErrorSquaredFX = 0.0, sumErrorSquaredFY = 0.0;
	Particle* bfP; Particle* bhP;
	for( unsigned int i = 0; i < bruteForce->getSize(); i++ )
	{
		bfP = bruteForce->getParticle( i );
		bhP = BarnesHut->getParticle( i );
		sumErrorSquaredFX += (bfP->fx - bhP->fx) * (bfP->fx - bhP->fx);
		sumErrorSquaredFY += (bfP->fy - bhP->fy) * (bfP->fy - bhP->fy);
	}
	RMSE[ 0 ] = sqrt( sumErrorSquaredFX / bruteForce->getSize() );
	RMSE[ 1 ] = sqrt( sumErrorSquaredFY / bruteForce->getSize() );

	return RMSE;
} //}}}

