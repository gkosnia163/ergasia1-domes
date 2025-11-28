#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define name_length 50
#define EAN_length 14

typedef struct inventory{
    char EAN[EAN_length];
    char name[name_length];
    int quantity;
    double price; 
    struct inventory* next;//sxoliase TODO
}inv;

typedef struct orderItem{
    char EAN[EAN_length];
    int quantity;
    int priority;
    struct orderItem* next;//sxoliase TODO
}orderItem;

//το struct order χρησιμοποιεί προϊόντα τύπου orderItem, καθώς θέλω να αποθηκεύσω
//περισσότερα απο 1 δεδομένα σε κάθε προϊόν, την προτεραιότητα και ποσότητα.
typedef struct order{ 
    int costumer_id;  
    char buisness_name[name_length];
    struct orderItem* items;
    struct order* next;//sxoliase TODO
}order;

typedef struct costumer{
    int costumer_id;
    char buisness_name[name_length];
    int phone[10];
    char address[name_length];
    struct costumer* next; //sxoliase TODO
}costumer;

inv* newInventoryNode(char EAN[EAN_length], char name[], int quantity, double price){ //δημιουργία καινούριου node     
    inv* node = (inv*)malloc(sizeof(inv));
    if(!node) return NULL;

    strcpy(node->EAN, EAN);
    strcpy(node->name, name);               
    node->quantity = quantity;              
    node->price = price;
    node->next = NULL;

    return node;
} 

void insertInventory(inv** head, inv* newInventoryNode){
    if((*head == NULL) && sizeof(newInventoryNode->EAN) < 4 || sizeof(newInventoryNode->EAN) > 13){  //elegxos ama sizeof(EAN) > 4 && < 13 
        perror("insufficient amount of 'EAN'(barcode) digits");
        return;
    }
    
    if(*head == NULL && sizeof(newInventoryNode->EAN) > 4){
        *head = newInventoryNode;
        return;
    }

    inv* temp = *head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = newInventoryNode;
}

void recommend(inv** head, inv* newInventoryNode, char EAN[]){
    int i = 0, counter = 0;
    int found = 0;
    for(i; EAN[i] == '\0'; i++){
        counter++;
    }

    inv* temp = *head;
    while(temp->next != NULL){
        if(strncmp(temp->EAN,EAN,counter)){
            printf("EAN: %s, name: %s, quantity: %d\n",temp->EAN,temp->name,temp->quantity);
            found = 1;
        }
    }
    if(!found) printf("No products found with the given EAN(barcode) code");
}

int main(){


    return 0;   
}