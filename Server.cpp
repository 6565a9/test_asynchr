#include"Server.hpp"



void TCPChat::accepting(void)  {
	struct sockaddr_in addr;
	socklen_t  clientlen  = sizeof(addr);
	int newfd = accept(m_fd, (sockaddr*)&addr, &clientlen);

	char ipv4[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ipv4, INET_ADDRSTRLEN);

	try{

		if(newfd > 0){
			clients.at(ipv4);
			write(newfd, "ERROR: You are already connected" );
			close(newfd);			
		}
	}catch(...){
			clients[ipv4]={newfd};
			std::cout << "Connection from " << ipv4 << std::endl;
			std::cout << "Now clients: ";
			for(auto client : clients ){
				std::cout << client.first << std::endl;
			}
	}



	
}


bool TCPChat::binding ( std::string & host , int port, unsigned int limit ) noexcept {
	struct sockaddr_in addr;
	addr.sin_family=m_family;
	addr.sin_port = htons(port);
	addr.sin_addr = { inet_addr(host.c_str()) };
	int res = bind( m_fd, (struct sockaddr *) &addr, sizeof(addr) ) ;
	listen(m_fd, limit);	
	return res >= 0 ;
	
}

void TCPChat::work(void){
	while(1){
		accepting();

		for( auto & client : clients ){
//			std::cout << client.first << std::endl;
//			std::cout << client.second << std::endl;
			try{
					std::string msg = read(client.second.fd);

					if(msg.size() == 0){
						delClient(client.first);					
						continue;
					}
					client.second.timeout=0;

					auto msgs =  Util::trim(msg);
					try{
						funcs.at(msgs[0]);
						( this->*(funcs[msgs[0]]) ) (client.second, msgs, clients);
					}catch(...){
						if(client.second.room_name.size() == 0)
						{
							write(client.second.fd, "ERROR: You are not in room(ENTER NICK ROOMNAME)\n" );
							//TODO: Warnings count, kick.
							continue;
						}
						auto room_vec = rooms[client.second.room_name];
						for( auto fd : room_vec )
							write(fd, client.second.name+ ": " + msg+"\r\n");
						
					}

					std::cout << client.first << ":" << msg << std::endl;					
					continue;
			}catch(...){}
			client.second.timeout+=0.5;
			if( client.second.timeout > need_timeout ){
				delClient(client.first);
			}			
		}
		std::this_thread::sleep_for( half_of_second(1) ); // 
	}
}
