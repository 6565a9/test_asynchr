#include"util.hpp"
namespace Util{
std::vector<std::string> trim(std::string full_msg){
			std::vector<std::string> msgs;
			{

				std::istringstream stream(full_msg);
				std::string tmp;
				while( std::getline(stream, tmp, ' ' ) )
					msgs.push_back(tmp);
			}
			return msgs;
}

}
