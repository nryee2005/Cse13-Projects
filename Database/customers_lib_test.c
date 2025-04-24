#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customers_lib.h"

void str_should_be_exactly_equal(const char *message, char *expected,
                                 char *actual) {
  printf("%s\n", message);
  printf("%s: wanted \"%s\", got \"%s\"\n",
         !strcmp(expected, actual) ? "SUCCESS" : "FAILURE", expected, actual);
}

void should_be_exactly_equal(const char *message, int expected, int actual) {
  printf("%s\n", message);
  printf("%s: wanted %d, got %d\n",
         (expected == actual) ? "SUCCESS" : "FAILURE", expected, actual);
}

int main(void) {

printf("\n**** tests for add_customer_to_list ****\n");

ll_customers *list = NULL;

list = add_customer_to_list(list, "nryee@ucsc.edu", "nathan", 11, "sushi");
str_should_be_exactly_equal("add customer to empty list", "sushi", list->favorite_food);

list = add_customer_to_list(list, "nryee@ucsc.edu", "nate", 5, "ramen");
str_should_be_exactly_equal("update customer name", "nate", list->name);
str_should_be_exactly_equal("update customer food", "ramen", list->favorite_food);


list = add_customer_to_list(list, "dani@ucsc.edu", "dani", 9, "peppers");
str_should_be_exactly_equal("add another customer: first", "dani@ucsc.edu", list->email);
str_should_be_exactly_equal("add another customer: second", "nryee@ucsc.edu", list->next->email);

printf("\n**** tests for display_customer ****\n");

int count = 1;
ll_customers *listcopy = list;
while(listcopy) {
  printf("Customer %d:\n", count);
  display_customer(listcopy);
  printf("\n");

  listcopy = listcopy->next;
  count++; 
}

printf("\n**** tests for delete_customer_from_list ****\n");

list = add_customer_to_list(list, "pcb@ucsc.edu", "brian", 10, "steak");
list = delete_customer_from_list(list, "dani@ucsc.edu");
str_should_be_exactly_equal("remove second node", "ramen", list->next->favorite_food);
list = delete_customer_from_list(list, "pcb@ucsc.edu");
str_should_be_exactly_equal("remove first node", "ramen", list->favorite_food);
list = delete_customer_from_list(list, "nryee@ucsc.edu");
printf("Removed last node, is list empty?\n");
printf("%s\n", (list==NULL) ? "SUCCESS" : "FAILURE");


printf("\n**** tests for add_customer ****\n");

customer_table *table = make_table(1000);

add_customer(table, "nryee@ucsc.edu", "nathan", 11, "sushi");
str_should_be_exactly_equal("add customer to table", "sushi", lookup_customer(table,"nryee@ucsc.edu")->favorite_food);
add_customer(table, "dani@ucsc.edu", "dani", 11, "malatang");
str_should_be_exactly_equal("add another customer to table", "malatang", lookup_customer(table,"dani@ucsc.edu")->favorite_food);
add_customer(table, "dani@ucsc.edu", "dani", 11, "indomie");
str_should_be_exactly_equal("update existing customer", "indomie", lookup_customer(table,"dani@ucsc.edu")->favorite_food);

printf("\n**** tests for list_customers ****\n");
list_customers(table);

printf("\n**** tests for delete_customer ****\n");

bool success = delete_customer(table, "nryee@ucsc.edu");
if (success) {
  printf("SUCCESSFULLY DELETED CUSTOMER\n");
} else { printf("FAILED\n"); }
success = delete_customer(table, "dani@ucsc.edu");
if (success) {
  printf("SUCCESSFULLY DELETED SECOND CUSTOMER\n");
} else { printf("FAILED\n"); }

success = false;
for (size_t i = 0; i<table->num_buckets; i++) {
  if (table->buckets[i]) {
    success = true;
  }
}
if (success) {
  printf("table not empty");
}
else { printf("table empty"); }

printf("\n**** tests for load_customers ****\n");
load_table(table, "customers_test.tsv");
printf("hello\n");
str_should_be_exactly_equal("sammy", "bananas", lookup_customer(table,"sammy@ucsc.edu")->favorite_food);
str_should_be_exactly_equal("bimmy", "bimmy", lookup_customer(table,"bimmy@ucsc.edu")->name);
str_should_be_exactly_equal("joe", "borgar", lookup_customer(table,"joe@example.com")->favorite_food);
list_customers(table);

printf("\n**** tests for save_to_file ****\n");
add_customer(table, "nryee@ucsc.edu", "nathan", 11, "sushi");
add_customer(table, "dani@ucsc.edu", "dani", 11, "malatang");
save_to_file(table, "customers.tsv");
printf("Successfully wrote customer table to file\n");

quit_program(table);

}

