#pragma once

typedef enum {eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType} eProductType;

const char* typeStr[eNofProductType];

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7
typedef struct
{
	char		name[NAME_LENGTH+1];
	char		barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float		price;
	int			count;
}Product;

void	initProduct(Product* pProduct);
void	initProductNoBarcode(Product* pProduct);
void	initProductName(Product* pProduct);
void	printProductV(const void* val);
void	printProduct(const Product* pProduct);

void	getBorcdeCode(char* code);
int		isProduct(const Product* pProduct,const char* barcode);
eProductType getProductType();
const char*	getProductTypeStr(eProductType type);

void	updateProductCount(Product* pProduct);
void	freeProduct(Product* pProduct);

int	compareProductByName(const void* Product1, const void* Product2);
int	compareProductByBarcode(const void* Product1, const void* Product2);
int	compareProductByType(const void* Product1, const void* Product2);
int	compareProductByPrice(const void* Product1, const void* Product2);