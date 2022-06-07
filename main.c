#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"


int menu();



const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping",
								"Print Product By Type","Sort Products","Find Product"};

int main()
{
	SuperMarket	market;
	int option;
	int stop = 0;
	if (initSuperMarketFromFile(&market) ==0 )
	{
		initSuperMarket(&market);
	}
	loadCustomersFromFile(&market,TEXT_FILE_NAME);
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;


		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			doShopping(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;
		case eSortProducts:
			sortProducts(&market);
			break;
		case eFindProduct:
			findProduct(&market);
			break;
		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	saveSuperMarketToBinaryFile(&market, BINARY_FILE_NAME);
	saveCustomersToFile(&market,TEXT_FILE_NAME);
	freeMarket(&market);

	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initSuperMarketFromFile(SuperMarket* pSuper) {
	if (loadSuperMarketFromFile(pSuper, BINARY_FILE_NAME)) {
		return 1;
	}
	return 0;
}