#include"socket.hpp"


void Socket::openSocket(int domain, int type, int protocol){
	close(m_fd);

	m_fd=socket(domain, type, protocol);
	m_family=domain;

	if(m_fd==-1)
		Throw::throw_error("Can't open socket");
}

void Socket :: setNonblock(bool nonblock){
	int flags;
	if ( flags = fcntl(m_fd, F_GETFL, 0) == -1)
		Throw::throw_error("Can't get acces to socket" );
	if( fcntl(m_fd, F_SETFL, nonblock ? (flags | O_NONBLOCK) : flags & ~O_NONBLOCK) == -1 )
		Throw::throw_error("Can't set nonblock" );
}

Socket::~Socket(void){
	close(m_fd);

}

Socket accept(void){

}

void bind(std::string, int port){

}
