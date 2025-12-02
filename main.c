#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define name_length 50
#define EAN_maxSize 14

typedef struct inventory{
    char EAN[EAN_maxSize];
    char name[name_length];
    int quantity;
    double price; 
    struct inventory* next;//sxoliase TODO
}inv;

typedef struct orderItem{
    char EAN[EAN_maxSize];
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

inv* newInventoryNode(char EAN[], char name[], int quantity, double price){ //δημιουργία καινούριου node     
    inv* node = (inv*)malloc(sizeof(inv));
    if(!node) return NULL;

    strcpy(node->EAN, EAN);
    strcpy(node->name, name);               
    node->quantity = quantity;              
    node->price = price;
    node->next = NULL;

    return node;
} 

//χρησιμοποιείται στην insertInventory σε περίπτωση που ο χρήστης εισάγει 4-13 ψηφεία
//επρεπε να μπει πίσω απο insert, αλλιώς δεν την διάβαζε.
void recommend(inv** head, char EAN[]){ 
    int found = 0;
    size_t counter = strlen(EAN);

    inv* temp = *head; //αρχικοποίηση temp στο πρώτο node της λίστας
    while(temp != NULL){ //proteinie edw lathos den kserwz
        if(strncmp(temp->EAN,EAN,counter) == 0){
            printf("EAN: %s, name: %s, quantity: %d\n",temp->EAN,temp->name,temp->quantity);
            found = 1;
        }
        temp = temp -> next;
    }
    if(!found) printf("No products found with the given EAN(barcode) code");
}

void insertInventory(inv** head, char EAN[], char name[], int quantity, double price){ //δεν είναι σκέτη insert, κάνει update και ήδη υπάρχοντα προϊόντα.
    size_t eanLength = strlen(EAN);//kltra bale size_t
    if(eanLength < 4 || eanLength > 13){  //Γενικός έλεγχος ψηφίων ΕΑΝ
        perror("insufficient amount of EAN/barcode digits");
        return;
    }
    
    inv* temp = *head; //αρχικοποίηση στο πρώτο node της λίστας
    while(temp != NULL){ //αναζήτηση ήδη υπαρκτής ΕΑΝ
        if(strcmp(temp->EAN, EAN) == 0){ //αμα υπάρχει
            temp->quantity += quantity; //αυξάνει την ποσότητα μόνο
            return;
        }
        temp = temp->next;
    }

    //περίπτωση που δεν πληρούνται τα ψηφία όλα, αλλα υπάρχουν αρκετά για recommendation
    if(eanLength < 13){ 
        printf("\nProducts with similar EAN/barcode\n");
        recommend(head, EAN);
    }
    //περίπτωση που πληρούνται όλες οι προϋποθέσεις και δημιουργήτε νέο node (προϊόν), τύπου
    //inv* με την βοήθεια της newInventoryNode συνάρτησης    
    else if(eanLength == 13) { 
        inv* node = newInventoryNode(EAN, name, quantity, price);

        if(*head == NULL){ //θα βάλουμε το καινούριο node στο τέλος της λίστας,
            *head = node;
        }else{
            temp = *head;
            while(temp->next != NULL){
                temp = temp->next;
            }            
            temp->next = node; //εισαγωγή του νέου node
        }
        printf("Product : '%s' (EAN: %s), added to inventory\n", name, EAN);
    }
}

/*Εισαγωγή νέου προϊόντος (αν δεν υπάρχει το προϊόν το προσθέτει στο απόθεμα 
αλλιώς ανανεώνει το απόθεμα για το συγκεκριμένο προϊόν). Θα πρέπει να γίνεται 
έλεγχος στο ΕΑΝ και είτε να εμφανίζεται μήνυμα λάθους είτε -εάν είναι μικρότερο 
από 13 αλλά τουλάχιστον 4 χαρακτήρες- να προτείνει κωδικούς προϊόντων που 
αρχίζουν με το ίδιο πρόθεμα για να επιλέξει ο χρήστης. */

int main(){     
    inv* invHead = NULL;
    char line[100]; //= {"nothing yet"};
    char word[20];

    while(fgets(line,sizeof(line), stdin) != NULL){

        sscanf(line, "%s", word);
        if(strcmp(word, "insert") == 0){
            char ean[EAN_maxSize];
            char name[name_length];
            int quantity;
            double price;

            if(sscanf(line,"%*s %s %s %d %lf", ean, name, &quantity, &price) == 4){ //χρειάστηκε μια extra δήλωση τύπου δεδομένων *s για να προσπεράσουμε την πρώτη λέξη 'insert'
                insertInventory(&invHead, ean, name, quantity, price);
            }else printf("Wrong insert format, try: 'insert <barcode> <name> <quantity> <price>'\n");

        }else if (strcmp(word, "print") == 0){
            inv* temp = invHead;
            while(temp != NULL){
                printf("EAN: %s, name: %s, quantity: %d, price: %.2lf\n", temp->EAN, temp->name, temp->quantity, temp->price);
                temp = temp->next;
            }
        }else if(strcmp(word, "exit") == 0) {
            break;
        }else printf("Cannot recognise command, type help\n"); // kanto kltro grapse sxolia TODO
    }

    return 0;   
}