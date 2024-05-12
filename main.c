#include <stdio.h>
#include <string.h>

#include "defs.h" // Include necessary header file

int main() {
    EvidenceArray evArr;
    initEvidenceArray(&evArr, MAX_CAP); // Initialize an EvidenceArray with a maximum capacity

    // Load test data
    loadEvidenceData(&evArr); // Load test data into the EvidenceArray

    int choice;
    do {
        printMenu(&choice); // Display the main menu and get the user's choice

        switch (choice) {
            case 1:
                // Add Evidence
                {
                    EvidenceType newEvidence;
                    printf("Enter the ID: ");
                    scanf("%d", &newEvidence.id);
                    printf("Enter the Room Name: ");
                    while ((getchar()) != '\n'); // Clear input buffer
                    fgets(newEvidence.room, MAX_STR, stdin);
                    newEvidence.room[strlen(newEvidence.room) - 1] = '\0'; // Remove newline

                    // Get the Evidence Type
                    int evidenceType;
                    printf("Enter the Evidence Type (1 for EMF, 2 for THERMAL, 3 for SOUND): ");
                    scanf("%d", &evidenceType);

                    // Set the "Device" field based on the evidence type
                    switch (evidenceType) {
                        case 1:
                            strcpy(newEvidence.device, "EMF");
                            break;
                        case 2:
                            strcpy(newEvidence.device, "THERMAL");
                            break;
                        case 3:
                            strcpy(newEvidence.device, "SOUND");
                            break;
                        default:
                            // Handle invalid evidence types here
                            break;
                    }

                    printf("Enter the Device Reading Value: ");
                    scanf("%f", &newEvidence.value);
                    printf("Enter the Timestamp (hours minutes seconds): ");
                    int hours, minutes, seconds;
                    scanf("%d %d %d", &hours, &minutes, &seconds);

                    // Calculate the total timestamp in seconds
                    newEvidence.timestamp = hours * 3600 + minutes * 60 + seconds;

                    addEvidence(&evArr, &newEvidence); // Add the new evidence to the EvidenceArray
                    printf("Evidence added:\n");
                    printEvidence(&newEvidence); // Print the added evidence
                }
                break;
            case 2:
                // Delete Evidence
                {
                    int id;
                    printf("Enter the ID of the evidence to remove: ");
                    scanf("%d", &id);

                    int result = delEvidence(&evArr, id); // Delete evidence with the specified ID
                    if (result == C_OK) {
                        printf("Evidence with ID %d has been removed.\n", id);
                    } else {
                        printf("Evidence with ID %d not found.\n", id);
                    }
                }
                break;
            case 3:
                // Print Evidence
                printEvidenceArray(&evArr); // Print the contents of the EvidenceArray
                break;
        }
    } while (choice != 0);

    // Cleanup allocated memory
    cleanupEvidenceArray(&evArr); // Free memory used by the EvidenceArray

    return C_OK;
}

/* 
  Function: Print Menu
  Purpose:  Print the main menu and get the user's choice via user input
  Params:   
    Output: int* choice - the choice typed by the user
*/
void printMenu(int* choice)
{
  // NOTE: Do not make _any_ modifications to this function.
  int c = -1;
  int numOptions = 3;

  printf("\nMAIN MENU\n");
  printf("  (1) Add evidence\n");
  printf("  (2) Delete evidence\n");
  printf("  (3) Print evidence\n");
  printf("  (0) Exit\n\n");

  printf("Please enter your selection: ");
  scanf("%d", &c);

  if (c == 0) {
    *choice = c;
    return;
  }

  while (c < 0 || c > numOptions) {
    printf("Please enter your selection: ");
    scanf("%d", &c);
  }

  *choice = c;
}
