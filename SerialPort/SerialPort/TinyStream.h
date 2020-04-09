#pragma once

#define DATAFLOWDIRECTION_NONE 0
#define DATAFLOWDIRECTION_WRITE 1
#define DATAFLOWDIRECTION_READ 2
#define DATAFLOWDIRECTION_BIDIRECTIONAL 3

/*
HOW DATA IS INTERPRETED:
- Send out a ping to see if it gets a reply.
- If a valid reply is recieved, send a request for the stream properties
- The foreign device should send back some bytes formatted like so:


byte[n0] : pair request
	byte[no{0,1}] : 
		x00xxxxxx: 0 = greeting (are you there?)
		x01xxxxxx: 1 = reply (include what type of information flow should be included [n0{2,3}]
		x10xxxxxx: 2 = update (include extra data that will change the contract)
	byte[n0{2,3}] :
		x0000xxxx:		0 = greeting (just send back that you exist)
		x0001xxxx:16:	1 = greeting (reply to me)
		x0100xxxx:		0 = reply (no data flow)
		x0101xxxx:		1 = reply (sending information)
		x0110xxxx:		2 = reply (receiving information)
		x0111xxxx:		3 = reply (bi-directional data flow)
		x1000xxxx:		0 = update

if a reply is recieved of 3, we should expect that the data flow being sent will be sent, but that 
we should send back any data that we are going to send in the other direction.


byte[n0] : streamCount = how many data streams there are (0-255)

byte[n1-streamCount-n1^]:
	byte[n1] : streamNameLength = length of name of stream
	byte[n2-streamNameLength+n1] : characters of name of stream
	byte[n3{0}] : is array
	byte[n3{1}] : is array variable length

	byte[n3{2,3}] : if array, set data type to use to get the number of objects in the array
					to return, or the data type that is used to count the current number of
					items to return if variable size array
	byte[n3{4,5,6}] : type of data (up to 8 values)
		0 = bit stream (1 bit)(boolean)
		1 = char (1 byte)
		2 = short (2 bytes)
		3 = int (4 bytes)
		4 = long (8 bytes)
		5 = float (4 bytes)
		6 = double (8 bytes)
		7 = long double (12 byte)
	byte[n3{7}] : sub-type of data (doesn't really matter for how the data is stored)
		0 = char[unsigned], short[unsigned], int[unsigned], long[unsigned]
		1 = char[signed], short[signed], int[signed], long[signed]



*/


#include <iostream>
#include "IIO.h"


class TinyStream{
private:

	unsigned char dataFlowDirection;

	IIO* IO;
	unsigned char state;


public:


	void SetDataFlowDirection(const unsigned char dataFlowDirection);
	bool SetInput(IIO* IO);

	TinyStream();

	~TinyStream();

	/*do everything it needs to do, based on the state it's in. If it hasn't paired with 
	anything, set up the contract, if it has, try to read data from it.*/
	bool Update();
};

