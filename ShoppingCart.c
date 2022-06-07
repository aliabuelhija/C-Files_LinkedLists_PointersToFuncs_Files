#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "ShoppingCart.h"



void	initCart(ShoppingCart* pCart)
{
	L_init(&pCart->shoppingCartList);
}
float	getTotalPrice(ShoppingCart* pCart,LIST* list)
{
	float price = 0;
	NODE* pNode = &(list->head);
	ShoppingItem* pItem;

	while (pNode->next!=NULL) {
		pItem = (ShoppingItem*)pNode->next->key;
		price = price+(pItem->price* pItem->count);
		pNode = pNode->next;
	}
	return price;
}

int		addItemToCart(ShoppingCart* pCart, LIST* list, const char* name, float price, int count)
{
	ShoppingItem* pItem = getItemByName(pCart, list, name);
	NODE* pNode = &(list->head);

	if (pItem == NULL)
	{
		pItem = createItem(name, price, count);
		if (!pItem)
			return 0;
	}
	else {
	pItem->count += count;
	return 1;
	}
	int	compareRes = 0;
	while (pNode->next != NULL) {
		compareRes = compareItems(pItem->name, pNode->next->key);
		if (compareRes <= 0)
			break;
		pNode = pNode->next;
	}
	if (compareRes < 0 || pNode->next == NULL)
		pNode = insertItemToList(pNode, pItem);


	return 1;
}

void	printShoppingCart( ShoppingCart* pCart)
{
	float price = getTotalPrice(pCart, &pCart->shoppingCartList);

	L_print(&pCart->shoppingCartList,printItem);

	printf("Total bill to pay: %.2f\n", price);
}



ShoppingItem*	getItemByName(ShoppingCart* pCart,LIST* list, const char* name)
{

		NODE* pNode = &(list->head);

	while (pNode->next!=NULL) {
		if (compareItems(name,pNode->next->key)==0) {
			return (ShoppingItem*)pNode->next->key;
		}
		pNode = pNode->next;
	}
	return NULL;
}

void	freeShoppingCart(ShoppingCart* pCart)
{
	L_free(&pCart->shoppingCartList,freeItem);
}
int compareItems(const char* d1, const void* d2) {
	const ShoppingItem* pItem2 = (const ShoppingItem*)d2;
	if (strcmp(d1, pItem2->name) == -1)
		return -1;
	if (strcmp(d1, pItem2->name) == 0)
		return 0;
	return 1;
}
NODE* insertItemToList(NODE* pNode, ShoppingItem* pItem)
{
	ShoppingItem* pNewItem = (ShoppingItem*)malloc(sizeof(ShoppingItem));
	if (!pNewItem)
		return NULL;
	*pNewItem = *pItem;

	return  L_insert(pNode, pNewItem);
}