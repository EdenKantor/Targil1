#include "Header.h"

void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);
}

/*
searchName func gets:
List* L= pointer to list
char name[51]= name of product
The func search if the product name is in the list. 
if yes- return pointer of the node, if no - return NULL.
*/
Items* searchName(List* L, char name[51]) 
{
	Items* temp;
	temp = L->head;
	if (L->head == NULL)//list empty
		return NULL;
	while (temp != NULL)
	{
		if (strcmp(name, temp->ProductName) == 0)
		{
			return temp; //product name found, return the node.
		}
		temp = temp->next;
	}
	return NULL; //product name not found, return NULL.
}

/*
DeleteList func gets: 
List* L- list pointer
The func destroy the linked list(free).
*/
void DeleteList(List* L) 
{
	Items* temp;
	while (L->head != NULL)
	{
		temp = L->head;
		L->head = (L->head)->next; //update the head to the next node
		free(temp->ProductName); //free memory allocation of the name
		free(temp); //free the node
	}
	L->tail = NULL;
}
/* (instruction 1)
CreateProducts func gets: 
List *L= list pointer
FILE *f= Manot file pointer 
The func reads from Manot.txt file and builds a list of products in the kitchen.
*/
void CreateProducts(List *L, FILE *f)
{
	char name[51];//product name
	Items* temp;
	temp = (Items*)malloc(sizeof(struct Items)); //create the first node of the list
	if (temp == NULL)
	{
		Error_Msg("Error allocating struct!");
		exit(1);
	}
	//file reading
	while (fscanf(f,"%s %d %d", name, &temp->ProductQuantity, &temp->ProductPrice) != EOF)
	{
		if (searchName(L, name) != NULL) { //check if theres already a product with the same name in the kitchen
			printf("There is already a product with the name %s in the kitchen!\n", name);
			continue;
		}
		if (temp->ProductQuantity < 0) { //check if the quantity is not positive
				printf("The quantity of the product have to be positive!\n");
				continue;
		}
		if (temp->ProductPrice < 0) { //check if the price of the product is not positive
				printf("The price of the product have to be positive!\n");
				continue;
		}
		//product name dynamic allocation
		temp->ProductName = (char*)malloc(strlen(name) + 1); //memory allocation of product name
		if (temp->ProductName == NULL) {
			Error_Msg("Error allocation name");
			free(temp); //free the current temp node that not linked in the list
			DeleteList(L); //free list
			exit(1);
		}
		strcpy(temp->ProductName, name); //copy to struct

		//Insert at the end of a linked list
		temp->next = NULL;
		if (L->head == NULL) //check if the list is empty
			L->head = temp;
		else
			L->tail->next = temp;
		L->tail = temp; //update the tail

		temp = (Items*)malloc(sizeof(struct Items)); //malloc for the next node
		if (temp == NULL)
		{
			DeleteList(L);
			Error_Msg("Error allocating struct!");
			exit(1);
		}
	}
	printf("The kitchen was created\n");
	free(temp); //free the last temp struct
}

