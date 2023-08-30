/* 
    Simple Pharmacy Management Application:
*/

#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 

/*
    Product (Before selling it) informations structure Type:
*/
typedef struct {
    char product_name[50]; 
    unsigned long code; 
    int quantity; 
    float price;
    float TTC_price;
} Product;

/*
    Selled product informations structure Type:
*/
typedef struct {
    char selled_product_name[50]; 
    unsigned long selled_code;
    int selled_quantity;
    float selled_price; 
    float selled_TTC_price;
    // selling_time <----to manage.
} Selled;

/* This function allows us to display all the functionnalities that the program include */
int menu_display() {
    int task_number;
    printf("\n-----------------------------------------------PHARMACY MANAGER MENU:--------------------------------------------------\n");

    printf("1.Add a New product: \n"); 
    printf("2.Add multiple new products: \n");
    printf("3.List all products (Name, Price, TTC Price): \n"); 
    printf("4.Sell a product: \n"); 
    printf("5.Display the selled products: \n");
    printf("6.Search products: \n"); 
    printf("7.Stock status (Products that their quantity lower than 3): \n"); 
    printf("8.Feed the stock: \n"); 
    printf("9.Delete a Product: \n"); 
    printf("10.Sales statistics: \n");
    printf("11.Quit the program!\n");
    
    // The user must input a number between 1 and 9, some thing else it will display a message that tell the user what must do.
    do { 
        printf("\nEnter the Task number (1 - 11): "); 
        if (scanf("%d", &task_number) == 0) task_number = 0;

        // clean up the buffer input stream, in case that there is a left data, To ignore infinit loop.
        while (getchar() != '\n');

    } while (task_number < 1 || task_number > 11);

    return task_number;
}

/* This function allows us to back to the display menu */
void back_to_menu() {
    char back_to_menu; 
    do {
        printf("\tEnter (y) to back to the menu: "); 
        scanf(" %c", &back_to_menu);
        while(getchar() != '\n');
    } while (back_to_menu != 'y' && back_to_menu != 'Y');
}

/* This function allows us to add one product into the store */
void add_new_product(Product *product, int *product_index, int *product_reallocated_size) {
    /* 
        REALLOCTION OF THE MEMORY SPACES THAT STRUCTURE product POINTER POINTING ON, 
        IN CASE OF FILLING ALL THE MEMORY SPACES ON THE HEAP: 
    */
    if(*product_index >= *product_reallocated_size) {
        *product_reallocated_size += 50;
        product = (Product *)realloc(product, *product_reallocated_size * sizeof(Product));
    }

    /* 
        Handling the user data or Buffer input stream:
    */
    char *name = product[*product_index].product_name;
    unsigned long *code = &product[*product_index].code; 
    int *quantity = &product[*product_index].quantity; 
    float *price = &product[*product_index].price; 
    float *TTC_price = &product[*product_index].TTC_price;
    
    char string[50];
    do {
        printf("\tenter name of the product, (Name of the product must include at least 1 alphabet + 4 characters) "); 
        fgets(string, 50, stdin);
        if (string[strlen(string) - 1] == '\n') string[strlen(string) - 1] = '\0';
        else while(getchar() != '\n');

    } while (strlen(string) < 5 || atoi(string) != 0);
    strcpy(name, string);

    // We are using here `code_checker` variable to check if the code that will be enter the by the use already exist or not, To avoid multiple same code!
    int code_checker;
    do { 
        printf("\tEnter the code of the product (Must be a number, Include at least 12 digits and not the same as another product)");
        if (scanf("%lu", code) != 1) *code = 0; 
        while(getchar() != '\n');

        code_checker = 0;
        for (int i = 0; i < *product_index; i++) {
            if (*code == product[i].code) {
                code_checker = 1;
            }
        }
    } while (code_checker == 1 || *code / 100000000000 < 1);

    do {
        printf("\tEnter the quantity of the product (min-quantity: 50, max-quantity: 200): "); 
        if (scanf("%d", quantity) != 1) *quantity = 0;
        while(getchar() != '\n');

    } while (*quantity < 50 || *quantity > 200);
    
    do {
        printf("\tEnter the price of the product (price at least 1$): "); 
        if(scanf("%f", price) != 1) *price = 0;
        while(getchar() != '\n');
    } while (*price < 1);
    
    *TTC_price = *price + ((float)15 / 100) * *price; 

    // increasing product_index by one. 
    ++*product_index;
}

