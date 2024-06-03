#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996)

#define MAX_INPUT_LENGTH 30

// Define the structure for a doubly linked list node
typedef struct FlightNode
{
    char* destination;
    char* date;
    float farePrice;
    struct FlightNode* next;
    struct FlightNode* prev;
} FlightNode;

// Function prototypes

// Function to create a new node
FlightNode* createNode(const char* destination, const char* date, float farePrice);

// Function to insert a node into a sorted doubly linked list by fare
void insertSortedByFare(FlightNode** head, FlightNode** tail, FlightNode* newNode);

// Function to insert a node into a sorted doubly linked list by destination
void insertSortedByDestination(FlightNode** head, FlightNode** tail, FlightNode* newNode);

// Function to find a flight node by destination and date
FlightNode* findFlight(FlightNode* head, const char* destination, const char* date);

// Function to delete a node from a doubly linked list
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail);

// Function to print the flight list
void printFlightList(const FlightNode* head);

// Function to free the entire list
void freeFlightList(FlightNode* head);