/*(instruction 2)
AddItems func gets:
char* ProductName= product name
int Quantity= product quantity
List* L= list of kitchen products pointer 
The func adds quantity to an existing product in the kitchen.
*/
void AddItems(char* ProductName, int Quantity, List* L)
{
	Items *temp;
	if (Quantity < 0) { //check if the quantity is not positive
		printf("The quantity have to be positive!\n");
		return;
	}
	temp = searchName(L, ProductName); //call searchName func to check if theres a product with that name in the kitchen
	if (temp == NULL) { //if theres no product with that name in the kitchen
		printf("There is no product with the name %s in the kitchen!\n", ProductName);
		return;
	}
	else
	{
		temp->ProductQuantity += Quantity; //update the quantity of the product
		printf("%d %s were added to the kitchen\n", Quantity, ProductName);
	}
}
/*(instruction 3)
OrderItem func gets:
int TableNumber= table number
char* ProductName= product name
int Quantity= quantity
List* L=list of kitchen products pointer 
List* TableLst= list of table orders pointer
This func builds a list of orders to table in the resturant or add new order
*/
void OrderItem(int TableNumber, char* ProductName, int Quantity, List* L, List* TableLst)
{
	Items* temp, *TableNode, *Current, *Prev;

	if (TableNumber > NUM) { //if the table number is not above the number of all the table in the resturant
		printf("There is only %d tables in the resturant\n", NUM);
		return;
	}
	temp = searchName(L, ProductName); //call searchName func to check the product exists
	if (temp == NULL) { //if not exists
		printf("We dont have %s, sorry!\n", ProductName);
		return;
	}
	if (Quantity < 0 || Quantity > temp->ProductQuantity) //check if the quantity given is positive or not above the quantity of a product that available in the kitchen
	{
		if (Quantity < 0)
		{
			printf("The quantity have to be positive!\n");
			return;
		}
		if (Quantity > temp->ProductQuantity)
		{
			printf("The ordered quantity is greater than the quantity that available of this product\n");
			return;
		}
	}

	//if the new order is a reorder so it save a current node for deleting an order if needed.
	Current = searchName(TableLst, ProductName); //calls searchName func to check if theres already a product with the same name in the list
	if (Current != NULL) { //check if the table has already ordered this product, if does update the quantity of the existed product
		Current->ProductQuantity += Quantity; //update the quantity of the node (adds the quantity of the new order to the existed product)
		printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber);
		TableLst->quantity = Quantity; //save the last quantity added for the product (the new order)
		TableLst->ExistedProduct = (char*)malloc(strlen(ProductName) + 1); //memory allocation for the name
		if (TableLst->ExistedProduct == NULL) {
			Error_Msg("Error allocation name");
			exit(1);
		}
		strcpy(TableLst->ExistedProduct, ProductName); //save the name of the existed product that update its quantity for the table
		Prev = TableLst->head;
		while (Prev->next != Current)
		{
			Prev = Prev->next;
		}
		Prev->next = Current->next; //disconnect the current node from the link
		Current->next = TableLst->head; //raise the node to the head of the list
		TableLst->head = Current; //update the head
		return;
	}

	//creat and add the new order
	TableNode = (Items*)malloc(sizeof(struct Items)); //malloc of node in the list
	if (TableNode == NULL)
	{
		DeleteList(TableLst);
		Error_Msg("Error allocating struct!");
		exit(1);
	}
	TableNode->ProductPrice = temp->ProductPrice; //adds the price of the product from the product in the kitchen 
	TableNode->ProductQuantity = Quantity; //adds the quantity of the product that was ordered
	TableNode->ProductName = (char*)malloc(strlen(ProductName) + 1); //memory allocation for the name of the meal
	if (TableNode->ProductName == NULL) {
		Error_Msg("Error allocation name");
		DeleteList(TableLst); //free list
		exit(1);
	}
	strcpy(TableNode->ProductName, ProductName); //copy to struct

	if (TableLst->head == NULL) {
		TableLst->head = TableNode;
		TableLst->tail = TableNode;
		TableLst->tail->next = NULL;
	}
	else {
		TableNode->next = TableLst->head;
		TableLst->head = TableNode; //update the head
	}
	temp->ProductQuantity -= Quantity; //update the quantity of the product in the kitchen
	printf("%d %s were added to the table number %d\n", Quantity, ProductName, TableNumber);
}

/*(instruction 4)
RemoveItem func gets:
int TableNumber= table number
List* TableLst= table orders list pointer
The function Cancels last table order (allowing each table to cancel only one-time)
*/
void RemoveItem(int TableNumber, List* TableLst)
{
	Items *temp;
	if (TableLst->head == NULL) {
		printf("The table number %d is not ordered yet\n", TableNumber);
		return;
	}
	if (TableLst->Cancel_Order == true) { //check if the table already returned once
		printf("You can only return a product once!\n");
		return;
	}
	temp = TableLst->head;
	if (TableLst->ExistedProduct != NULL) {
		if (strcmp(temp->ProductName, TableLst->ExistedProduct) == 0) { //check if the last order of the table was an addition to an existed previous order
			temp->ProductQuantity -= TableLst->quantity; //return the last order that was addition to an existed order
			TableLst->Cancel_Order = true; //return a product is only one-time
			printf("%d %s was returned to the kitchen from table number %d\n", TableLst->quantity, temp->ProductName, TableNumber);
			return;
		}
	}
	TableLst->head = TableLst->head->next;
	printf("%d %s was returned to the kitchen from table number %d\n", temp->ProductQuantity, temp->ProductName, TableNumber);
	free(temp->ProductName); //free memory allocation of the name
	free(temp); //free the last order
	TableLst->Cancel_Order = true; //return a product is only one-time
}

/*(instruction 5)
RemoveTable func gets:
int TableNumber= table number
List* TableLst= table orders list pointer.
The func calculate the payment of the table and show the table the bill + tip and summary of their orders, and then delete the list of the table.
*/
void RemoveTable(int TableNumber, List* TableLst)
{
	Items* temp;
	int sum = 0;
	float tip;
	if (TableLst->head == NULL) {
		printf("The table number %d is not ordered yet\n", TableNumber);
		return;
	}
	temp = TableLst->head;
	while (temp != NULL)
	{
		printf("%d %s. ", temp->ProductQuantity, temp->ProductName);
		sum =sum+ (temp->ProductPrice*temp->ProductQuantity); //sum the price of the products
		temp = temp->next;
	}
	tip = (float)sum * 0.15; //calculate the tip
	printf("%d nis+%g nis for tips, please!\n", sum, tip);
	DeleteList(TableLst); //delete the list of products of the table
	TableLst->Cancel_Order = false;
}