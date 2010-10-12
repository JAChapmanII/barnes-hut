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
#ifndef ERROR_TESTER_HPP
#define ERROR_TESTER_HPP

#include <string>

#include <QtCore/QThread>

#include "particle_system.hpp"
#include "quadtree.hpp"

class ErrorTester : public QThread
{
	public:
		/**
		 * Construct an ErrorTester to test a file with tau from 0 to iTau.
		 * @param iFileName : file to load particle system from
		 * @param iTau : max tau to go to
		 */
		ErrorTester( std::string iFileName, long double iTau = 0.5 );

		/**
		 * Run the RMSE test for the Barnes-Hut algorithm.
		 */
		void run();

		/**
		 * Returns a pointer to the current brute-force simulation.
		 * @return : this's brute force simulation
		 */
		ParticleSystem* getBruteForce();

		/**
		 * Returns the currently associated file name.
		 * @return : file name this has
		 */
		std::string getFileName() const;

		/**
		 * Returns the start of tau for this test.
		 * @return : min tau
		 */
		long double getMinTau() const;

		/**
		 * Returns the end of tau for this test.
		 * @return : max tau
		 */
		long double getMaxTau() const;

		/**
		 * Returns the tau delta for this test.
		 * @return : tau delta
		 */
		long double getTauDelta() const;

		/**
		 * Sets the brute-force simulation to point towards something new.
		 * @param nBruteForce : pointer to new simulation
		 */
		void setBruteForce( ParticleSystem* nBruteForce = NULL );

		/**
		 * Sets the file name this is associated with.
		 * @param nFileName : new file name
		 */
		void setFileName( std::string nFileName );

		/**
		 * Sets the min tau for this test.
		 * @param nTau : new min tau
		 */
		void setMinTau( long double nTau = 0.0 );

		/**
		 * Sets the max tau for this test.
		 * @param nTau : new max tau
		 */
		void setMaxTau( long double nTau = 0.5 );

		/**
		 * Sets the tau delta for this test.
		 * @param nTauDelta : new tau delta
		 */
		void setTauDelta( long double nTauDelta = 0.0001 );

		/**
		 * Creates a brute-force simulation and returns it.
		 * @param fileName : file to load
		 */
		static ParticleSystem* generateBruteForce( std::string fileName );

	private:
		std::string fileName;
		long double minTau;
		long double maxTau;
		long double tauDelta;

		ParticleSystem* bruteForce;
		long double** RMSE;

		ErrorTester( const ErrorTester& rhs );
		ErrorTester& operator=( const ErrorTester& rhs );
};

#endif // ERROR_TESTER_HPP
