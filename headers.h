#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_OF_PRODUCTS 20
#define NUM_OF_CUSTOMERS 5
#define ORDERS_PER_CUSTOMER 10

// Δομή για τα προϊόντα
typedef struct {
    char Descript[50];
    float Price;
    int Item_Count;
    int Total_Req;
    int Sold_Pieces;
    int Not_Served_Users;
} Product;

// Συνάρτηση για αρχικοποίηση του καταλόγου προϊόντων
void initialize_catalog(Product catalog[]);

// Συνάρτηση για print του summary
void print_report(Product catalog[], int Total_Req, int Successful_Orders, int Failed_Orders, float Total_Rev);

#endif // HEADERS_H
