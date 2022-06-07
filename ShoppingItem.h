#pragma once


typedef struct
{
	char*		name;
	float		price;
	int			count;
}ShoppingItem;

void	printItem(const void* item);
ShoppingItem*	createItem(const char* name, float price, int count);
void	freeItem(ShoppingItem* pItem);
