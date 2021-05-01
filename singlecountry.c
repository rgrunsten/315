#include <stdio.h>
#include <string.h>
#include "backend.c"

void displayCaseData(char country[])
{
    /*
    API call for cases and store in a struct
    Should also calculate statistics and display them here
    If country doesn't exist/API call fails, print error message and return
    */
	printf("Case data for %s\n", country);
}
void displayVaccineData(char country[])
{
    /*
    API call for vaccines and store in a struct
    Should also calculate statistics and display them here
    If country doesn't exist/API call fails, print error message and return
    */
	printf("Vaccine data for %s\n", country);
}
void displayRecoveryData(char country[])
{
    /*
    API call for recovery and store in a struct
    Should also calculate statistics and display them here
    If country doesn't exist/API call fails, print error message and return
    */
	printf("Recovery data for %s\n", country);
}
void displayDeathData(char country[])
{
    /*
    API call for deaths and store in a struct
    Should also calculate statistics and display them here
    If country doesn't exist/API call fails, print error message and return
    */
	printf("Death data for %s\n", country);
}


int main()
{
	int option, i;
	char country[100];
	do
	{
		printf("Welcome to the COVID data tracker! Please select an option\n");
		printf("1. View COVID information for a country\n");
		printf("2. Compare COVID information between 2 countries\n"); /*Option 2 not created yet*/
		printf("3. Exit the program\n");
		scanf("%d", &option);
		
		if(option != 1 && option != 2 && option != 3)
		{
		    printf("Invalid option entered!");
		}

		if (option == 1)
		{

		    printf("Enter the name of the country\n");
		    scanf("%s", &country);

			do /*Asks for which option user wants*/
			{
				printf("1. Case Data\n");
				printf("2. Vaccine Data\n");
				printf("3. Recovery Data\n");
				printf("4. Death Data\n");
				printf("5. Return to menu\n");
				scanf("%d", &i);
				if (i == 1)
				{
					displayCaseData(country);
				}
				else if (i == 2)
				{
					displayVaccineData(country);
				}
				else if (i == 3)
				{
					displayRecoveryData(country);
				}
				else if (i == 4)
				{
					displayDeathData(country);
				}
				else if(i > 5)
				{
					printf("Please select option 1-5");
				}
			} while (i != 5);

		}
		
		
		
		
	} while (option == 1 || option == 2);


	return 0;
}
