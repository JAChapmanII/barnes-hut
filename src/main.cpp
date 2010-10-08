/** {{{
 * Copyright 2010 Jeff Chapman.
 *
 * This file is a part of aos
 *
 * aos is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * aos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with aos.  If not, see <http://www.gnu.org/licenses/>.
 *
 */// }}}
#include <iostream>
using std::cout;
using std::cerr;
using std::cin;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

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
	cout << "Selected file: " << fileName << '\n';

	ifstream particleDescriptor( fileName.c_str() );
	if( particleDescriptor.bad() || !particleDescriptor.good() )
	{
		cerr << "Your file is bad\n";
		return -1;
	}

	return 0;
}

