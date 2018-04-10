#pragma once
#include"socket.hpp"
#include<map>
#include<chrono>
#include<thread> // for std::this_thread::sleep_for
#include<algorithm>
#include"util.hpp"

constexpr double need_timeout = 180;

constexpr const char login[] = "ENTER";



class TCPChat : public Server{
	protected:
			fd_set read_fds;
			int max_fds;
	

	using half_of_second = std::chrono::duration<int, std::ratio<1,2>> ;
	struct client{
		//client(int fd, double timeout=0):fd(fd), timeout(timeout){}
		int fd;
		double timeout=0;
		std::string room_name{""};
		
		std::string name{""};
	};

///
	protected:
		void inline add_timeout( double timeout=0.5 ){
			for(auto & client : clients){
				client.second.timeout+=timeout;
				if( client.second.timeout > need_timeout )
					delClient(client.first);
					
			}
		}

	using command_chat = bool (TCPChat::*)(client & c, const std::vector<std::string> msgs, const std::map<std::string, client> clients );
///

	std::map<std::string, std::vector<int>> rooms;
	std::map<std::string, command_chat> funcs;

	protected:
		std::map<std::string, client> clients; 

	protected:

	//commmands. Я не стал создавать для одной комманды отдельный класс.

	bool welcome_msg(client & c, const std::vector<std::string> msgs, const std::map<std::string, client> clients)// ENTER NICK ROOM
		{
			if(msgs.size() < 3) 
			{
				write(	c.fd, "not enought params...\n"  );
				return false;
			}
			
			for(auto & client : clients )
				if( client.second.name == msgs[1] ){
					write( c.fd, "nick busy\n" );
					return false;
				}
				

			c.name=msgs[1];
			c.room_name=msgs[2];
			rooms[msgs[2]].push_back(c.fd);
			write( c.fd, "Succefuly: you are joined\r\n" );
		}
		
	protected:
		void inline delClient(const std::string host){
				try{
					auto room_vec = rooms[clients[host].room_name];
					auto element = std::find(room_vec.cbegin(), room_vec.cend(), clients[host].fd );
					if(element != room_vec.cend() )
						room_vec.erase( element );

					clients.at(host);
					
					close(clients[host].fd);
					
					std::cout << host << " disconnected " << std::endl;
					clients.erase( clients.find(host) );						
				}catch(...){}
				update_fds();
		}
		void update_fds(void);
	public:
		TCPChat(std::string host, const int port, const unsigned int limit=10000):
			Server()
		{

			if( !binding(host, port, limit) ) 
				Throw::throw_error("Can't bind to ", host, ":", port, " ", strerror(errno) );
			//setNonblock();
			funcs[login] = &TCPChat::welcome_msg;
			update_fds();

		}
	
		void accepting(void) override;
		bool binding ( std::string& , int, unsigned int ) noexcept override;
	public:
		void work(void);
};
