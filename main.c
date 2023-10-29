#include "Header.h"

int main()
{
    List inventory, TableArr[NUM]; //kitchen inventory list, tables arr. 
    int TableNumber, Quantity, value, i;
    char ProductName[NUM + 1] = { 0 };
    FILE* manot, * instruct;
    bool Print = false;

    //Files pointers
    manot = fopen("Manot.txt", "rt");
    if (manot == NULL)
        Error_Msg("input file is wrong");
    instruct = fopen("Instructions.txt", "rt");
    if (instruct == NULL)
        Error_Msg("input file is wrong");

    //Initialization:
    for (i = 0; i < NUM; i++)
    {
        TableArr[i].head = NULL;
        TableArr[i].tail = NULL;
        TableArr[i].ExistedProduct = NULL;
    }
    inventory.ExistedProduct = NULL;

    //if the first instruct not 1- print to user until change.
    if (fscanf(instruct, "%d", &value) == 0)
        printf("ScanF ERROR!");
    while (value != 1)
    {
        if (Print != true) {
            printf("The first instruct must be 1!\n");
            Print = true;}
        fseek(instruct, 0, SEEK_SET);
        if (fscanf(instruct, "%d", &value) == 0)
            printf("ScanF ERROR!");
    }
    
        inventory.head = NULL;
        inventory.tail = NULL;
        CreateProducts(&inventory, manot);//call to instruct(1) function
    
    
    //while to the end of instructions file. 
    while (fscanf(instruct, "%d", &value) != EOF)//read the instruct
    {
        switch (value)
        {
        case 2:
            if (fscanf(instruct, "%s %d", ProductName, &Quantity) < 2)
                printf("ScanF ERROR!");
            AddItems(ProductName, Quantity, &inventory);
            break;
        case 3:
            if (fscanf(instruct, "%d %s %d", &TableNumber, ProductName, &Quantity) < 3)
                printf("ScanF ERROR!");
            OrderItem(TableNumber, ProductName, Quantity, &inventory, &TableArr[TableNumber]);
            break;
        case 4:
            if (fscanf(instruct, "%d", &TableNumber) != 1)
                printf("ScanF ERROR!");
            RemoveItem(TableNumber, &TableArr[TableNumber]);
            break;
        case 5:
            if (fscanf(instruct, "%d", &TableNumber) != 1)
                printf("ScanF ERROR!");
            RemoveTable(TableNumber, &TableArr[TableNumber]);
            break;
        default:
            break;
        }
    }

    //closing files pointer and free all memory.
    fclose(manot);
    fclose(instruct);
    for (i = 0; i < NUM; i++)
    {
        DeleteList(&TableArr[i]); //destroy all tables list
    }
    DeleteList(&inventory); //destroy kitchen list
    if(inventory.ExistedProduct!=NULL)
        free(inventory.ExistedProduct);
    
    return 0;
}
        
