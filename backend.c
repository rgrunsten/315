#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

char urlBaseCases[50] = "https://covid-api.mmediagroup.fr/v1/cases?country=";
char urlBaseVaccines[53] = "https://covid-api.mmediagroup.fr/v1/vaccines?country=";

void fetchQuery(const char* url, const char* countryCacheName) {
  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );
  FILE* file = fopen( countryCacheName, "w" );
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file);
  curl_easy_perform( easyhandle );
  curl_easy_cleanup( easyhandle );
}

void setQuery(const char* country) {
  FILE *stream;
  char *buf;
  size_t len;
  stream = open_memstream(&buf, &len);
  fprintf(stream, "%s%s", urlBaseCases, country);
  //strcat(urlBaseCases, country);
  //strcat(urlBaseVaccines, country);
  fclose(stream);
  printf("%s", buf);
  fetchQuery(buf, "hardcode");
}

int main(void) {
  setQuery("France");
}
