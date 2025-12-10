#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define name_length 50
#define EAN_maxSize 14

typedef struct inventory{
    char EAN[EAN_maxSize];
    char name[name_length];
    int quantity;
    double price; 
    struct inventory* next;//sxoliase TODO
}inv;

typedef struct order{
    char EAN[EAN_maxSize];
    int quantity;
    int priority;
    struct order* next;//sxoliase TODO
}order;

//το struct order χρησιμοποιεί προϊόντα τύπου orderItem, καθώς θέλω να αποθηκεύσω
//περισσότερα απο 1 δεδομένα σε κάθε προϊόν, την προτεραιότητα και ποσότητα.
typedef struct orderComp{ 
    int costumer_id;  
    char buisness_name[name_length];
    order* items;
    struct orderComp* next;//sxoliase TODO
}orderComp;

//extra monada
typedef struct costumer{
    int costumer_id;
    char buisness_name[name_length];
    int phone[10];
    char address[name_length];
    struct costumer* next; //sxoliase TODO
}costumer;

inv* newInventoryNode(char EAN[], char name[], int quantity, double price){ 
    inv* node = (inv*)malloc(sizeof(inv));
    if(!node) return NULL;

    strcpy(node->EAN, EAN);
    strcpy(node->name, name);               
    node->quantity = quantity;              
    node->price = price;
    node->next = NULL;

    return node;
} 

order* newOrderNode(char EAN[], int quantity, int priority){
    order* node = (order*)malloc(sizeof(order));
    if(!node) return NULL;
    
    strcpy(node->EAN, EAN);
    node->quantity = quantity;              
    node->priority = priority;
    node->next = NULL;

    return node;
}

//χρησιμοποιείται στην insertInventory σε περίπτωση που ο χρήστης εισάγει 4-13 ψηφεία
//επρεπε να μπει πίσω απο insert, αλλιώς δεν την διάβαζε.
void recommend(inv** head, char EAN[]){ 
    int found = 0;
    size_t counter = strlen(EAN);

    inv* temp = *head;                         //αρχικοποίηση temp στο πρώτο node της λίστας
    while(temp != NULL){ //proteinie edw lathos den kserwz
        if(strncmp(temp->EAN,EAN,counter) == 0){
            printf("EAN: %s, name: %s, quantity: %d\n",temp->EAN,temp->name,temp->quantity);
            found = 1;
        }
        temp = temp -> next;
    }
    if(!found) printf("No products found with the given EAN(barcode) code");
}

void insertInventory(inv** invHead, char EAN[], char name[], int quantity, double price){ //δεν είναι σκέτη insert, κάνει update και ήδη υπάρχοντα προϊόντα.
    size_t eanLength = strlen(EAN);
    if(eanLength < 4 || eanLength > 13){      //Γενικός έλεγχος ψηφίων ΕΑΝ
        perror("insufficient amount of EAN/barcode digits");
        return;
    }
    
    inv* temp = *invHead;                      //αρχικοποίηση στο πρώτο node της λίστας
    while(temp != NULL){                       //αναζήτηση ήδη υπαρκτής ΕΑΝ
        if(strcmp(temp->EAN, EAN) == 0){       //αν υπάρχει
            temp->quantity += quantity;        //αυξάνει την ποσότητα μόνο
            return;
        }
        temp = temp->next;
    }

    //περίπτωση που δεν πληρούνται τα ψηφία όλα, αλλα υπάρχουν αρκετά για recommendation
    if(eanLength < 13){ 
        printf("\nProducts with similar EAN/barcode\n");
        recommend(invHead, EAN);
    }

    //περίπτωση που πληρούνται όλες οι προϋποθέσεις και δημιουργήτε νέο node (προϊόν), τύπου
    //inv* με την βοήθεια της newInventoryNode συνάρτησης    
    else if(eanLength == 13) { 
        inv* node = newInventoryNode(EAN, name, quantity, price);

        if(*invHead == NULL){                   //έλεγχος αμα το head βρίσκεται στο τέλος της λίστας,
            *invHead = node;
        }else{
            temp = *invHead;
            while(temp->next != NULL){          //μετατόπιση head στο τέλος της λίστας
                temp = temp->next;
            }            
            temp->next = node;                  //εισαγωγή νέου node
        }
        printf("Product : '%s' (EAN: %s), added to inventory\n", name, EAN);
    }
}

