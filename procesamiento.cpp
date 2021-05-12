#include "procesamiento.h"

#include <map>

using json = nlohmann::json;


static std::map<std::string, std::string> monthTable = {
	{"Jan", "01"},
	{"Feb", "02"},
	{"Mar", "03"},
	{"Apr", "04"},
	{"May", "05"},
	{"Jun", "06"},
	{"Jul", "07"},
	{"Aug", "08"},
	{"Sep", "09"},
	{"Oct", "10"},
	{"Nov", "11"},
	{"Dec", "12"},
};


static void formatDate(std::string&);

bool procesamiento(json& tweetJSON, std::list<std::string>& tweetList)
{

	std::string intro = tweetJSON[0]["user"]["name"];
	std::string introduccion = intro;
	introduccion  += ": - ";
	std::string terminador = " -";
	int i = 0;


	for (auto element : tweetJSON)
	{
		//eliminamos el url al final para mostrar
		std::string auxString = element["text"];
		int extended = auxString.find("https");
		auxString = auxString.substr(0, extended);
		std::string fecha = element["created_at"]; // todo : lo unico que falta es cambiar el formato de la fecha a como dice en el tp
		formatDate(fecha);
		tweetList.push_back( fecha + introduccion + auxString + terminador );
		//std::cout << c << std::endl;
		//String twitterFormat = "EEE MMM dd HH:mm:ss ZZZZZ yyyy";
		//o DD/MM/AA – hh:mm
	}
	return true;
}

static void formatDate (std::string& fecha)
{
	std::string year = fecha.substr(28);
	std::string day = fecha.substr(8, 9);
	std::string mes = fecha.substr(4, 6);
	std::string hourminute = fecha.substr(10,15);
	fecha = day + '/' + monthTable[mes] + '/' + year + " - " + hourminute;
	 
	
}