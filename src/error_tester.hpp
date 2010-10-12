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
		void run();

	private:
		std::string fileName;

		ParticleSystem* bruteForce;
		Quadtree* BFTree;
};

#endif // ERROR_TESTER_HPP
