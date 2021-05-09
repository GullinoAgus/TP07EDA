#include "TweetDLoader.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static size_t recieveCallback(char* ptr, size_t size, size_t nmemb, void* userdata);


TweetDLoader::TweetDLoader(const char* API_key, const char* API_SecretKey) : APIKey(API_key), 
																			APISecretKey(API_SecretKey)
{
	this->curl = curl_easy_init();
	if (curl || API_key == nullptr || API_SecretKey == nullptr)
	{
		curl_easy_setopt(this->curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");
		curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(this->curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(this->curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		std::string credentials = std::string(this->APIKey) + ":" + std::string(this->APISecretKey);

		curl_easy_setopt(this->curl, CURLOPT_USERPWD, credentials.c_str());

		struct curl_slist* header = nullptr;
		header = curl_slist_append(header, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, header);

		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data.c_str());

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receivedData);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->receivedData);

		if (curl_easy_perform(curl) != CURLE_OK)
		{
			curl_easy_cleanup(curl);
			throw std::exception("ERR: Failed request to Twitter");

		}

		curl_easy_cleanup(curl);

		json recievedJson = json::parse(this->receivedData);
		this->token = recievedJson["access_token"];

	}
	else
	{
		throw std::exception("ERR: Unable to initialize cURL");
	}
}


bool TweetDLoader::download(std::list<std::string>& buffer, const char* usrname, unsigned int tweetCount)
{



}



static size_t recieveCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t realsize = size * nmemb;
	std::string* usrdata = (std::string*)userdata;
	usrdata->append(ptr);
	return realsize;
}