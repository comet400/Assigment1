/* File: assigment1.cpp
Project : SENG1050, A-01 : ASSIGNMENT 1 - SORTED DOUBLY-LINKED LISTS
Programmer : Lukas Fukuoka Vieira 
First version : 18th of May, 2024.
Description : This program will store your flights, their dates, and fares. It will then print the flights sorted by fare and destination. 
The user can search for a flight by destination and date, 
this program uses double linked list to store all information that the user desires to input.
*/

#include <stdio.h>  // Include standard input/output functions
#include <stdlib.h> // Include standard library functions
#include <string.h> // Include string functions

#pragma warning(disable: 4996) // Disable warning for unsafe string functions

#define MAX_INPUT_LENGTH 30 // Maximum length for input strings

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


/*
 * Function: createNode
 * Parameters: const char* destination, const char* date, float farePrice
 * Description: This function creates a new node for the doubly linked list with the given destination, date, and fare price.
 * Return value: FlightNode* (Pointer to the newly created node)
 */
FlightNode* createNode(const char* destination, const char* date, float farePrice);


/*
 * Function: insertSortedByFare
 * Parameters: FlightNode** head, FlightNode** tail, FlightNode* newNode
 * Description: This function inserts a new node into a sorted doubly linked list by fare.
 * Return value: void
 */
void insertSortedByFare(FlightNode** head, FlightNode** tail, FlightNode* newNode);


/*
 * Function: insertSortedByDestination
 * Parameters: FlightNode** head, FlightNode** tail, FlightNode* newNode
 * Description: This function inserts a new node into a sorted doubly linked list by destination.
 * Return value: void
 */
void insertSortedByDestination(FlightNode** head, FlightNode** tail, FlightNode* newNode);


/*
 * Function: findFlight
 * Parameters: FlightNode* head, const char* destination, const char* date
 * Description: This function finds a flight node in the list by destination and date.
 * Return value: FlightNode* (Pointer to the found node or NULL if not found)
 */
FlightNode* findFlight(FlightNode* head, const char* destination, const char* date);


/*
 * Function: deleteNode
 * Parameters: FlightNode* node, FlightNode** head, FlightNode** tail
 * Description: This function deletes a node from a doubly linked list.
 * Return value: void
 */
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail);


/*
 * Function: printFlightList
 * Parameters: const FlightNode* head
 * Description: This function prints the entire flight list starting from the head.
 * Return value: void
 */
void printFlightList(const FlightNode* head);


/*
 * Function: freeFlightList
 * Parameters: FlightNode* head
 * Description: This function frees the entire doubly linked list.
 * Return value: void
 */
void freeFlightList(FlightNode* head);


int main(void) // Main function
{
    FlightNode* fareHead = NULL, * fareTail = NULL;
    FlightNode* destHead = NULL, * destTail = NULL;

    printf("This program will store your flights and their dates and fares.\n");

    while (1)
    {
        char destBuffer[MAX_INPUT_LENGTH]; // Buffer for destination input
        char dateBuffer[MAX_INPUT_LENGTH]; // Buffer for date input
        float fare; // Variable for fare input

        // Get destination input from user
        printf("Enter destination: "); // Prompt user for input
        fgets(destBuffer, MAX_INPUT_LENGTH, stdin); // Read input from user
        destBuffer[strcspn(destBuffer, "\n")] = 0;  // Remove newline character
        if (strcmp(destBuffer, ".") == 0) break;  // Exit loop if user inputs "."

        // Get date input from user
        printf("Enter date (YYYY-MM-DD): "); // Prompt user for input
        fgets(dateBuffer, MAX_INPUT_LENGTH, stdin); // Read input from user
        dateBuffer[strcspn(dateBuffer, "\n")] = 0;  // Remove newline character
        if (strcmp(dateBuffer, ".") == 0) break;  // Exit loop if user inputs "."

        // Get fare input from user
        printf("Enter fare: "); // Prompt user for input
        if (scanf("%f", &fare) != 1) // Read input from user
        {
            printf("Invalid input. Please try again.\n"); // Print error message
            while (getchar() != '\n'); // Clear input buffer
            continue; // Skip the rest of the loop
        }
        while (getchar() != '\n'); // Clear input buffer

        // Create new nodes for fare and destination sorted lists
        FlightNode* newFareNode = createNode(destBuffer, dateBuffer, fare); // Create new node for fare list
        FlightNode* newDestNode = createNode(destBuffer, dateBuffer, fare); // Create new node for destination list

        // Insert nodes into respective sorted lists
        if (newFareNode && newDestNode) 
        {
            insertSortedByFare(&fareHead, &fareTail, newFareNode); // Insert into fare sorted list
            insertSortedByDestination(&destHead, &destTail, newDestNode); // Insert into destination sorted list
        } 
        else 
        {
            printf("Failed to create new node. Please try again.\n"); // Print error message
        }
    }

    // Print the flight list sorted by fare
    printf("\nFlights sorted by fare:\n"); 
    printFlightList(fareHead); // Print the list

    // Print the flight list sorted by destination
    printf("\nFlights sorted by destination:\n"); // Print message
    printFlightList(destHead); // Print the list

    // Search for a flight by destination and date
    char searchDest[MAX_INPUT_LENGTH]; 
    char searchDate[MAX_INPUT_LENGTH];

    printf("\nEnter destination to search: "); // Prompt user for input
    fgets(searchDest, MAX_INPUT_LENGTH, stdin); // Read input from user
    searchDest[strcspn(searchDest, "\n")] = 0;  // Remove newline character

    printf("Enter date to search: "); // Prompt user for input
    fgets(searchDate, MAX_INPUT_LENGTH, stdin); // Read input from user
    searchDate[strcspn(searchDate, "\n")] = 0;  // Remove newline character

    // Find the flight in the destination sorted list
    FlightNode* foundFlight = findFlight(destHead, searchDest, searchDate); // Find the flight
    if (foundFlight) 
    {
        printf("Flight found with fare: %.2f\n", foundFlight->farePrice); // Print the fare of the found flight
        printf("Enter new fare (or the same fare to keep it): "); // Prompt user for input
        float newFare; // Variable for new fare input
        if (scanf("%f", &newFare) != 1) // Read input from user
        {
            printf("Invalid input. Fare unchanged.\n"); // Print error message
            while (getchar() != '\n'); // Clear input buffer if invalid input
        }
        else if (newFare != foundFlight->farePrice)
        {
            // Update the fare and re-insert the node into the fare sorted list
            foundFlight->farePrice = newFare; // Update the fare
            deleteNode(foundFlight, &fareHead, &fareTail); // Delete the node from the fare sorted list
            FlightNode* updatedNode = createNode(searchDest, searchDate, newFare); // Create a new node with updated fare
            insertSortedByFare(&fareHead, &fareTail, updatedNode); // Insert the updated node into the fare sorted list
            printf("Fare updated.\n"); // Print success message
        }
        else
        {
            printf("Fare unchanged.\n"); // Print message if fare is the same
        }
    }
    else
    {
        printf("No matching flight found.\n"); // Print error message if no matching flight is found
    }

    // Print the updated flight list sorted by fare
    printf("\nFlights sorted by fare after update:\n"); 
    printFlightList(fareHead); // Print the updated list

    // Print the updated flight list sorted by destination
    printf("\nFlights sorted by destination after update:\n");
    printFlightList(destHead); // Print the updated list

    // Free all allocated memory
    freeFlightList(fareHead); // Free the fareHead list
    freeFlightList(destHead); // This should be empty after fareHead is freed

    return 0;
}