/*θα προσθέτουμε τα προϊόντα παραγγελίας με όνομα αντί να γράφει ο χρήστης 14ψήφιους αριθμούς*/
void newOrder(order** orderHead, inv* invHead, char name[],int quantity, int priority){
    inv* temp = invHead;
    while(temp != NULL){
        if(strcmp(name, temp->name)){
            order* node = newOrderNode(temp->EAN, quantity, priority); //δημιουργία node, 
            if(*orderHead == NULL){             //έλεγχος αν το head βρίσκεται στο τέλος της λίστας
                *orderHead = node;              //εισαγωγή νέου node στη λίστα
            }else{                                  
                while(node->next != NULL){  
                    node = node->next;          //μετατόπιση του head (του order) στο τέλος της λίστας
                }                      
                node->next = node;              //εισαγωγή νέου node στη λίστα
            }             
            printf("product added to basket: %s", temp->name);
            return;
        }    
        temp = temp->next;
    }
}

/*-  Εκτύπωση αποθέματος (δέχεται είτε κωδικό ή μέρος του είτε όνομα είτε * για 
εκτύπωση όλου του αποθέματος).  */
void print(inv** invHead, char choice[name_length]){
    inv* temp = *invHead;
    if(strcmp(choice, "*") == 0){
        temp = *invHead;
        while(temp != NULL){
            printf("Product: %s, %s, quantity: %d, price: %lf\n", temp->name, temp->EAN, temp->quantity,  temp->price);
            temp = temp->next;
        }
        return;
    }else{
        while(temp != NULL){
            if(strcmp(choice, temp->EAN) == 0 || strcmp(choice, temp->name) == 0){
                printf("Product: %s, quantity: %d, price: %.2lf\n%s\n",temp->name, temp->quantity, temp->price, temp->EAN);
            }
            else if(strncmp(choice, temp->EAN, strlen(choice)) == 0){
                recommend(&temp, choice);
            }
            temp = temp->next;
        }
        return;
    }///////////////////////////////////////////////////////////////this shit needs fixing
}

int main(){     
    printf("welcome!\n");
    inv* invHead = NULL;
    order* orderHead = NULL;
    char line[100]; //= {"nothing yet"};
    char word[20];

    while(fgets(line,sizeof(line), stdin) != NULL){

        sscanf(line, "%s", word);
        if(strcmp(word, "insert") == 0){
            char ean[EAN_maxSize];
            char name[name_length];
            int quantity;
            double price;
            
            //bres allo solution anti gia *s
            if(sscanf(line,"%*s %s %s %d %lf", ean, name, &quantity, &price) == 4){ //χρειάστηκε μια extra δήλωση τύπου δεδομένων *s για να προσπεράσουμε την πρώτη λέξη 'insert'
                insertInventory(&invHead, ean, name, quantity, price);
            }else printf("Wrong insert format, try: 'insert <barcode> <name> <quantity> <price>'\n");
        
        }else if(strcmp(word, "order") == 0){
            char name[name_length];
            int quantity;
            int priority;

            if(sscanf(line, "%*s %s %d %d",name, &quantity, &priority) == 3){
                newOrder(&orderHead, invHead, name, quantity, priority);
            }else printf("Wrong order format, try: 'order <name> <quantity> <priority>'\n");
        
        }else if (strcmp(word, "print") == 0){
            char choice[name_length];   
            sscanf(line, "%*s %s",choice);
            print(&invHead, choice);        //συνάρτηση εκτύπωσης επιλογών 
        
        }else if(strcmp(word, "exit") == 0) {
            break;
        
        }else if(strcmp(word, "help") == 0){
            printf("Command list:\n\n\t- insert <barcode> <name> <quantity> <price>\n\t- print\n\t- exit");
        }else{ printf("Cannot recognise command, type help\n");} // kanto kltro grapse sxolia TODO
    }

    return 0;   
}