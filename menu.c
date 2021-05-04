#include <stdio.h>
#include <string.h>
#include "datahandler.c"
#include <curl/curl.h>

long statusCode;

void displayCaseData(struct Country* country)
{
	printf("Case data for %s\n\n", country->countryName);
	printf("Total confirmed cases: %d\n", country->cases->confirmedCases);
	printf("Percent of population infected: %.2f%%\n\n", country->cases->percentInfected);
	printf("People per kilometer (km) in the country: %.2f\n", country->cases->peoplePerKilo);
	printf("Cases per kilometer (km) in the country: %.2f\n\n", country->cases->casesPerKilo);
	printf("Average new cases per day (past 30 days): %.2f\n", country->cases->avgNewCases);
}
void displayVaccineData(struct Country* country)
{
	printf("Vaccine data for %s\n\n", country->countryName);
	printf("Total people vaccinated: %d\n", country->vaccines->totalVaccinated);
	printf("Total people partially vaccinated: %d\n\n", country->vaccines->totalPartiallyVaccinated);
	printf("Percent of population completely vaccinated: %.2f%%\n", country->vaccines->percentVaccinated);
	printf("Vaccines remaining to achieve herd immunity: %d\n", country->vaccines->herdImmunity);
}
void displayRecoveryData(struct Country* country)
{
	printf("Recovery data for %s\n\n", country->countryName);
	printf("Average recoveries per day (past 30 days): %.2f\n", country->recoveries->avgRecoveries);
	printf("Percent of population immune (vaccine overlap possible): %.2f%%\n", country->recoveries->percentImmune);
}
void displayDeathData(struct Country* country)
{
	printf("Death data for %s\n\n", country->countryName);
	printf("Total deaths: %d\n", country->deaths->totalDeaths);
	printf("Average deaths per day (past 30 days): %.2f\n", country->deaths->avgDeaths);
}

void displayComparedCaseData(struct Country* country, struct Country* country2)
{
	char casesWinner[50];
	char infectionWinner[50];
	
	printf("Case data for %s vs. %s\n\n", country->countryName, country2->countryName);
	printf("Total confirmed cases:\n%d - %d\n\n", country->cases->confirmedCases, country2->cases->confirmedCases);
	printf("Percent of population infected:\n%.2f%% - %.2f%%\n\n", country->cases->percentInfected, country2->cases->percentInfected);
	printf("People per kilometer (km) in the country:\n%.2f - %.2f\n\n", country->cases->peoplePerKilo, country2->cases->peoplePerKilo);
	printf("Cases per kilometer (km) in the country:\n%.2f - %.2f\n\n", country->cases->casesPerKilo, country2->cases->casesPerKilo);
	printf("Average new cases per day (past 30 days):\n%.2f - %.2f\n\n", country->cases->avgNewCases, country2->cases->avgNewCases);
	
	if(country->cases->confirmedCases < country2->cases->confirmedCases)
	{
		strcpy(casesWinner, country->countryName);
	}else
	{
		strcpy(casesWinner, country2->countryName);
	}
	
	if(country->cases->avgNewCases < country2->cases->avgNewCases)
	{
		strcpy(infectionWinner, country->countryName);
	}else
	{
		strcpy(infectionWinner, country2->countryName);
	}
	
	printf("Overall, in current cases, %s is better.\n", casesWinner);
	printf("Overall, in infection rate, %s is better.\n", infectionWinner);
}
void displayComparedVaccineData(struct Country* country, struct Country* country2)
{
	char vaccinesWinner[50];
	
	printf("Vaccine data for %s vs. %s\n\n", country->countryName, country2->countryName);
	printf("Total people vaccinated:\n%d - %d\n\n", country->vaccines->totalVaccinated, country2->vaccines->totalVaccinated);
	printf("Total people partially vaccinated:\n%d - %d\n\n", country->vaccines->totalPartiallyVaccinated, country2->vaccines->totalPartiallyVaccinated);
	printf("Percent of population completely vaccinated:\n%.2f%% - %.2f%%\n", country->vaccines->percentVaccinated, country2->vaccines->percentVaccinated);
	printf("Vaccines remaining to achieve herd immunity:\n%d - %d\n\n", country->vaccines->herdImmunity, country2->vaccines->herdImmunity);
	
	if(country->vaccines->totalVaccinated > country2->vaccines->totalVaccinated)
	{
		strcpy(vaccinesWinner, country->countryName);
	}else
	{
		strcpy(vaccinesWinner, country2->countryName);
	}
	
	printf("Overall, in vaccinations, %s is better.\n", vaccinesWinner);
}
void displayComparedRecoveryData(struct Country* country, struct Country* country2)
{
	char recoveriesWinner[50];
	
	printf("Recovery data for %s vs. %s\n\n", country->countryName, country2->countryName);
	printf("Average recoveries per day (past 30 days):\n%.2f - %.2f\n\n", country->recoveries->avgRecoveries, country2->recoveries->avgRecoveries);
	printf("Percent of population immune (vaccine overlap possible):\n%.2f%% - %.2f%%\n\n", country->recoveries->percentImmune, country2->recoveries->percentImmune);
	
	if(country->recoveries->avgRecoveries > country2->recoveries->avgRecoveries)
	{
		strcpy(recoveriesWinner, country->countryName);
	}else
	{
		strcpy(recoveriesWinner, country2->countryName);
	}
	
	printf("Overall, in recoveries, %s is better.\n", recoveriesWinner);
}
void displayComparedDeathData(struct Country* country, struct Country* country2)
{
	char deathsWinner[50];
	
	printf("Death data for %s vs. %s\n\n", country->countryName, country2->countryName);
	printf("Total deaths:\n%d - %d\n\n", country->deaths->totalDeaths, country2->deaths->totalDeaths);
	printf("Average deaths per day (past 30 days):\n%.2f - %.2f\n\n", country->deaths->avgDeaths, country2->deaths->avgDeaths);
	
	if(country->deaths->avgDeaths < country2->deaths->avgDeaths)
	{
		strcpy(deathsWinner, country->countryName);
	}else
	{
		strcpy(deathsWinner, country2->countryName);
	}
	
	printf("Overall, in deaths, %s is better.\n", deathsWinner);
}

