#include "procesamiento.h"

using json = nlohmann::json;

std::list<std::string> procesamiento(json j, std::list<std::string> names)
{
	std::list<std::string> tweets; // lista donde guardaremos tweets
	char* intro = j["user"]["name"] ;	// todo: nose bien como es la structura del json
	std::string introduccion(intro);
	introduccion  += ": - ";
	std::string terminador = " -";
	for (auto c : names)
	{
		//eliminamos el url al final para mostrar
		int extended = c.find("https");
		c = c.substr(0, extended);
		tweets.push_back( introduccion + c + terminador);
		
		std::cout << c << std::endl;
		
	}
	return tweets;
}
