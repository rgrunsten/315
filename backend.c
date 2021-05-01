#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// base API url
char urlBase[45] = "https://covid-api.mmediagroup.fr/v1/";

// uses libcurl to grab API data. Takes a URL and filename for the output
long fetchQuery(const char* url, const char* outputFileName) {

  // generates a curl request, sets the operation to URL curl
  CURL* easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );

  // sets the response to be written a file and performs the curl
  FILE* file = fopen( outputFileName, "w" );
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file);
  curl_easy_perform( easyhandle );
  long httpStatus;
  curl_easy_getinfo ( easyhandle, CURLINFO_RESPONSE_CODE, &httpStatus);
  curl_easy_cleanup( easyhandle );
  fclose(file);
  return httpStatus;
}

// constructs arguments for and then calls fetchQuery
long setQuery(const char* country, const char* category) {

  // uses memstream to construct the API url with country/category
  FILE *stream;
  char *buf;
  size_t len;
  stream = open_memstream(&buf, &len);
  fprintf(stream, "%s%s?country=%s", urlBase, category, country);
  fclose(stream);
  printf("%s\n", buf);

  // designates a name for the output file--for now, country+category
  char fileName[50];
  strcpy(fileName, country);
  strcat(fileName, category);

  // calls fetchQuery using the constructed URL and filename
  return fetchQuery(buf, fileName);
}

// deletes a file with the name fileName
void cleanCache(const char* fileName) {
  remove(fileName);
}

/*int main(void){
  long test;
  test = setQuery("France", "cases");
  printf("%ld\n", test);
}*/
