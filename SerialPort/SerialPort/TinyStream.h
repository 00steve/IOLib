#pragma once


/*
HOW DATA IS INTERPRETED:
- Send out a ping to see if it gets a reply.
- If a valid reply is recieved, send a request for the stream properties
- The foreign device should send back some bytes formatted like so:

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







class TinyStream{
	bool SetInput(char &buffer);
};

