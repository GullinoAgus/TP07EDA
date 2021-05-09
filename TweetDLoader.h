#pragma once
#include <iostream>
#include <curl/curl.h>
#include <list>

#define DEFAULT_TWEET_COUNT 1

typedef enum {IDLE = 0, DOWNLOADING} state_t;

class TweetDLoader
{
public:
	TweetDLoader(const char* API_key, const char* API_SecretKey);

	int getStatus();
	bool download(std::list<std::string>& buffer, const char* usrname, unsigned int tweetCount = 0);

private:
	int status;
	const char* APIKey;
	const char* APISecretKey;
	std::string token;
	std::string receivedData;
	std::string queryLink;
	CURL* curl;
	CURLM* multiCurl;
	CURLMcode err;

	
};

