#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

char urlBase[36] = "https://covid-api.mmediagroup.fr/v1/"
char setQuery(const char* country) {
  
}

void fetchQuery(const char* url, const char* countryCacheName) {
  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );
  FILE* file = fopen( countryCacheName, "w" );
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file);
  curl_easy_cleanup( easyhandle );
}

int main(void) {

  
}
