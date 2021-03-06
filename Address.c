#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Address.h"
#include "General.h"

#define WORD_SEP "#"

int	initAddress(Address* pAd)
{
	char allData[MAX_STR_LEN];
	char** elements;
	int count;
	int ok = 1;
	do {
		getsStrFixSize(allData, sizeof(allData), 
			"Enter address data\nFormat: street!house number!city\nstreet and city can have spaces\n");
		elements = splitCharsToWords(allData, &count, "!");
		ok = checkElements(elements, count);
		if (!ok)
			freeElements(elements, count);
	} while (!ok);

	//get the house number
	sscanf(elements[1], "%d", &pAd->num);
	pAd->street = fixAddressParam(elements[0]);
	if (!pAd->street)
	{
		freeElements(elements, count);
		return 0;
	}

	pAd->city = fixAddressParam(elements[2]);
	if (!pAd->city)
	{
		free(pAd->street);
		freeElements(elements, count);
		return 0;
	}

	freeElements(elements, count);
	return 1;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWordsBySpace(param, &count, &totalLength);
	//add size for the WORD_SEP between words and for '\0'
	fixParamStr = (char*)calloc(totalLength + count, sizeof(char));
	if (!fixParamStr)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	int len;
	for (int i = 0; i < count; i++)
	{
		len = (int)strlen(wordsArray[i]);
		wordsArray[i][0] = toupper(wordsArray[i][0]); //upper
		for(int j = 1; j < len-1; j++)
		{
			wordsArray[i][j] = tolower(wordsArray[i][j]); //small
		}
		if(count == 1)
			wordsArray[i][len-1] = toupper(wordsArray[i][len - 1]); //upper
		else
			wordsArray[i][len - 1] = tolower(wordsArray[i][len - 1]); //small

		strcat(fixParamStr, wordsArray[i]);
		if (i != count - 1) //not last
			strcat(fixParamStr, WORD_SEP);
	}

	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return fixParamStr;

}


int checkElements(char**  elements, int count)
{
	if (count != 3)
		return 0;
	//check second word is a number;
	char* streetNum = elements[1];
	while (*streetNum)
	{
		if (!isdigit(*streetNum))
			return 0;
		streetNum++;
	}
	return 1;
}

void freeElements(char**  elements, int count)
{
	for (int i = 0; i < count; i++)
		free(elements[i]);
	free(elements);
}

void printAddress(const Address* pAd)
{
	printf("%s %d, %s\n", pAd->street, pAd->num, pAd->city);
}

void	freeAddress(Address* pAd)
{
	free(pAd->city);
	free(pAd->street);
}