/* This function allows us to add multiple products into the store */
void add_multiple_new_products(Product *product, int *product_index, int *product_reallocated_size) {
    int number_of_products_want_to_set; 
    do {
        printf("\tHow many product you want to enter? (20 is the max products you can set): "); 
        if (scanf("%d", &number_of_products_want_to_set) != 1) number_of_products_want_to_set = 0;
        while(getchar() != '\n');
    } while (number_of_products_want_to_set == 0 || number_of_products_want_to_set > 20);
    
    for (int i = 0; i < number_of_products_want_to_set; i++) {
        printf("\nEnter informations of product %d: \n", i + 1);
        add_new_product(product, product_index, product_reallocated_size);
    }

    // back to the menu:
    back_to_menu();
}

/* This function implement the logic to display the product elements in an alphabitical order or with a price order */
void ordered_structure_array(Product *product, int *product_index, int *choise) {
    int choise1, choise2; 
    if (*choise == 1) {
        choise1 = 1; 
        choise2 = 0;
    }else {
        choise1 = 0; 
        choise2 = 1;
    }

    for (int i = 0; i < *product_index; i++) {
        for (int j = i + 1; j < *product_index; j++) {
            if (choise1 && product[i].product_name[0] > product[j].product_name[0] || choise2 && product[i].price < product[j].price) {
                char name_swapping[50] = {};
                strcpy(name_swapping, product[i].product_name);
                unsigned long code_swapping = product[i].code;
                int quantity_swapping       = product[i].quantity; 
                float price_swapping        = product[i].price;
                float TTC_price_swapping    = product[i].TTC_price;
                
                strcpy(product[i].product_name, product[j].product_name); 
                product[i].code             = product[j].code;
                product[i].quantity         = product[j].quantity;
                product[i].price            = product[j].price; 
                product[i].TTC_price        = product[j].TTC_price;

                strcpy(product[j].product_name, name_swapping);
                product[j].code             = code_swapping; 
                product[j].quantity         = quantity_swapping; 
                product[j].price            = price_swapping; 
                product[j].TTC_price        = TTC_price_swapping;

            }
        }
    }
    // Display structure elements of product array after ordering them alphabitically
    for (int i = 0; i < *product_index; i++) {
        printf("-Product name: %s\t-code: %lu\t-quantity: %d\t-price: %.2f$\t-TTC_price: %.2f$\n", product[i].product_name, product[i].code, product[i].quantity, product[i].price, product[i].TTC_price);
    }
}

void increasing_alphabitic_order_display(Product *product, int *product_index, int *choise) {
    ordered_structure_array(product, &*product_index, &*choise);
}

void decreasing_price_order_display(Product *product, int *product_index, int *choise) {
    ordered_structure_array(product, &*product_index, &*choise);
}
/* Listing the product elements in an alphabitical order or with a price order*/
void list_all_products(Product *product, int *product_index, int display_back_to_menu) {
    if (*product_index > 0) {
        int display_methode; 
        printf("\n1.Display all products with an increasing alphabitic order: "); 
        printf("\n2.Display all products with a decreasing price order: \n");
    
        int swtch = -1;
        do {
            printf("Choose between (1 and 2): ");
            if (scanf("%d", &display_methode) != 1) display_methode = 0;
            while(getchar() != '\n');
            if(display_methode == 1) swtch = 1;
            else if(display_methode == 2) swtch = 2;

        } while (display_methode != swtch || display_methode < 1);
        if(display_methode == 1) increasing_alphabitic_order_display(product, &*product_index, &display_methode); 
        else decreasing_price_order_display (product, &*product_index, &display_methode);
    }else printf("\tThere is no products in the store to display, Feed the store.\n");
    // Back to the menu: 
    if (display_back_to_menu == 1) back_to_menu();
}

