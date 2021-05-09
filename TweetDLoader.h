#pragma once
#include <iostream>
#include <curl/curl.h>
#include <list>

class TweetDLoader
{
public:
	TweetDLoader(const char* API_key, const char* API_SecretKey);

	bool download(std::list<std::string>& buffer, const char* usrname, unsigned int tweetCount = 0);

private:
	const char* APIKey;
	const char* APISecretKey;
	std::string token;
	std::string receivedData;
	CURL* curl;
	CURLM* multiCurl;

	
};

