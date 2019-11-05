/*
The stellarium telescope library helps building
telescope server programs, that can communicate with stellarium
by means of the stellarium TCP telescope protocol.
It also contains smaple server classes (dummy, Meade LX200).

Author and Copyright of this file and of the stellarium telescope library:
Johannes Gajdosik, 2006, modified for NexStar telescopes by Michael Heinz.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
*/

#include "NexStar5Command.hpp"
#include "TelescopeClientDirectNexStar5.hpp"
#include "common/LogFile.hpp"

#include <cmath>

using namespace std;

NexStar5Command::NexStar5Command(Server &server) : server(*static_cast<TelescopeClientDirectNexStar5*>(&server)), has_been_written_to_buffer(false)
{
}

NexStar5CommandGotoPosition::NexStar5CommandGotoPosition(Server &server, unsigned int ra_int, int dec_int) : NexStar5Command(server)
{
	dec = dec_int;
	ra = static_cast<int>(ra_int);
}

#define NIBTOASCII(x) (((x)<10)?('0'+(x)):('A'+(x)-10))
#define ASCIITONIB(x) (((x)<'A')?((x)-'0'):((x)-'A'+10))

bool NexStar5CommandGotoPosition::writeCommandToBuffer(char *&p,char *end)
{
	#ifdef DEBUG5
	char *b = p;
	#endif
	
	if (end-p < 6)
		return false;

	*p++ = 'R';
	
	// set the RA
	int x = ra;
	*p++ = NIBTOASCII ((x>>4) & 0x0f);
	*p++ = NIBTOASCII (x & 0x0f); 
	*p++ = ',';

	// set object dec:
	x = dec;
	*p++ = NIBTOASCII ((x>>4) & 0x0f);
	*p++ = NIBTOASCII (x & 0x0f); 
	*p = 0;

	has_been_written_to_buffer = true;
	#ifdef DEBUG5
	*log_file << Now() << "NexStar5CommandGotoPosition::writeCommandToBuffer:"
	          << b << endl;
	#endif
	
	return true;
}

int NexStar5CommandGotoPosition::readAnswerFromBuffer(const char *&buff, const char *end) const
{
	if (buff >= end)
		return 0;
	
	if (*buff=='@')
	{
		#ifdef DEBUG4
		*log_file << Now() << "NexStar5CommandGotoPosition::readAnswerFromBuffer: slew ok"
		          << endl;
		#endif
	}
	else
	{
		#ifdef DEBUG4
		*log_file << Now() << "NexStar5CommandGotoPosition::readAnswerFromBuffer: slew failed." << endl;
		#endif
	}
	buff++;
	return 1;
}

void NexStar5CommandGotoPosition::print(QTextStream &o) const
{
	o << "NexStar5CommandGotoPosition("
	  << ra << "," << dec <<')';
}

bool NexStar5CommandGetRaDec::writeCommandToBuffer(char *&p, char *end)
{
	if (end-p < 1)
		return false;
	// get RA:
	*p++ = 'E';
	has_been_written_to_buffer = true;
	return true;
}

int NexStar5CommandGetRaDec::readAnswerFromBuffer(const char *&buff, const char *end) const
{
	if (end-buff < 6)
		return 0;

	int ra, dec;
	const char *p = buff;

	// Next 2 bytes are RA.
	ra = 0;
	ra += ASCIITONIB(*p); ra <<= 4; p++;
	ra += ASCIITONIB(*p); p++;

	if (*p++ != ',')
	{
		#ifdef DEBUG4
		*log_file << Now() << "NexStar5CommandGetRaDec::readAnswerFromBuffer: "
		                      "error: ',' expected" << endl;
		#endif
		return -1;
	}

	// Next 2 bytes are DEC.
	dec = 0;
	dec += ASCIITONIB(*p); dec <<= 4; p++;
	dec += ASCIITONIB(*p); p++;

	if (*p++ != '@')
	{
		#ifdef DEBUG4
		*log_file << Now() << "NexStar5CommandGetRaDec::readAnswerFromBuffer: "
		                      "error: '#' expected" << endl;
		#endif
		return -1;
	}
	
	
	#ifdef DEBUG4
	*log_file << Now() << "NexStar5CommandGetRaDec::readAnswerFromBuffer: "
	                      "ra = " << ra << ", dec = " << dec
	          << endl;
	#endif
	buff = p;

	server.raReceived(static_cast<unsigned int>(ra));
	server.decReceived(static_cast<unsigned int>(dec));
	return 1;
}

void NexStar5CommandGetRaDec::print(QTextStream &o) const
{
	o << "NexStar5CommandGetRaDec";
}

