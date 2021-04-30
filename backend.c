#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

char urlBase[45] = "https://covid-api.mmediagroup.fr/v1/";

void fetchQuery(const char* url, const char* outputFileName) {
  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );
  FILE* file = fopen( outputFileName, "w" );
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file);
  curl_easy_perform( easyhandle );
  curl_easy_cleanup( easyhandle );
}

void setQuery(const char* country, const char* category) {
  FILE *stream;
  char *buf;
  size_t len;
  stream = open_memstream(&buf, &len);
  fprintf(stream, "%s%s?country=%s", urlBase, category, country);
  fclose(stream);
  printf("%s", buf);
  char fileName[50];
  strcpy(fileName, country);
  strcat(fileName, category);
  fetchQuery(buf, fileName);
}

int main(void) {
  setQuery("France", "cases");
}