/* This function allows us to sell a product from the the structure product array and implement it to the structure selled array */
void sell_product(Product *product, Selled *selled, int *product_index, int *selled_index, int *selled_reallocated_size) {
    if (*product_index > 0) {
        /* 
            REALLOCTION OF THE MEMORY SPACES THAT STRUCTURE selled POINTER POINTING ON, 
            IN CASE OF FILLING ALL THE MEMORY SPACES ON THE HEAP: 
        */
        if(*selled_index >= *selled_reallocated_size) {
            *selled_reallocated_size += 50;
            selled = (Selled *)realloc(selled, *selled_reallocated_size * sizeof(Selled));
        }

        /*
            Selling logic Implementation:
        */
        list_all_products(product, product_index, 0);

        unsigned long product_code_to_sell; 
        int product_quantity_to_sell;
        do {
            printf("Enter (carefully) the code of product that you want to sell, (All codes include at least 12 digits): "); 
            if (scanf("%lu", &product_code_to_sell) != 1) product_code_to_sell = 0;
            while(getchar() != '\n'); 
        } while (product_code_to_sell / 100000000000 < 1);
    
        int checker = 0;
        int find_product_index;
        int recover_selled_index = -1;
        for (int i = 0; i < *product_index; i++) {
           // Checking if code is valid
           if (product[i].code == product_code_to_sell) {
                checker = 1; 
                find_product_index = i;
            }
            // Checking if code is already in selled structure array
            if (selled[i].selled_code == product_code_to_sell && *selled_index <= *product_index) recover_selled_index = i;
        }
        // If the code is valid, The proccess will be continue if not it will display to the user that the code is not valid
        if (checker == 1) {
            do {
                printf("Enter the quantity of the product that you want to sell (At least 1 product from the quantity): ");
                if (scanf("%d", &product_quantity_to_sell) != 1) product_quantity_to_sell = 0;
                while(getchar() != '\n');
            } while (product_quantity_to_sell < 1);

            /*
                Now we must check based on the input of the user if the code exist on the list or not, If it is we need to update the list
            */
            if (recover_selled_index != -1) {
                if (product[find_product_index].quantity > 0 && product_quantity_to_sell <= product[find_product_index].quantity) {
                    product[find_product_index].quantity -= product_quantity_to_sell;
                    selled[recover_selled_index].selled_quantity  += product_quantity_to_sell;
                }else if(product[find_product_index].quantity <= 0 ) {
                    printf("The quantity of %s is empty, (You must feed the store).\n", product[find_product_index].product_name);

                }else if(product_quantity_to_sell > product[find_product_index].quantity) {
                    printf("The quantity of %s is lower than your order, (You must feed the store).\n", product[find_product_index].product_name);
                }

            }else if(product[find_product_index].code == product_code_to_sell) {

                if (product[find_product_index].quantity > 0 && product_quantity_to_sell <= product[find_product_index].quantity) {
                    product[find_product_index].quantity -= product_quantity_to_sell;

                    strcpy(selled[*selled_index].selled_product_name, product[find_product_index].product_name);
                    selled[*selled_index].selled_quantity  = product_quantity_to_sell;
                    selled[*selled_index].selled_code      = product[find_product_index].code; 
                    selled[*selled_index].selled_price     = product[find_product_index].price; 
                    selled[*selled_index].selled_TTC_price = product[find_product_index].TTC_price;
                    // Increasing the index of selled product list.
                    ++*selled_index;

                }else if(product[find_product_index].quantity <= 0 ) {
                    printf("The quantity of %s is empty, (You must feed the store).\n", product[find_product_index].product_name);

                }else if(product_quantity_to_sell > product[find_product_index].quantity) {
                    printf("The quantity of %s is lower than your order, (You must feed the store).\n", product[find_product_index].product_name);
                }
            }
        }else {
            printf("The code you entered is not valid: \n");
        }
    }else printf("\tThere is no products in the store to sell, Feed to the store.\n");
    // Back to the menu: 
    back_to_menu();
}

/* This function allows us to display the selled products, their informations too*/
void display_selled_product(Selled *selled, int *selled_index) {
    int selled_checker = 0;
    for (int i = 0; i < *selled_index; i++) {
        printf("-Selled product name: %s\t-code: %lu\t-quantity: %d\t-price: %.2f$\t-TTC_price: %.2f$\n", selled[i].selled_product_name, selled[i].selled_code, selled[i].selled_quantity, selled[i].selled_price, selled[i].selled_TTC_price);
        selled_checker = 1;
    }
    if (selled_checker == 0) printf("\tThere is no selled products yet.\n");
    // Back to the menu: 
    back_to_menu();
}

