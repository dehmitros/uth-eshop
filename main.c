#include "headers.h"

// Συνάρτηση για αρχικοποίηση του καταλόγου προϊόντων
void initialize_catalog(Product catalog[]) {
    for (int i = 0; i < NUM_OF_PRODUCTS; i++) {
        snprintf(catalog[i].Descript, sizeof(catalog[i].Descript), "Product %d", i + 1);
        catalog[i].Price = (rand() % 1000) / 100.0 + 1; // Τυχαία τιμή μεταξύ 1 και 10.00
        catalog[i].Item_Count = 2;
        catalog[i].Total_Req = 0;
        catalog[i].Sold_Pieces = 0;
        catalog[i].Not_Served_Users = 0;
    }
}

// Συνάρτηση για print του summary
void print_report(Product catalog[], int Total_Req, int Successful_Orders, int Failed_Orders, float Total_Rev) {
    int Num_Of_Req_Prod = 0;
    int Num_Of_Bought_Prod = 0;

    // Σύνολο προϊόντων που έγιναν request και που πουλήθηκαν
    for (int i = 0; i < NUM_OF_PRODUCTS; i++) {
        Num_Of_Req_Prod += catalog[i].Total_Req;
        Num_Of_Bought_Prod += catalog[i].Sold_Pieces;
    }

    // Print το summary
    printf("\nSummary:\n");
    printf("%d requests were made, where %d succeeded and %d failed.\n", Total_Req, Successful_Orders, Failed_Orders);
    printf("%d products were requested, where %d products were bought, totalling %.2f euros.\n",
           Num_Of_Req_Prod, Num_Of_Bought_Prod, Total_Rev);
}

int main() {
    Product catalog[NUM_OF_PRODUCTS];
    initialize_catalog(catalog);

    int Order_Pipes[NUM_OF_CUSTOMERS][2]; // Σωλήνες για αιτήματα
    int Response_Pipes[NUM_OF_CUSTOMERS][2]; // Σωλήνες για απαντήσεις

    pid_t pids[NUM_OF_CUSTOMERS];

    srand(time(NULL));

    // Δημιουργία σωλήνων για κάθε πελάτη
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        if (pipe(Order_Pipes[i]) == -1 || pipe(Response_Pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Δημιουργία διεργασιών πελατών
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        pids[i] = fork();

        if (pids[i] == 0) {
            // Child (Client)
            close(Order_Pipes[i][0]);
            close(Response_Pipes[i][1]);

            // Ξεχωριστό seed για κάθε client
            srand(time(NULL) ^ (getpid() << 16));

            for (int j = 0; j < ORDERS_PER_CUSTOMER; j++) {
                int product_index = rand() % NUM_OF_PRODUCTS; // Επιλογή τυχαίου προϊόντος
                if (write(Order_Pipes[i][1], &product_index, sizeof(product_index)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }

                char Response[100];
                if (read(Response_Pipes[i][0], Response, sizeof(Response)) == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                printf("Client %d: %s\n", i + 1, Response);

                sleep(1); // ανά δευτερόλεπτο
            }

            close(Order_Pipes[i][1]);
            close(Response_Pipes[i][0]);
            exit(EXIT_SUCCESS);
        }
    }

    // Γονική διεργασία (Διακομιστής)
    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        close(Order_Pipes[i][1]);
        close(Response_Pipes[i][0]);
    }

    int Total_Req = 0, Successful_Orders = 0, Failed_Orders = 0;
    float Total_Rev = 0.0;

    for (int completed_orders = 0; completed_orders < NUM_OF_CUSTOMERS * ORDERS_PER_CUSTOMER; completed_orders++) {
        for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
            int product_index;

            if (read(Order_Pipes[i][0], &product_index, sizeof(product_index)) > 0) {
                catalog[product_index].Total_Req++;
                Total_Req++;

                char Response[100];
                if (catalog[product_index].Item_Count > 0) {
                    catalog[product_index].Item_Count--;
                    catalog[product_index].Sold_Pieces++;
                    Successful_Orders++;
                    Total_Rev += catalog[product_index].Price;
                    snprintf(Response, sizeof(Response), "Purchase complete, your total is: %.2f euro.",
                             catalog[product_index].Price);
                } else {
                    catalog[product_index].Not_Served_Users++;
                    Failed_Orders++;
                    snprintf(Response, sizeof(Response), "Products unavailable, request failed.");
                }

                if (write(Response_Pipes[i][1], Response, sizeof(Response)) == -1) {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
                sleep(1); // Επεξεργασία μίας παραγγελίας ανά δευτερόλεπτο
            }
        }
    }

    for (int i = 0; i < NUM_OF_CUSTOMERS; i++) {
        close(Order_Pipes[i][0]);
        close(Response_Pipes[i][1]);
        wait(NULL); // Αναμονή για ολοκλήρωση process πελατών
    }

    print_report(catalog, Total_Req, Successful_Orders, Failed_Orders, Total_Rev);

    return 0;
}