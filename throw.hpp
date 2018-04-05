#include<iostream>
#include<sstream>

namespace Throw{
	void inline join_msgs(std::string & t){}
	template <typename T, typename ... Args> inline void join_msgs(std::string & t, T msg, Args ... args) {
		t+= static_cast<std::ostringstream*>
					( &(std::ostringstream () << msg) )->str();		
		join_msgs(t, args...);
	}

	template <typename T, typename ... Args> inline void throw_error(T msg, Args ... args){
		std::string end_msg;
		join_msgs(end_msg ,msg, args...);
		throw( std::runtime_error( end_msg ) ) ;
	}
}
