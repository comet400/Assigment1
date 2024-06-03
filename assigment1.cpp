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

int main(void)
{
    FlightNode* fareHead = NULL, * fareTail = NULL;
    FlightNode* destHead = NULL, * destTail = NULL;

    printf("This program will store your flights and their dates and fares.\n");

    while (1)
    {
        char destBuffer[MAX_INPUT_LENGTH];
        char dateBuffer[MAX_INPUT_LENGTH];
        float fare;

        printf("Enter destination: ");
        fgets(destBuffer, MAX_INPUT_LENGTH, stdin);
        destBuffer[strcspn(destBuffer, "\n")] = 0;
        if (strcmp(destBuffer, ".") == 0) break;

        printf("Enter date (YYYY-MM-DD): ");
        fgets(dateBuffer, MAX_INPUT_LENGTH, stdin);
        dateBuffer[strcspn(dateBuffer, "\n")] = 0;
        if (strcmp(dateBuffer, ".") == 0) break;

        printf("Enter fare: ");
        if (scanf("%f", &fare) != 1)
        {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        while (getchar() != '\n'); // Clear input buffer

        FlightNode* newFareNode = createNode(destBuffer, dateBuffer, fare);
        FlightNode* newDestNode = createNode(destBuffer, dateBuffer, fare);

        if (newFareNode && newDestNode) {
            insertSortedByFare(&fareHead, &fareTail, newFareNode);
            insertSortedByDestination(&destHead, &destTail, newDestNode);
        }
        else {
            printf("Failed to create new node. Please try again.\n");
        }
    }

    printf("\nFlights sorted by fare:\n");
    printFlightList(fareHead);

    printf("\nFlights sorted by destination:\n");
    printFlightList(destHead);

    char searchDest[MAX_INPUT_LENGTH];
    char searchDate[MAX_INPUT_LENGTH];

    printf("\nEnter destination to search: ");
    fgets(searchDest, MAX_INPUT_LENGTH, stdin);
    searchDest[strcspn(searchDest, "\n")] = 0;

    printf("Enter date to search: ");
    fgets(searchDate, MAX_INPUT_LENGTH, stdin);
    searchDate[strcspn(searchDate, "\n")] = 0;

    FlightNode* foundFlight = findFlight(destHead, searchDest, searchDate);
    if (foundFlight) 
    {
        printf("Flight found with fare: %.2f\n", foundFlight->farePrice);
        printf("Enter new fare (or the same fare to keep it): ");
        float newFare;
        if (scanf("%f", &newFare) != 1)
        {
            printf("Invalid input. Fare unchanged.\n");
            while (getchar() != '\n'); // Clear input buffer
        }
        else if (newFare != foundFlight->farePrice)
        {
            foundFlight->farePrice = newFare;
            deleteNode(foundFlight, &fareHead, &fareTail);
            FlightNode* updatedNode = createNode(searchDest, searchDate, newFare);
            insertSortedByFare(&fareHead, &fareTail, updatedNode);
            printf("Fare updated.\n");
        }
        else
        {
            printf("Fare unchanged.\n");
        }
    }
    else
    {
        printf("No matching flight found.\n");
    }

    printf("\nFlights sorted by fare after update:\n");
    printFlightList(fareHead);

    printf("\nFlights sorted by destination after update:\n");
    printFlightList(destHead);

    // Free all allocated memory
    freeFlightList(fareHead);
    freeFlightList(destHead); // This should be empty after fareHead is freed

    return 0;
}

// Function to create a new node
FlightNode* createNode(const char* destination, const char* date, float farePrice)
{
    if (destination == NULL || date == NULL) return NULL;
    if (destination[0] == '\0' || date[0] == '\0') return NULL;

    FlightNode* newNode = (FlightNode*)malloc(sizeof(FlightNode));
    if (!newNode) return NULL;

    newNode->destination = (char*)malloc((strlen(destination) + 1) * sizeof(char));
    newNode->date = (char*)malloc((strlen(date) + 1) * sizeof(char));

    if (!newNode->destination || !newNode->date) {
        free(newNode->destination);
        free(newNode->date);
        free(newNode);
        return NULL;
    }

    strcpy(newNode->destination, destination);
    strcpy(newNode->date, date);
    newNode->farePrice = farePrice;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

// Function to insert a node into a sorted doubly linked list by fare
void insertSortedByFare(FlightNode** head, FlightNode** tail, FlightNode* newNode)
{
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return;
    }

    FlightNode* current = *head;
    while (current != NULL && current->farePrice < newNode->farePrice) {
        current = current->next;
    }

    if (current == *head)
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
    else if (current == NULL)
    {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
    else
    {
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }
}

// Function to insert a node into a sorted doubly linked list by destination
void insertSortedByDestination(FlightNode** head, FlightNode** tail, FlightNode* newNode)
{
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return;
    }

    FlightNode* current = *head;
    while (current != NULL && strcmp(current->destination, newNode->destination) < 0)
    {
        current = current->next;
    }

    if (current == *head)
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
    else if (current == NULL)
    {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
    else
    {
        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;
    }
}

// Function to find a flight node by destination and date
FlightNode* findFlight(FlightNode* head, const char* destination, const char* date)
{
    while (head != NULL)
    {
        if (strcmp(head->destination, destination) == 0 && strcmp(head->date, date) == 0) 
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

// Function to delete a node from a doubly linked list
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail)
{
    if (node == NULL) return;

    if (node == *head) {
        *head = node->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        else {
            *tail = NULL;
        }
    }
    else if (node == *tail) {
        *tail = node->prev;
        if (*tail != NULL) {
            (*tail)->next = NULL;
        }
        else {
            *head = NULL;
        }
    }
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free(node->destination);
    free(node->date);
    free(node);
}

// Function to print the flight list
void printFlightList(const FlightNode* head)
{
    while (head != NULL) {
        printf("%-35s %-35s %-10.2f\n", head->destination, head->date, head->farePrice);
        head = head->next;
    }
}

// Function to free the entire list
void freeFlightList(FlightNode* head)
{
    while (head != NULL)
    {
        FlightNode* temp = head;
        head = head->next;
        free(temp->destination);
        free(temp->date);
        free(temp);
    }
}
