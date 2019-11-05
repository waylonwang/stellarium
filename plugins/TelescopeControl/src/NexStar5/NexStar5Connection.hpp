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

#ifndef NEXSTAR5CONNECTION_HPP
#define NEXSTAR5CONNECTION_HPP

#include "common/SerialPort.hpp"

#include <list>
using namespace std;

class NexStar5Command;

//! Serial port connection to a Celestron NexStar 5/8 or a compatible telescope.
class NexStar5Connection : public SerialPort
{
public:
	NexStar5Connection(Server &server, const char *serial_device);
	~NexStar5Connection(void) { resetCommunication(); }
	void sendGoto(unsigned int ra_int, int dec_int);
	void sendSync(unsigned int ra_int, int dec_int);
	void sendCommand(NexStar5Command * command);
	
private:
	void dataReceived(const char *&p, const char *read_buff_end);
	void sendPosition(unsigned int ra_int, int dec_int, int status) {Q_UNUSED(ra_int); Q_UNUSED(dec_int); Q_UNUSED(status);}
	void resetCommunication(void);
	
private:
	list<NexStar5Command*> command_list;
};

#endif // NEXSTAR5CONNECTION_HPP
