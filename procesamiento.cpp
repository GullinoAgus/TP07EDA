#include "procesamiento.h"

using json = nlohmann::json;

std::list<std::string> procesamiento(json j, std::list<std::string> names, std::string usuario)
{
	std::list<std::string> tweets; // lista donde guardaremos tweets
	std::string intro = usuario ;	
	std::string introduccion = intro;
	introduccion  += ": - ";
	std::string terminador = " -";
	int i = 0;
	for (auto c : names)
	{
		//eliminamos el url al final para mostrar
		int extended = c.find("https");
		c = c.substr(0, extended);
		std::string fecha = j[i]["created_at"]; // todo : lo unico que falta es cambiar el formato de la fecha a como dice en el tp
		tweets.push_back( fecha +'\n' + introduccion + c + terminador );
		i++;
		//std::cout << c << std::endl;
		
	}
	return tweets;
}