int main()
{
	int option, i;
	char country[100], country2[100];
	struct Country* countryStructure;
	struct Country* secondCountryStructure;
	
	do
	{
		printf("Welcome to the COVID data tracker! Please select an option\n");
		printf("1. View COVID information for a country\n");
		printf("2. Compare COVID information between 2 countries\n");
		printf("3. Exit the program\nPlease select an option: ");
		scanf("%d", &option);
		
		if(option != 1 && option != 2 && option != 3)
		{
		    printf("Invalid option entered!");
		}

		if (option == 1)
		{

		    printf("Enter the name of the country: ");
		    scanf("%s", &country);
			
			/* Attempts to fetch the country data from the cache or API */
			countryStructure = fetchCountryData(country);
			
			if(countryStructure == NULL)
			{
				printf("Failed to fetch country data of country: %s\n", country);
				
				continue;
			}

			do /*Asks for which option user wants*/
			{
				printf("1. Case Data\n");
				printf("2. Vaccine Data\n");
				printf("3. Recovery Data\n");
				printf("4. Death Data\n");
				printf("5. Return to menu\nPick a category: ");
				scanf("%d", &i);

				switch (i)
				{
					case 1:
						displayCaseData(countryStructure);
						break;
					case 2:
						displayVaccineData(countryStructure);
						break;
					case 3:
						displayRecoveryData(countryStructure);
						break;
					case 4:
						displayDeathData(countryStructure);
						break;
					case 5:
						break;
					default:
						printf("Please select option 1-5");
				}
			} while (i != 5);

		}
		
		if (option == 2)
		{
			printf("Enter the name of the first country: ");
			scanf("%s", &country);
			printf("Enter the name of the second country: ");
			scanf("%s", &country2);
			
			/* Attempts to fetch the country data from the cache or API */
			countryStructure = fetchCountryData(country);
			secondCountryStructure = fetchCountryData(country2);
			
			if(countryStructure == NULL || secondCountryStructure == NULL)
			{
				printf("Failed to fetch country data for one of the provided countries!");
				
				continue;
			}

			do /*Asks for which option user wants */
			{
				printf("1. Case Data\n");
				printf("2. Vaccine Data\n");
				printf("3. Recovery Data\n");
				printf("4. Death Data\n");
				printf("5. Return to menu\nPick a category: ");
				scanf("%d", &i);

				switch (i)
				{
					case 1:
						displayComparedCaseData(countryStructure, secondCountryStructure);
						break;
					case 2:
						displayComparedVaccineData(countryStructure, secondCountryStructure);
						break;
					case 3:
						displayComparedRecoveryData(countryStructure, secondCountryStructure);
						break;
					case 4:
						displayComparedDeathData(countryStructure, secondCountryStructure);
						break;
					case 5:
						break;
					default:
						printf("Please select option 1-5");
				}
			} while (i != 5);
		}
	} while (option == 1 || option == 2);


	return 0;
}
