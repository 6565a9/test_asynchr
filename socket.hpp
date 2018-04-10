#pragma once
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/time.h>

#include"throw.hpp"

#define ELIF(what) else if(what)

class Server;

class Socket{
	protected:
		bool blocked=0;
		int m_fd=-1, m_family;

	public:
		bool isBlocked(void){
			return blocked;
		}
	public:
		Socket operator=(Socket & sock){
			close(m_fd);
			blocked=sock.blocked;
			m_fd=sock.m_fd;
		}
		Socket operator=(Socket sock){
			close(m_fd);
			blocked=sock.blocked;
			m_fd=sock.m_fd;
		}
		bool operator!(void){
			if(m_fd == -1) return true;
		} 
	public:
		void setNonblock(bool nonblock=true);
		void openSocket(int domain, int type, int protocol);
		const int & getFD(){
			return m_fd;
		}
		~Socket(void);	 
};


class SocketTalking : public Socket{
	protected:
		SocketTalking()=default;
	public:
		void write(int fd, std::string msg, int flag=MSG_DONTWAIT){
			if( send(fd, msg.c_str(), msg.size(), flag) == -1)	
				Throw::throw_error("Can't send to socket" );
		}
		std::string read(int fd, std::size_t size=1024, int flag=MSG_DONTWAIT){
			char buf[size];
			memset(buf, 0, size);
			//char * buf = new char[size];
			if( recv(fd, buf, size, flag) == -1 ){
			//	delete buf;
				Throw::throw_error("Can't read from socket" );
			}
			//delete buf;
			return std::string(buf);
		}
};

class Client;

class Server : public SocketTalking{
		
		public:
			Server(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0){
				openSocket(domain, type, protocol);
			}
			virtual void accepting(void)=0;
			virtual bool binding ( std::string& , int, unsigned int ) = 0;
};
