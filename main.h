#pragma once
#include "Supermarket.h"

#define EXIT -1
#define TEXT_FILE_NAME "Customers.txt"
#define BINARY_FILE_NAME "SuperMarket.bin"
typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping,
	ePrintProductByType,eSortProducts,eFindProduct, eNofOptions

} eMenuOptions;

const char* menuStrings[eNofOptions];

int menu();
int initSuperMarketFromFile(SuperMarket* pSuper);