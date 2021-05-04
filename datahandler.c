#include "cJSON.h"
#include "cJSON.c"
#include "backend.c"
#include <stdio.h>
#include <malloc.h>

struct CaseData
{
	int confirmedCases;
	double percentInfected;
	double peoplePerKilo;
	double casesPerKilo;
	double avgNewCases;
};

struct VaccineData
{
	int totalVaccinated;
	int totalPartiallyVaccinated;
	double percentVaccinated;
	int herdImmunity;
};

struct RecoveryData
{
	double avgRecoveries;
	double percentImmune;
};

struct DeathData
{
	int totalDeaths;
	double avgDeaths;
};

struct Country
{
	struct CaseData* cases;
	struct VaccineData* vaccines;
	struct RecoveryData* recoveries;
	struct DeathData* deaths;
	struct Country* nextCountry;
	char countryName[50];
};

/* Pointers for the linked list / cache */
struct Country* listStartPointer;
struct Country* listIndexPointer;

/* Averages the values of the last thirty days in a "dates" JSON object provided by the API */
double avgLastThirty(cJSON* datesObject)
{
	cJSON* list;
	int index;
	int total = 0;

	if (datesObject == NULL)
	{
		return 0.0;
	}

	/* The child pointer of a JSON object provides a linked list of its contained values */
	list = datesObject->child;

	/* Parse through 30 entries of the list, which is already in order from the current date going backwards (thanks API),
	   then get the change in cases from the previous day to the current day for each entry, adding them up to the total value */
	for (index = 0; index < 30; index++)
	{
		total += (list->valueint - list->next->valueint);
		list = list->next;
	}

	/* Finally return the average total case increase per day by dividing by the number of increases parsed (30) */
	return total / 30.0;
}

struct CaseData* parseCasesJSON(char* casesJSON, char* casesHistoryJSON)
{
	/* Parse JSON */
	cJSON* casesJsonObject = cJSON_Parse(casesJSON);
	cJSON* historyJsonObject = cJSON_Parse(casesHistoryJSON);
	
	struct CaseData* caseData;

	if (casesJsonObject == NULL || historyJsonObject == NULL)
	{
		printf("Cases or case history JSON response from API was invalid!\n");

		return NULL;
	}

	{
		/* Attempt to fetch main JSON objects */
		cJSON* casesAllObject = cJSON_GetObjectItem(casesJsonObject, "All");
		cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

		if (casesAllObject != NULL && historyAllObject != NULL)
		{
			/* Allocate memory for a new data structure to store the data */
			caseData = (struct CaseData*) malloc(sizeof(struct CaseData));

			{
				/* Fetch JSON objects of relevant data */
				cJSON* confirmed = cJSON_GetObjectItem(casesAllObject, "confirmed");
				cJSON* population = cJSON_GetObjectItem(casesAllObject, "population");
				cJSON* area = cJSON_GetObjectItem(casesAllObject, "sq_km_area");
				cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");

				/* Pre-calculate statistics and add them to the data structure */
				caseData->confirmedCases = confirmed->valueint;
				caseData->percentInfected = (confirmed->valuedouble / population->valuedouble) * 100;
				caseData->peoplePerKilo = population->valuedouble / area->valuedouble;
				caseData->casesPerKilo = confirmed->valuedouble / area->valuedouble;
				caseData->avgNewCases = avgLastThirty(dates);

				return caseData;
			}
		}
	}

	return NULL;
}

struct VaccineData* parseVaccineJSON(char* vaccineJSON)
{
	/* Parse JSON */
	cJSON* vaccinesJsonObject = cJSON_Parse(vaccineJSON);
	
	struct VaccineData* vaccineData;

	if (vaccinesJsonObject == NULL)
	{
		printf("Vaccine JSON response from API was invalid!\n");

		return NULL;
	}

	{
		/* Attempt to fetch main JSON object */
		cJSON* vaccinesAllObject = cJSON_GetObjectItem(vaccinesJsonObject, "All");

		if (vaccinesAllObject != NULL)
		{
			/* Allocate memory for a new data structure to store the data */
			vaccineData = (struct VaccineData*) malloc(sizeof(struct VaccineData));

			{
				/* Fetch JSON objects of relevant data */
				cJSON* vaccinated = cJSON_GetObjectItem(vaccinesAllObject, "people_vaccinated");
				cJSON* partially_vaccinated = cJSON_GetObjectItem(vaccinesAllObject, "people_partially_vaccinated");
				cJSON* population = cJSON_GetObjectItem(vaccinesAllObject, "population");
				
				/* Pre-calculate statistics and add them to the data structure */
				vaccineData->totalVaccinated = vaccinated->valueint;
				vaccineData->totalPartiallyVaccinated = partially_vaccinated->valueint;
				vaccineData->percentVaccinated = (vaccinated->valuedouble / population->valuedouble) * 100;
				vaccineData->herdImmunity = (population->valueint * 2) - (vaccinated->valueint * 2) - partially_vaccinated->valueint;

				return vaccineData;
			}
		}
	}

	return NULL;
}

struct RecoveryData* parseRecoveryJSON(char* recoveredHistoryJSON, struct VaccineData* vaccineData)
{
	/* Parse JSON */
	cJSON* historyJsonObject = cJSON_Parse(recoveredHistoryJSON);
	
	struct RecoveryData* recoveryData;

	if (historyJsonObject == NULL || vaccineData == NULL)
	{
		printf("Recovery history JSON response from API or provided vaccine data was invalid!\n");

		return NULL;
	}

