
#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#include <iostream>

#include <list>


#include "nlohmann/json.hpp"

#include <string.h>

using json = nlohmann::json;

bool procesamiento(json& tweetJSON, std::list<std::string>& tweetList);






#endif // PROCESAMIENTO_H

