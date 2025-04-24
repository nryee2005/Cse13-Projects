#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//structs

typedef struct ll_customers {
  char *email;
  char *name;
  int shoe_size;
  char *favorite_food;
  
  struct ll_customers *next;
   
} ll_customers;

typedef struct customer_table {

  size_t num_buckets;
  ll_customers **buckets;

} customer_table;

//functions for ll_customers

ll_customers *add_customer_to_list(ll_customers *list, char *email, char *name, int shoe_size, char *favorite_food);

ll_customers *delete_customer_from_list(ll_customers *list, char *email);

ll_customers *lookup_on_list(ll_customers *list, char *email);

void update_customer(ll_customers *customer, char *name, int shoe_size, char *favorite_food);

void display_customer(ll_customers *customer);

//functions for customer_table

customer_table *make_table(size_t num_buckets);

void load_table(customer_table *table, const char *filename);

void add_customer(customer_table *table, char *email, char *name, int shoe_size, char *favorite_food);

ll_customers *lookup_customer(customer_table *table, char *email);

bool delete_customer(customer_table *table, char *email);

void list_customers(customer_table *table);

void save_to_file(customer_table *table, const char *filename);

void quit_program(customer_table *table);


#define UNUSED(x) (void)(x)

void chomp(char *s);