// Function to create a new flight node
FlightNode* createNode(const char* destination, const char* date, float farePrice)
{
    // Validate the input parameters
    if (destination == NULL || date == NULL) return NULL; // Return NULL if destination or date is NULL
    if (destination[0] == '\0' || date[0] == '\0') return NULL; // Return NULL if destination or date is empty

    // Allocate memory for the new node
    FlightNode* newNode = (FlightNode*)malloc(sizeof(FlightNode)); // Allocate memory for the new node
    if (!newNode) return NULL;  // Check for memory allocation failure

    // Allocate memory for destination and date strings
    newNode->destination = (char*)malloc((strlen(destination) + 1) * sizeof(char)); //Allocate memory for destination string
    newNode->date = (char*)malloc((strlen(date) + 1) * sizeof(char));  // Allocate memory for date string

    // Check for memory allocation failure
    if (!newNode->destination || !newNode->date) 
    {
        // Free allocated memory in case of failure
        free(newNode->destination); // Free memory allocated for destination
        free(newNode->date); // Free memory allocated for date
        free(newNode); // Free memory allocated for the node
        return NULL;
    }

    // Copy the destination and date strings into the new node
    strcpy(newNode->destination, destination); // Copy the destination string
    strcpy(newNode->date, date);    // Copy the date string
    newNode->farePrice = farePrice;  // Set the fare price
    newNode->next = NULL;  // Initialize the next pointer
    newNode->prev = NULL;  // Initialize the previous pointer

    return newNode;
}

// Function to insert a node into a sorted doubly linked list by fare
void insertSortedByFare(FlightNode** head, FlightNode** tail, FlightNode* newNode)
{
    // If the list is empty, set the new node as head and tail
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return;
    }

    // Traverse the list to find the appropriate position
    FlightNode* current = *head;
    while (current != NULL && current->farePrice < newNode->farePrice)
    {
        current = current->next;
    }

    // Insert at the beginning of the list
    if (current == *head)
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
    // Insert at the end of the list
    else if (current == NULL)
    {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
    // Insert in the middle of the list
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
    // If the list is empty, set the new node as head and tail
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return;
    }

    // Traverse the list to find the appropriate position
    FlightNode* current = *head;
    while (current != NULL && strcmp(current->destination, newNode->destination) < 0)
    {
        current = current->next;
    }

    // Insert at the beginning of the list
    if (current == *head)
    {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
    }
    // Insert at the end of the list
    else if (current == NULL)
    {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode;
    }
    // Insert in the middle of the list
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
    // Traverse the list to find the matching node
    while (head != NULL)
    {
        if (strcmp(head->destination, destination) == 0 && strcmp(head->date, date) == 0)
        {
            return head;  // Return the matching node
        }
        head = head->next;
    }
    return NULL;  // Return NULL if not found
}

// Function to delete a node from a doubly linked list
void deleteNode(FlightNode* node, FlightNode** head, FlightNode** tail)
{
    if (node == NULL) return;  // Return if node is NULL

    // If node to delete is the head
    if (node == *head)
    {
        *head = node->next;
        if (*head != NULL)
        {
            (*head)->prev = NULL;
        }
        else {
            *tail = NULL;  // List becomes empty
        }
    }
    // If node to delete is the tail
    else if (node == *tail)
    {
        *tail = node->prev;
        if (*tail != NULL)
        {
            (*tail)->next = NULL;
        }
        else
        {
            *head = NULL;  // List becomes empty
        }
    }
    // If node to delete is in the middle
    else
    {
        if (node->prev != NULL) {
            node->prev->next = node->next;
        }
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
    }
    // Free the memory allocated for the node
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
        head = head->next; // ignore warning
        // Free the memory allocated for each node
        free(temp->destination);
        free(temp->date);
        free(temp);
    }
}

// End of program
