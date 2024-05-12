#include "defs.h"

// Initialize an EvidenceType structure
void initEvidence(int id, char* room, char* device, float value, int timestamp, EvidenceType* ev) {
    ev->id = id;
    strncpy(ev->room, room, MAX_STR);
    strncpy(ev->device, device, MAX_STR);
    ev->value = value;
    ev->timestamp = timestamp;
}

// Print evidence with special formatting
void printEvidence(EvidenceType* evidence) {
    char formattedTime[9];
    int hours = evidence->timestamp / 3600;
    int minutes = (evidence->timestamp % 3600) / 60;
    int seconds = evidence->timestamp % 60;
    snprintf(formattedTime, sizeof(formattedTime), "%02d:%02d:%02d", hours, minutes, seconds);

    // Determine the device format based on the provided criteria
    char deviceFormat[32] = "";

    if (strncmp(evidence->device, "EMF", 3) == 0) {
        if (evidence->value > 4.0) {
            snprintf(deviceFormat, sizeof(deviceFormat), " (HIGH)");
        }
        // Print EMF with 1 decimal place
        printf("%-4d | %-15s | %-12s | %15.1f%s | %10s\n", evidence->id, evidence->room, evidence->device, evidence->value, deviceFormat, formattedTime);
    } else if (strncmp(evidence->device, "THERMAL", 7) == 0) {
        if (evidence->value < 0.0) {
            snprintf(deviceFormat, sizeof(deviceFormat), " (COLD)");
        }
        // Print THERMAL with 2 decimal places
        printf("%-4d | %-15s | %-12s | %15.2f%s | %10s\n", evidence->id, evidence->room, evidence->device, evidence->value, deviceFormat, formattedTime);
    } else if (strncmp(evidence->device, "SOUND", 5) == 0) {
        if (evidence->value < 35.0) {
            snprintf(deviceFormat, sizeof(deviceFormat), " (WHISPER)");
        } else if (evidence->value > 70.0) {
            snprintf(deviceFormat, sizeof(deviceFormat), " (SCREAM)");
        }
        // Print SOUND with 1 decimal place
        printf("%-4d | %-15s | %-12s | %13.1f%s | %10s\n", evidence->id, evidence->room, evidence->device, evidence->value, deviceFormat, formattedTime);
    } else {
        // Handle other device types here
        // Print values with 2 decimal places
        printf("%-4d | %-15s | %-12s | %24.2f%s | %10s\n", evidence->id, evidence->room, evidence->device, evidence->value, deviceFormat, formattedTime);
    }
}




// Helper function to copy strings safely
void copyString(char* dest, const char* src, size_t max_length) {
    strncpy(dest, src, max_length);
    dest[max_length - 1] = '\0'; // Ensure null-terminated string
}

// Perform a deep copy of an EvidenceType structure
void copyEvidence(EvidenceType* dest, EvidenceType* src) {
    dest->id = src->id;
    copyString(dest->room, src->room, MAX_STR);
    copyString(dest->device, src->device, MAX_STR);
    dest->value = src->value;
    dest->timestamp = src->timestamp;
}
void initEvidenceArray(EvidenceArray* evArr, int cap) {
    // Initialize an EvidenceArray with a given capacity
    evArr->capacity = cap;
    evArr->size = 0;
    evArr->elements = (EvidenceType*)malloc(cap * sizeof(EvidenceType));

    // Check for memory allocation failure
    if (evArr->elements == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(C_NOK);
    }
}

void growEvidenceArray(EvidenceArray* evArr) {
    // Increase the capacity of an existing EvidenceArray
    EvidenceArray tempArray;
    initEvidenceArray(&tempArray, evArr->capacity * 2);

    // Copy existing elements to the new array
    for (int i = 0; i < evArr->size; i++) {
        copyEvidence(&tempArray.elements[i], &evArr->elements[i]);
    }

    free(evArr->elements);
    evArr->elements = tempArray.elements;
    evArr->capacity = tempArray.capacity;
}

void addEvidence(EvidenceArray* evArr, EvidenceType* src) {
    // Add evidence to an EvidenceArray, maintaining sorted order

    // Check if the array needs to be grown
    if (evArr->size == evArr->capacity) {
        growEvidenceArray(evArr);
    }

    int insertIndex = 0;

    // Find the correct insertion point in descending order by room name
    while (insertIndex < evArr->size && strcmp(src->room, evArr->elements[insertIndex].room) < 0) {
        insertIndex++;
    }

    // If rooms have the same name, sort in ascending order by timestamp
    if (insertIndex < evArr->size && strcmp(src->room, evArr->elements[insertIndex].room) == 0) {
        while (insertIndex < evArr->size && src->timestamp > evArr->elements[insertIndex].timestamp) {
            insertIndex++;
        }
    }

    // Move elements forward to make space for the new element
    for (int i = evArr->size; i > insertIndex; i--) {
        copyEvidence(&evArr->elements[i], &evArr->elements[i - 1]);
    }

    // Copy the evidence into the array at the insertion point
    copyEvidence(&evArr->elements[insertIndex], src);

    // Increase the size of the array
    evArr->size++;
}

int delEvidence(EvidenceArray* evArr, int id) {
    // Delete evidence from an EvidenceArray by its ID
    int found = 0;
    for (int i = 0; i < evArr->size; i++) {
        if (evArr->elements[i].id == id) {
            // Move elements backward to fill the space
            for (int j = i; j < evArr->size - 1; j++) {
                copyEvidence(&evArr->elements[j], &evArr->elements[j + 1]);
            }
            evArr->size--;
            found = 1;
            break;
        }
    }
    return found ? C_OK : C_NOK;
}

void printEvidenceArray(EvidenceArray* evArr) {
    // Print the contents of an EvidenceArray
    printf("ID   | Room Name       | Device       | Value                  | Timestamp\n");
    printf("-----|-----------------|--------------|------------------------|-----------\n");
    for (int i = 0; i <evArr->size; i++) {
        printEvidence(&evArr->elements[i]);
    }
}

void cleanupEvidenceArray(EvidenceArray* evArr) {
    // Free the memory allocated for an EvidenceArray
    free(evArr->elements);
}
