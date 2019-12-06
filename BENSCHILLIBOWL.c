#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

void AddOrderToBack(Order **orders, Order *order);

MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};
int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    return BENSCHILLIBOWLMenu[rand() % BENSCHILLIBOWLMenuLength];
}

/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */

BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {
    BENSCHILLIBOWL *bcb = (BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL));
    bcb->orders = NULL;
    bcb->order_num = 0;
	  bcb->order_num_next = 1;
    bcb->num_orders_handled = 0;
    bcb->max_size = max_size;
    bcb->expected_num_orders=expected_num_orders;
    pthread_mutex_init(&(bcb->mutex), NULL);  
    printf("Restaurant is open!\n");
    return bcb;
}


/* check that the number of orders received is equal to the number handled (ie.fullfilled). Remember to deallocate your resources */

void CloseRestaurant(BENSCHILLIBOWL* bcb) {
  if (bcb->num_orders_handled != bcb->expected_num_orders) {    
        fprintf(stderr, "Some or all orders were not handled.\n");
        exit(0);
    }
    pthread_mutex_destroy(&(bcb->mutex));  
    free(bcb);
    printf("Restaurant is closed!\n");
}

/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
  pthread_mutex_lock(&(bcb->mutex));
  
  while (bcb->order_num == bcb->order_num_next) { 
        pthread_cond_wait(&(bcb->can_add_orders), &(bcb->mutex));
    }
  
    order->order_num = bcb->order_num_next;
    AddOrderToBack(&(bcb->orders), order);
    bcb->order_num_next++;  
    bcb->order_num++;       
    pthread_cond_broadcast(&(bcb->can_get_orders)); ded.
    pthread_mutex_unlock(&(bcb->mutex)); 
  
    return -1;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
  
    pthread_mutex_lock(&(bcb->mutex)); //  critical section
       
    while(bcb->order_num == 0) {  
        if (bcb->num_orders_handled >= bcb->expected_num_orders) {
            pthread_mutex_unlock(&(bcb->mutex));
            return NULL;
        }
        pthread_cond_wait(&(bcb->can_get_orders), &(bcb->mutex));
    }
    
    // Reached the front of the queue
    Order *order = bcb->orders;
    bcb->orders = bcb->orders->next;
    bcb->order_num--; 
    bcb->num_orders_handled++;  
  
    pthread_cond_broadcast(&(bcb->can_add_orders));
    pthread_mutex_unlock(&(bcb->mutex));   
    return order;
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {
  if (*orders != NULL) { 
        Order *order_num = *orders;
        while (order_num->next) {
            order_num = order_num->next;}
        curr_order->next = order;
  }
  else {  
        *orders = order;}   
}