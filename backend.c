#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/* base API url */
char urlBase[45] = "https://covid-api.mmediagroup.fr/v1/";

/* uses libcurl to grab API data. Takes a URL and filename for the output */
long fetchQuery(const char* url, const char* outputFileName) {
  CURL* easyhandle;
  FILE* file;
  long httpStatus;
  /* generates a curl request, sets the operation to URL curl */
  easyhandle = curl_easy_init();
  curl_easy_setopt( easyhandle, CURLOPT_URL, url );

  /* sets the response to be written a file and performs the curl */
  file = fopen( outputFileName, "w" );
  curl_easy_setopt( easyhandle, CURLOPT_WRITEDATA, file);
  curl_easy_perform( easyhandle );
  curl_easy_getinfo ( easyhandle, CURLINFO_RESPONSE_CODE, &httpStatus);
  curl_easy_cleanup( easyhandle );
  fclose(file);
  return httpStatus;
}

/* constructs arguments for and then calls fetchQuery */
long setQuery(const char* country, const char* category) {

  /* uses memstream to construct the API url with country/category */
  FILE *stream;
  char *buf;
  size_t len;
  char fileName[50];
  
  stream = open_memstream(&buf, &len);
  fprintf(stream, "%s%s?country=%s", urlBase, category, country);
  fclose(stream);

  /* designates a name for the output file--for now, country+category */
  strcpy(fileName, country);
  strcat(fileName, category);

  /* calls fetchQuery using the constructed URL and filename */
  return fetchQuery(buf, fileName);
}

long setHistoryQuery(const char* country, const char* status) {

  /* uses memstream to construct the API url with country/status */
  FILE *stream;
  char *buf;
  size_t len;
  char fileName[50];
  
  stream = open_memstream(&buf, &len);
  fprintf(stream, "%shistory?country=%s&status=%s", urlBase, country, status);
  fclose(stream);

  /* designates a name for the output file--for now, country+category */
  strcpy(fileName, country);
  strcat(fileName, "history");
  strcat(fileName, status);
 
  /*  calls fetchQuery using the constructed URL and filename */
  return fetchQuery(buf, fileName);
}

char * readFile(const char* fileName) {
  FILE *stream;
  char *buf = 0;
  long len;
  
  if((stream = fopen(fileName, "rb"))) {
	  fseek(stream, 0, SEEK_END);
	  len = ftell(stream);
	  fseek(stream, 0, SEEK_SET);
	  buf = malloc(len);
	  if (buf) {
		fread (buf, 1, len, stream);
	  }
	  fclose(stream);
	  return buf;
  }
  
  return NULL;
}

/*  deletes a file with the name fileName */
void deleteFile(const char* fileName) {
  remove(fileName);
}

char * removeSpaces(char* inputString) {
  const char* d = inputString;
  do {
    while (*d == ' ') {
      ++d;
    }
  } while ((*inputString++ = *d++));
  return inputString;
}