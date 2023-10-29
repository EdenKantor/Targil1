#ifndef HEADER_H
#define HEADER_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define NUM 50
typedef struct Items    //declaration of node type
{
	struct Items* next; 
	char* ProductName; //product name
	int ProductPrice; // product price
	int ProductQuantity; // product Quantity
}Items;

typedef struct List //manager struct that manage the list
{
	struct Items* head;
	struct Items* tail;
	int quantity; // quantity for reorder product
	char* ExistedProduct; // name of reorder product
	bool Cancel_Order; //if order was canceld 
}List;

//functions declarations
void CreateProducts(List *, FILE *); //instruction 1
void AddItems(char* ProductName, int Quantity, List* L);//instruction 2
void OrderItem(int TableNumber, char* ProductName, int Quantity, List* L, List* TableLst);//instruction 3
void RemoveItem(int TableNumber, List* TableLst);//instruction 4
void RemoveTable(int TableNumber, List* TableLst);//instruction 5
Items* searchName(List* L, char name[51]);
void DeleteList(List* L);
void Error_Msg(char* msg);

#endif