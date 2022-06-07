#pragma once

#include "ShoppingItem.h"
#include "def.h"
#include "list.h"
typedef struct
{

	LIST shoppingCartList;
}ShoppingCart;




void	initCart(ShoppingCart* pCart);
float	getTotalPrice(ShoppingCart* pCart, LIST* list);
ShoppingItem*	getItemByName(ShoppingCart* pCart, LIST* list, const char* name);
int		addItemToCart(ShoppingCart* pCart, LIST* list, const char* name, float price, int count);
void	printShoppingCart(ShoppingCart* pCart);
void	freeShoppingCart(ShoppingCart* pCart);
int compareItems(const char* d1, const void* d2);
NODE* insertItemToList(NODE* pNode, ShoppingItem* pItem);