	{
		/* Attempt to fetch main JSON object */
		cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

		if (historyAllObject != NULL)
		{
			/* Allocate memory for a new data structure to store the data */
			recoveryData = (struct RecoveryData*) malloc(sizeof(struct RecoveryData));

			{
				/* Fetch JSON objects of relevant data */
				cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");
				cJSON* population = cJSON_GetObjectItem(historyAllObject, "population");

				/* Pre-calculate statistics and add them to the data structure */
				recoveryData->avgRecoveries = avgLastThirty(dates);
				recoveryData->percentImmune = ((((double) vaccineData->totalVaccinated) + dates->child->valuedouble) / population->valuedouble) * 100;

				return recoveryData;
			}
		}
	}

	return NULL;
}

struct DeathData* parseDeathJSON(char* deathsHistoryJSON)
{
	/* Parse JSON */
	cJSON* historyJsonObject = cJSON_Parse(deathsHistoryJSON);
	
	struct DeathData* deathData;

	if (historyJsonObject == NULL)
	{
		printf("Death history JSON response from API was invalid!\n");

		return NULL;
	}

	{
		/* Attempt to fetch main JSON object */
		cJSON* historyAllObject = cJSON_GetObjectItem(historyJsonObject, "All");

		if (historyAllObject != NULL)
		{
			/* Allocate memory for a new data structure to store the data */
			deathData = (struct DeathData*) malloc(sizeof(struct DeathData));

			{
				/* Fetch JSON objects of relevant data */
				cJSON* dates = cJSON_GetObjectItem(historyAllObject, "dates");

				/* Pre-calculate statistics and add them to the data structure */
				deathData->totalDeaths = dates->child->valueint;
				deathData->avgDeaths = avgLastThirty(dates);

				return deathData;
			}
		}
	}

	return NULL;
}

struct Country* fetchCountryData(char countryName[])
{
	struct Country* newCountry;
	
	char* caseData;
	char* caseHistoryData;
	char* recoveredHistoryData;
	char* vaccineData;
	char* deathData;
	
	int fetchAttempt = 0;
	int statusResponse = 0;
	
	char fileName[100];
	
	/* Check if the country data is in the local cache before attempting to use a cache file and then the API */
	if (listStartPointer != NULL)
	{
		listIndexPointer = listStartPointer;

		/* Traverse the cache, if the country name matches and the entry isn't expired, use the cached country */
		while (listIndexPointer != NULL)
		{
			if (strcmp(countryName, listIndexPointer->countryName) != 0)
			{
				return listIndexPointer;
			}

			listIndexPointer = listIndexPointer->nextCountry;
		}
	}
	
file:
	/* There are multiple files per cache entry, so just check the cases file first to see if it exists */
	strcpy(fileName, countryName);
	strcat(fileName, "cases");
	
	caseData = readFile(fileName);

	if(caseData == NULL)
	{
		goto api;
	}
	
	/* Read each file and set the contents to their own individual variables */
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, countryName);
	strcat(fileName, "historyconfirmed");
	
	caseHistoryData = readFile(fileName);
	
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, countryName);
	strcat(fileName, "historyrecovered");
	
	recoveredHistoryData = readFile(fileName);
	
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, countryName);
	strcat(fileName, "vaccines");
	
	vaccineData = readFile(fileName);
	
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, countryName);
	strcat(fileName, "historydeaths");
	
	deathData = readFile(fileName);
	newCountry = (struct Country*) malloc(sizeof(struct Country));
	
	/* Parse file contents for each category and store them in the new Country structure */
	newCountry->cases = parseCasesJSON(caseData, caseHistoryData);
	newCountry->vaccines = parseVaccineJSON(vaccineData);
	newCountry->recoveries = parseRecoveryJSON(recoveredHistoryData, newCountry->vaccines);
	newCountry->deaths = parseDeathJSON(deathData);
	
	strcpy(newCountry->countryName, countryName);
	
	/* Add the file cache data to the local cache */
	if(listStartPointer != NULL)
	{
		listIndexPointer = listStartPointer;
		
		while(listIndexPointer->nextCountry != NULL)
		{
			listIndexPointer = listIndexPointer->nextCountry;
		}
		
		listIndexPointer->nextCountry = newCountry;
	}
	
	return newCountry;
api:
	if(fetchAttempt == 1)
	{
		printf("Failed to utilize API, cannot perform necessary queries...\n");
		
		return NULL;
	}
	
	printf("Querying API for information, this may take a few seconds...\n");

	/* Perform all the necessary API queries, store the responses in files, and then attempt to read the files and add them to the local cache */
	statusResponse = setQuery(countryName, "cases");
	
	if(statusResponse != 200)
	{
		printf("Cases query failed! Check your spelling?\n");
		
		return NULL;
	}
	
	statusResponse = setHistoryQuery(countryName, "confirmed");
	
	if(statusResponse != 200)
	{
		printf("Cases history query failed! Are we being rate limited?\n");
		
		return NULL;
	}
	
	statusResponse = setQuery(countryName, "vaccines");
	
	if(statusResponse != 200)
	{
		printf("Vaccines query failed!Are we being rate limited?\n");
		
		return NULL;
	}
	
	statusResponse = setHistoryQuery(countryName, "recovered");
	
	if(statusResponse != 200)
	{
		printf("Recovered query failed! Are we being rate limited?\n");
		
		return NULL;
	}
	
	statusResponse = setHistoryQuery(countryName, "deaths");
	
	if(statusResponse != 200)
	{
		printf("Deaths query failed! Are we being rate limited?\n");
		
		return NULL;
	}
	
	fetchAttempt = 1;
	
	goto file;
}