/* Searching products(before selling them) with their codes or with the quantity of those products */
void search_products(Product *product, int *product_index) {
    if (*product_index > 0) {
        int search_mode;
        unsigned long search_with_code; 
        int search_with_quantity;
        do { 
            printf("Search a product using 'code' or 'quantity': \n\tClick (1) and Enter the code, Or click (2) and Enter the quantity:"); 
            if (scanf("%d", &search_mode) != 1) search_mode = 0;
            while(getchar() != '\n');

            if (search_mode == 1) {
                do {
                    printf("\tEnter the code of the product, All codes include 12 digits or more, So the code must have at least 12 digits: ");
                    if(scanf("%lu", &search_with_code) != 1) search_with_code = 0;
                    while(getchar() != '\n');

                } while (search_with_code / 100000000000 < 1);

            }else if(search_mode == 2) {
                do {
                    printf("\tEnter the quantity to get products with value: ");
                    if (scanf("%d", &search_with_quantity) != 1) search_with_quantity = 0;
                    while(getchar() != '\n');
                } while (search_with_quantity < 1);
            }

        } while (search_mode != 1 && search_mode != 2);
    
        if (search_mode == 1) {
            int code_checker = 0;
            for (int i = 0; i < *product_index; i++) {
                if (product[i].code == search_with_code) {
                    printf("-Product name: %s\t-code: %lu\t-quantity: %d\t-price: %.2f\t-TTC_price: %.2f\n", product[i].product_name, product[i].code, product[i].quantity, product[i].price, product[i].TTC_price);
                    code_checker = 1;
                    break;
                }
            }
            if (code_checker == 0) printf("\tThere is no product with this code.\n");

        }else if(search_mode == 2) {
            int quantity_checker = 0;
            for (int i = 0; i < *product_index; i++) {
                if (product[i].quantity == search_with_quantity) {
                    printf("-Product name: %s\t-code: %lu\t-quantity: %d\t-price: %.2f\t-TTC_price: %.2f\n", product[i].product_name, product[i].code, product[i].quantity, product[i].price, product[i].TTC_price);
                    quantity_checker = 1;
                }
            }
            if (quantity_checker == 0) printf("\tThere is no product with this quantity in the store.\n");
        } 
    }else printf("\tThere is no products in the store Yet, Feed the the store.\n");
    // Back to the menu: 
    back_to_menu();
}

/* Display the products in the store that their quantities lower than 3 */
void stock_status(Product *product, int *product_index) {
    if(*product_index > 0) {
        int quantity_checker = 0;
        for (int i = 0; i < *product_index; i++) {
            if (product[i].quantity < 3) {
                printf("-Product name: %s\t-code: %lu\t-quantity: %d\n", product[i].product_name, product[i].code, product[i].quantity);
                quantity_checker = 1;
            }
        }  
        if (quantity_checker == 0) printf("\tThere is no product in the store that its quantity lower that 3.\n");
    }else printf("\tThere is no products in the store Yet, Feed the store.\n");
    // Back to the menu:
    back_to_menu();
}

/* Developing the quantity of the structure product elements */
void feed_the_stock(Product *product, int *product_index) {
    if (*product_index > 0) {
        list_all_products(product, product_index, 0);

        unsigned long product_code;
        int product_code_checker = 0;
        int find_product_index;
       
        do {
            printf("\tEnter the code of the product that you want to augment its quantity, (All codes includes at least 12 digits: ");
            if (scanf("%lu", &product_code) != 1) product_code = 0;
            while(getchar() != '\n');

        } while (product_code / 100000000000 < 1); 

        for (int i = 0; i < *product_index; i++) {
            if (product[i].code == product_code) {
                product_code_checker = 1;
                find_product_index = i;
            } 
        }
        
        if (product_code_checker == 1) {
            int update_quantity;
            do { 
                printf("\tAdd the quantity to this product, from(1 - 200): "); 
                if (scanf("%d", &update_quantity) != 1) update_quantity = 0;
                while(getchar() != '\n');

            } while (update_quantity < 1 || update_quantity > 200);
    
            // Updating the product quantity
            product[find_product_index].quantity += update_quantity;
        }else printf("\tYour code is not valid.\n");

    }else printf("\tThere is no products in the store Yet, You must add a product with options (1) or (2) and then you can augment it with this option.\n");
    // Back to the menu: 
    back_to_menu();
}

