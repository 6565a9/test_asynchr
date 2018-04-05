#include"Server.hpp"

int main(int argc, char **argums){
	if(argc < 4)
		return fprintf(stderr, "%s host port listen\n", argums[0]);
//QHSD 8 16 32 64 IF /int/float
//int is 32 bit, 4 byte. 1<<4 == 1*2^(4*8) == 4294967296, so more than 10k users.
//printf("%d\n", sizeof(int));
	TCPChat chat{argums[1], atoi(argums[2]), static_cast<unsigned int>(atoi(argums[3])) };
	chat.work();

}
