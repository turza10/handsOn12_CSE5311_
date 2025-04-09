#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* data;       // pointer to our array
    int size;        // current number of elements
    int capacity;    // total available slots
} Vector;

// Create a new vector
Vector* vector_create() {
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (!vec) return NULL;
    
    vec->capacity = 4;  // start with small capacity
    vec->size = 0;
    vec->data = (int*)malloc(vec->capacity * sizeof(int));
    
    if (!vec->data) {
        free(vec);
        return NULL;
    }
    
    return vec;
}

// Free vector memory
void vector_destroy(Vector* vec) {
    if (vec) {
        free(vec->data);
        free(vec);
    }
}

// Resize the internal array
int vector_resize(Vector* vec, int new_capacity) {
    if (!vec) return 0;
    
    int* new_data = (int*)malloc(new_capacity * sizeof(int));
    if (!new_data) return 0;
    
    // Copy old data to new array
    int copy_size = vec->size;
    if (copy_size > new_capacity) 
        copy_size = new_capacity;
    
    memcpy(new_data, vec->data, copy_size * sizeof(int));
    
    free(vec->data);
    vec->data = new_data;
    vec->capacity = new_capacity;
    
    if (vec->size > new_capacity)
        vec->size = new_capacity;
    
    return 1;
}

// Add element to end
int vector_push(Vector* vec, int value) {
    if (!vec) return 0;
    
    if (vec->size >= vec->capacity) {
        if (!vector_resize(vec, vec->capacity * 2))
            return 0;
    }
    
    vec->data[vec->size] = value;
    vec->size++;
    
    return 1;
}

// Remove element from end
int vector_pop(Vector* vec, int* value) {
    if (!vec || vec->size == 0) return 0;
    
    vec->size--;
    if (value) *value = vec->data[vec->size];
    
    // Shrink if needed
    if (vec->size < vec->capacity / 3 && vec->capacity > 4) {
        vector_resize(vec, vec->capacity / 2);
    }
    
    return 1;
}

// Get element at index
int vector_get(Vector* vec, int index, int* value) {
    if (!vec || index < 0 || index >= vec->size) return 0;
    
    if (value) *value = vec->data[index];
    return 1;
}

// Set element at index
int vector_set(Vector* vec, int index, int value) {
    if (!vec || index < 0 || index >= vec->size) return 0;
    
    vec->data[index] = value;
    return 1;
}

// Insert element at index
int vector_insert(Vector* vec, int index, int value) {
    if (!vec || index < 0 || index > vec->size) return 0;
    
    if (vec->size >= vec->capacity) {
        if (!vector_resize(vec, vec->capacity * 2))
            return 0;
    }
    
    // Shift elements right
    for (int i = vec->size; i > index; i--) {
        vec->data[i] = vec->data[i-1];
    }
    
    vec->data[index] = value;
    vec->size++;
    
    return 1;
}

// Remove element at index
int vector_erase(Vector* vec, int index) {
    if (!vec || index < 0 || index >= vec->size) return 0;
    
    // Shift elements left
    for (int i = index; i < vec->size - 1; i++) {
        vec->data[i] = vec->data[i+1];
    }
    
    vec->size--;
    
    // Shrink if needed
    if (vec->size < vec->capacity / 3 && vec->capacity > 4) {
        vector_resize(vec, vec->capacity / 2);
    }
    
    return 1;
}

// Reserve capacity
int vector_reserve(Vector* vec, int capacity) {
    if (!vec || capacity <= vec->capacity) return 0;
    
    return vector_resize(vec, capacity);
}

// Clear all elements
void vector_clear(Vector* vec) {
    if (vec) vec->size = 0;
}

// Get vector size
int vector_size(Vector* vec) {
    return vec ? vec->size : 0;
}

// Get vector capacity
int vector_capacity(Vector* vec) {
    return vec ? vec->capacity : 0;
}

// Print vector contents
void vector_print(Vector* vec) {
    if (!vec) {
        printf("NULL vector\n");
        return;
    }
    
    printf("Vector [size=%d, capacity=%d]: ", vec->size, vec->capacity);
    for (int i = 0; i < vec->size; i++) {
        printf("%d ", vec->data[i]);
    }
    printf("\n");
}

int main() {
    printf("--- Dynamic Array (Vector) Implementation in C ---\n\n");
    
    // Create a new vector
    printf("Creating vector...\n");
    Vector* vec = vector_create();
    vector_print(vec);
    
    // Test push_back operation
    printf("\nAdding elements with vector_push...\n");
    for (int i = 0; i < 10; i++) {
        vector_push(vec, i * 10);
        printf("After adding %d: ", i * 10);
        vector_print(vec);
    }
    
    // Test get operation
    printf("\nGetting elements by index...\n");
    for (int i = 0; i < vector_size(vec); i++) {
        int value;
        if (vector_get(vec, i, &value)) {
            printf("Element at index %d is %d\n", i, value);
        }
    }
    
    // Test set operation
    printf("\nModifying elements...\n");
    vector_print(vec);
    vector_set(vec, 3, 999);
    vector_set(vec, 7, 888);
    printf("After modifications: ");
    vector_print(vec);
    
    // Test insert operation
    printf("\nInserting elements...\n");
    vector_insert(vec, 0, 111);   // Insert at beginning
    vector_insert(vec, 5, 555);   // Insert in middle
    vector_insert(vec, vector_size(vec), 999);  // Insert at end
    printf("After insertions: ");
    vector_print(vec);
    
    // Test erase operation
    printf("\nErasing elements...\n");
    vector_erase(vec, 0);  // Erase from beginning
    vector_erase(vec, 4);  // Erase from middle
    printf("After erasures: ");
    vector_print(vec);
    
    // Test pop operation
    printf("\nPopping elements...\n");
    int popped_value;
    vector_pop(vec, &popped_value);
    printf("Popped value: %d\n", popped_value);
    vector_pop(vec, &popped_value);
    printf("Popped value: %d\n", popped_value);
    printf("After popping: ");
    vector_print(vec);
    
    // Test reserve operation
    printf("\nReserving capacity...\n");
    printf("Before reserve: capacity = %d\n", vector_capacity(vec));
    vector_reserve(vec, 50);
    printf("After reserve(50): capacity = %d\n", vector_capacity(vec));
    
    // Test clear operation
    printf("\nClearing vector...\n");
    vector_clear(vec);
    printf("After clear: ");
    vector_print(vec);
    
    // Test adding elements after clear
    printf("\nAdding elements after clear...\n");
    for (int i = 5; i > 0; i--) {
        vector_push(vec, i);
    }
    printf("Final vector: ");
    vector_print(vec);
    
    // Clean up
    printf("\nDestroying vector...\n");
    vector_destroy(vec);
    printf("Vector destroyed\n");
    
    return 0;
}