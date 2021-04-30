#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

int main(void) {
  

  char url[70] = "https://covid-api.mmediagroup.fr/v1/cases?country=France";

  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );

  curl_easy_perform( easyhandle );

  curl_easy_cleanup( easyhandle );

}
