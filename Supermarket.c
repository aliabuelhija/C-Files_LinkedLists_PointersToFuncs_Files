#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"
#include "fileHelper.h"
#define BINARY_FILE_NAME "SuperMarket.bin"

static const char* sortOptStr[eNofSortOpt] = {
	"None","Name", "Barcode" , "Type", "Price" };


int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->name = getStrExactLength("Enter market name");
	initAddress(&pMarket->location);
	return 1;
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket,barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if(!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket,const char* barcode)
{
	pMarket->productArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pMarket->productArr)
		return 0;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	strcpy(pProd->barcode, barcode);
	initProductNoBarcode(pProd);
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
		return 0;

	if (!initCustomer(&pMarket->customerArr[pMarket->customerCount]))
		return 0;

	pMarket->customerCount++;
	return 1;
}

void	doShopping(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return;
	}
	
	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return;
	}
	
	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return;
	}
	
	
	ShoppingCart cart;
	initCart(&cart);
	fillCart(pMarket,&cart);
	printf("---------- Shopping ended ----------\n");
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(&cart);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(&cart);
	printf("---------- Shopping  ----------\n");
}


void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	generalArrayFunction((void*)pMarket->productArr, pMarket->productCount, sizeof(Product*), printProductV);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer), printCustomerV);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart,&pCart->shoppingCartList, pProd->name, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count < 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
	

	generalArrayFunction((void*)pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomerV);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBorcdeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;

}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}
void	sortProducts(SuperMarket* pMarket){
	pMarket->sortOpt = showSortMenu();
	int(*compare)(const void* pProduct1, const void* pProduct2) = NULL;

	switch (pMarket->sortOpt)
	{
	case eName:
		compare = compareProductByName;	
		break;
	case eBarocde:
		compare = compareProductByBarcode;
		break;
	case eType:
		compare = compareProductByType;
		break;
	case ePrice:
		compare = compareProductByPrice;
		break;

	}

	if (compare != NULL)
		qsort(pMarket->productArr,pMarket->productCount, sizeof(Product**), compare);
}
eSortOption showSortMenu()
{
	int opt;
	printf("select the option you want the sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}
void	findProduct(const SuperMarket* pMarket) {
	int(*compare)(const void* product1, const void* product2) = NULL;
	Product p = { 0 };
	Product* product = &p;

	switch (pMarket->sortOpt)
	{
	case eName:
		printf("Enter product name\t");
		scanf("%s", &p.name);
		compare = compareProductByName;
		break;

	case eBarocde:
		printf("Enter product barcode\t");
		scanf("%s", &p.barcode);
		compare = compareProductByBarcode;
		break;

	case eType:
		p.type = getProductType();
		compare = compareProductByType;
		break;

	case ePrice:
		printf("Enter product price\n");
		scanf("%f", &p.price);
		compare = compareProductByPrice;
		break;
	}
	if (compare != NULL)
	{
		Product** pP = bsearch(&product, pMarket->productArr, pMarket->productCount, sizeof(Product*), compare);
		if (pP == NULL)
			printf("Product was not found\n");
		else {
			printf("Product found, ");
			printProduct(*pP);
		}

	}
	else {
		printf("the array not sorted , we can't do search without sort  \n");
	}
}
int		loadSuperMarketFromFile(SuperMarket* pSuper, const char* fileName) {
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open file\n");
		return 0;
	}
	pSuper->productArr = NULL;
	pSuper->name = readStringFromFile(fp, "error loading supermarket name ");
	if (!pSuper->name)
	{
		fclose(fp);
		return 0;
	}
	if (!loadAddressFromFile(pSuper, fp)) {
		fclose(fp);
		return 0;
	}
	
	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
	{
		fclose(fp);
		return 0;
	}
	pSuper->sortOpt = (eSortOption)opt;
	if (!readIntFromFile(&pSuper->productCount, fp, "Error reading product\n"))
	{
		fclose(fp);
		return 0;
	}

	if (pSuper->productCount > 0)
	{
		pSuper->productArr = (Product**)malloc(pSuper->productCount * sizeof(Product*));
		if (!pSuper->productArr)
		{
			printf("Alocation error\n");
			fclose(fp);
			return 0;
		}
	}
	else
		pSuper->productArr = NULL;
	for (int i = 0; i < pSuper->productCount; i++)
	{
		pSuper->productArr[i] = (Product*)calloc(1, sizeof(Product));
		if (!pSuper->productArr[i])
		{
			printf("Alocation error\n");
			free(pSuper->productArr);
			fclose(fp);
			return 0;
		}
		if (!readProductFromBinary(pSuper->productArr[i],fp))
		{
			free(pSuper->productArr);
			fclose(fp);
			return 0;
		}
	}
	printf("data loaded from %s\n", fileName);
	fclose(fp);
	return 1;
}
int		loadAddressFromFile(SuperMarket* pSuper,FILE* fp) {
	readIntFromFile(&pSuper->location.num, fp, "error loading supermarket num ");
	if (!pSuper->location.num)
	{
		fclose(fp);
		return 0;
	}
	pSuper->location.street = readStringFromFile(fp, "error loading supermarket street ");
	if (!pSuper->location.street)
	{
		fclose(fp);
		return 0;
	}
	pSuper->location.city = readStringFromFile(fp, "error loading supermarket city ");
	if (!pSuper->location.city)
	{
		fclose(fp);
		return 0;
	}
	return 1;
}
int readProductFromBinary(Product *pProduct, FILE* fp) {
	int nameLen, barcodeLen;
	if (fread(&nameLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fread(pProduct->name, sizeof(char), nameLen, fp) != nameLen) {
		return 0;
	}
	if (fread(&barcodeLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fread(pProduct->barcode, sizeof(char), barcodeLen, fp) != barcodeLen) {
		return 0;
	}
	if (fread(&pProduct->type, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fread(&pProduct->price, sizeof(float), 1, fp) != 1) {
		return 0;
	}
	if (fread(&pProduct->count, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	return 1;
}
int		loadCustomersFromFile(SuperMarket* pSuper, const char* fileName) {
	FILE* fp;

	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf("Error open customers file\n");
		return 0;
	}
	fscanf(fp, "%d", &pSuper->customerCount);
	fgetc(fp);
	pSuper->customerArr = (Customer*)malloc(pSuper->customerCount * sizeof(Customer));
	if (!pSuper->customerArr)
		return 0;
	for (int i = 0; i < pSuper->customerCount; i++)
	{
		if (!readCustomerFromFile(&pSuper->customerArr[i], fp))
		{
			printf("Error loading customer from file\n");
			fclose(fp);
			return 0;
		}
	}

	fclose(fp);
	return 1;

}
int readCustomerFromFile(Customer *pCustomer, FILE* fp){
	char temp[MAX_STR_LEN];
	if (!pCustomer)
		return 0;

	myGetsFromFile(temp, MAX_STR_LEN, fp);
	pCustomer->name = getDynStr(temp);


	return 1;

}
int	saveSuperMarketToBinaryFile(const SuperMarket* pSuper, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (writeStringToFile(pSuper->name, fp, "error entering the name") != 1) {
		return 0;
	}

	if (saveSuperMarketAddressToBinaryFile(&pSuper->location, fp) != 1) {
		return 0;
	}

	if (writeIntToFile((int)pSuper->sortOpt, fp, "error entering the sort type" )!= 1) {
		return 0;
	}

	if (writeIntToFile(pSuper->productCount, fp, "error entering product number") != 1) {
		return 0;
	}
	for (int i = 0; i < pSuper->productCount; i++)
	{
		if (!saveProductToFile(pSuper->productArr[i], fp))
		{
			fclose(fp);
			return 0;
		}
	}
	return 1;
}

int	saveSuperMarketAddressToBinaryFile(const Address* pAddress,  FILE* fp)
{
	if (!pAddress || !fp) {
		fclose(fp);
		return 0;
	}
	if (fwrite(&pAddress->num, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	int cityLen, streetLen;
	cityLen = (int)strlen(pAddress->city) + 1;
	streetLen = (int)strlen(pAddress->street) + 1;

	if (fwrite(&streetLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(pAddress->street, sizeof(char), streetLen, fp) != streetLen) {
		return 0;
	}

	if (fwrite(&cityLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(pAddress->city, sizeof(char), cityLen, fp) != cityLen) {
		return 0;
	}
	return 1;
}
int saveProductToFile(const Product* pProduct, FILE* fp)
{
	if (!pProduct || !fp) {
		return 0;
	}
	int nameLen, barcodeLen;
	nameLen = NAME_LENGTH + 1;
	barcodeLen = BARCODE_LENGTH + 1;
	if (fwrite(&nameLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(pProduct->name, sizeof(char), nameLen, fp) != nameLen) {
		return 0;
	}
	if (fwrite(&barcodeLen, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(pProduct->barcode, sizeof(char), barcodeLen, fp) != barcodeLen) {
		return 0;
	}
	if (fwrite(&(int)pProduct->type, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(&pProduct->price, sizeof(float), 1, fp) != 1) {
		return 0;
	}
	if (fwrite(&pProduct->count, sizeof(int), 1, fp) != 1) {
		return 0;
	}
	return 1;
}


int	saveCustomersToFile(const SuperMarket* Psuper, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "w");
	if (!fp) {
		printf("Error open file to write\n");
		return 0;
	}

	fprintf(fp, "%d\n", Psuper->customerCount);
	for (int i = 0; i < Psuper->customerCount; i++)
	{
		if (!saveCustomerToFile(&Psuper->customerArr[i],fp))
		{
			printf("Error write customer to file\n");
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;

}

int		saveCustomerToFile(const Customer* pCustomer, FILE* fp)
{
	if (!pCustomer)
		return 0;
	fprintf(fp, "%s\n", pCustomer->name);

	return 1;
}