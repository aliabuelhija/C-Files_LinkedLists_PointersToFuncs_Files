#pragma once

typedef struct
{
	char*	name;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);
void	printCustomerV(const void* val);

int		isCustomer(const Customer* pCust, const char* name);

void	freeCustomerV(void* val);
void	freeCustomer(Customer* pCust);



