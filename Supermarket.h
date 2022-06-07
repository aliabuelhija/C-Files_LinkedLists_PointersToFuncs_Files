#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
typedef enum { eNone, eName, eBarocde, eType, ePrice, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];
typedef struct
{
	char*		name;
	Address		location;
	Customer*	customerArr;
	int			customerCount;
	Product**	productArr;
	int			productCount;
	eSortOption	sortOpt;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
void	printSuperMarket(const SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
void	doShopping(SuperMarket* pMarket);
void	printProductByType(SuperMarket* pMarket);

void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);


int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
void	fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	sortProducts(SuperMarket* pMarket);
eSortOption showSortMenu();
void findProduct(const SuperMarket* pMarket);

void freeMarket(SuperMarket* pMarket);

int	loadSuperMarketFromFile(SuperMarket* pSuper, const char* fileName);
int	loadCustomersFromFile(SuperMarket* pSuper, const char* fileName);
int	loadAddressFromFile(SuperMarket* pSuper, FILE* fp);

int readProductFromBinary(Product *pProduct, FILE* fp);
int readCustomerFromFile(Customer *pCustomer, FILE* fp);

int	saveSuperMarketToBinaryFile(const SuperMarket* pSuper, const char* fileName);
int saveProductToFile(const Product* pProduct, FILE* fp);
int	saveSuperMarketAddressToBinaryFile(const Address* pAddress,  FILE* fp);

int	saveCustomersToFile(const SuperMarket* Psuper, const char* fileName);
int	saveCustomerToFile(const Customer* pCustomer, FILE* fp);