/* Delete product using its code */
void delete_product(Product *product, int *product_index) {
    if (*product_index > 0) {
        list_all_products(product, product_index, 0);

        unsigned long product_code;
        int product_code_checker = 0;
        int find_product_index;
        
        do { 
            printf("\tEnter the code of the product that you want to delete from the store, At least 12 digits: ");
            if (scanf("%lu", &product_code) != 1) product_code = 0;
            while(getchar() != '\n');
        } while (product_code / 100000000000 < 1);

        for (int i = 0; i < *product_index; i++) {
            if (product[i].code == product_code) {
                product_code_checker = 1;
                find_product_index = i;
            }
        }
        
        if (product_code_checker == 1) {
            // All the structures elements of product array will back forward to the deleted element 
            --*product_index; 
            for (int i = 0; i < *product_index; i++) {
                if (i >= find_product_index) product[i] = product[i + 1];
            }
        }else printf("\tYour code is not valid.\n");
    }else printf("\tThere is no products in the store Yet, Feed the store.\n");
    // Back to the menu: 
    back_to_menu();
}

void total_price_selled_products(Selled *selled, int *selled_index, float *total) {
    for (int i = 0; i < *selled_index; i++) *total += (selled[i].selled_quantity * selled[i].selled_price);
    printf("\tThe total price of the selled products is: %.2f $.\n", *total);
}

void average_price_selled_products(Selled *selled, int *selled_index, float *total) {
    int selled_p_numbers = 0;
    for (int i = 0; i < *selled_index; i++) selled_p_numbers += selled[i].selled_quantity;
    printf("\tThe average price of the selled products is: %.2f $.\n", *total / selled_p_numbers);
}

void max_selled_price(Selled *selled, int *selled_index) {
    float max_selled_price = selled[0].selled_price;
    for (int i = 0; i < *selled_index; i++) if (selled[i].selled_price > max_selled_price) max_selled_price = selled[i].selled_price;
    printf("\tThe max selled price is %.2f $.\n", max_selled_price);
}

void min_selled_price(Selled *selled, int *selled_index) {
    float min_selled_price = selled[0].selled_price; 
    for (int i = 0; i < *selled_index; i++) if (selled[i].selled_price < min_selled_price) min_selled_price = selled[i].selled_price;
    printf("\tThe min selled price is %.2f $.\n", min_selled_price);
}

/* Total price of the selled products - Average price of the the selled products - Max selled price - Min selld price */
void sales_statistics(Selled *selled, int *selled_index) {
    if (*selled_index > 0) {
        float total = 0;
        total_price_selled_products(selled, selled_index, &total); 
        average_price_selled_products(selled, selled_index, &total); 
        max_selled_price(selled, selled_index); 
        min_selled_price(selled, selled_index);

    }else printf("\tThere is no selled products to display the statistics.\n");
    // Back to the menu: 
    back_to_menu();
}

int main() {
    int product_reallocated_size = 100;
    int selled_reallocated_size  = 100;
    int quit_program  = 0;
    int product_index = 0;
    int selled_index  = 0;
    Product *product; 
    Selled  *selled;
    /*
        We are allocating 100 memory size in the heap as initial value, Then when we will need more we will handle it programmatically:D
    */
    product = (Product *)calloc(100, sizeof(Product));
    selled  = (Selled  *)calloc(100, sizeof(Selled));

    do {
        int task_number = menu_display();
        int display_back_to_menu = 1;
        // Task that chosen by number:
        switch(task_number) {
            case 1: add_new_product(product, &product_index, &product_reallocated_size);
            break;
            case 2: add_multiple_new_products(product, &product_index, &product_reallocated_size);
            break; 
            case 3: list_all_products(product, &product_index, display_back_to_menu);
            break; 
            case 4: sell_product(product, selled, &product_index, &selled_index, &selled_reallocated_size);
            break; 
            case 5: display_selled_product(selled, &selled_index);
            break;
            case 6: search_products(product, &product_index); 
            break;
            case 7: stock_status(product, &product_index); 
            break;
            case 8: feed_the_stock(product, &product_index); 
            break;
            case 9: delete_product(product, &product_index); 
            break;
            case 10: sales_statistics(selled, &selled_index);
            break;
            case 11: quit_program = 1;
            break;
        }
    } while (quit_program == 0);

    free(product); 
    free(selled);
    
    return 0;
}
//
