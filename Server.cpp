#include"Server.hpp"




void TCPChat::update_fds(void){
				FD_ZERO(&read_fds);
				FD_SET(m_fd, &read_fds);
				max_fds=m_fd;
				for(auto client : clients ){
					FD_SET( client.second.fd , &read_fds);
					if(client.second.fd > max_fds)
						max_fds=client.second.fd;
				}
				if(  select( max_fds + 1 , &read_fds , nullptr , nullptr , nullptr) < 0)
						Throw::throw_error("Select error");
				std::cout << "FD Was Set" << std::endl;

}

void TCPChat::accepting(void)  {
	if (FD_ISSET(m_fd, &read_fds)){ 
		struct sockaddr_in addr;
		socklen_t  clientlen  = sizeof(addr);
		int newfd = accept(m_fd, (sockaddr*)&addr, &clientlen);

		char ipv4[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ipv4, INET_ADDRSTRLEN);

		try{
	
			if(newfd > 0){
				if( clients[ipv4].fd == 0 ){
					std::cout << "All ok" << std::endl;
					throw(0);
				}
				
				write(newfd, "ERROR: You are already connected" );
				close(newfd);			
			}
		}catch(...){
				
				clients[ipv4]={newfd};
				std::cout << "Connection from " << ipv4 << std::endl;
				std::cout << "Now clients: ";
				FD_ZERO(&read_fds);
				FD_SET(m_fd, &read_fds);
				

				for(auto client : clients ){
					std::cout << client.first << std::endl;
					FD_SET( client.second.fd , &read_fds);
					if(client.second.fd > max_fds)
						max_fds=client.second.fd;
				}

				
				
				if(  select( max_fds + 1 , &read_fds , nullptr , nullptr , nullptr) < 0)
						Throw::throw_error("Select error");
				std::cout << "FD Was Set" << std::endl;
		}
	}//..



	
}


bool TCPChat::binding ( std::string & host , int port, unsigned int limit ) noexcept {
	struct sockaddr_in addr;
	addr.sin_family=m_family;
	addr.sin_port = htons(port);
	addr.sin_addr = { inet_addr(host.c_str()) };
	int res = bind( m_fd, (struct sockaddr *) &addr, sizeof(addr) ) ;
	if( listen(m_fd, limit) < 0) return false;	
	return res >= 0 ;
	
}

void TCPChat::work(void){
	
	while(1){
		update_fds();	
		accepting();

		for( auto & client : clients ){
			
//			std::cout << client.first << std::endl;
//			std::cout << client.second << std::endl;
			if( FD_ISSET( client.second.fd , &read_fds) )
			try{
					
					std::string msg = read(client.second.fd);
					
					if(msg.size() == 0){						
						std::cout << "Continue... " << std::endl;	
						delClient(client.first);			
						continue;
					}
					//client.second.timeout=0;

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
			}catch(std::runtime_error & err){
				std::cerr << err.what() << std::endl;
			}catch(...){}
			std::this_thread::sleep_for( half_of_second(1) ); //
	}
	
}

}
