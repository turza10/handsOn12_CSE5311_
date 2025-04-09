#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *table; // The array holding the elements
    int size;   // Total capacity of the table
    int num;    // Number of elements currently in the table
} DynamicTable;

// Initialize a dynamic table
DynamicTable *table_create()
{
    DynamicTable *T = (DynamicTable *)malloc(sizeof(DynamicTable));
    if (!T)
        return NULL;

    T->size = 0;
    T->num = 0;
    T->table = NULL;

    return T;
}

// Insert an element into the dynamic table (using the algorithm from the textbook)
void table_insert(DynamicTable *T, int x, int *total_cost, int *total_credits)
{
    printf("\n--- Inserting element %d ---\n", x);
    int operation_cost = 1; // Basic cost of inserting an element

    // If table is empty, allocate with 1 slot
    if (T->size == 0)
    {
        T->table = (int *)malloc(sizeof(int));
        if (!T->table)
            return;
        T->size = 1;
        printf("  Initial allocation with 1 slot\n");
    }

    // If table is full, resize it
    if (T->num == T->size)
    {
        int old_size = T->size;
        int new_size = 2 * T->size;
        printf("  Table is full (size=%d), resizing to %d slots\n", old_size, new_size);

        int *new_table = (int *)malloc(new_size * sizeof(int));
        if (!new_table)
            return;

        // Copy all existing elements (each copy is one operation)
        printf("  Copying %d elements from old table to new table\n", T->num);
        for (int i = 0; i < T->num; i++)
        {
            new_table[i] = T->table[i];
        }
        operation_cost += T->num; // Add cost of copying elements

        // Free old table and update to new table
        free(T->table);
        T->table = new_table;
        T->size = new_size;
    }

    // Insert the new element and increment count
    T->table[T->num] = x;
    T->num = T->num + 1;

    // Update cost counters
    *total_cost += operation_cost;

    // Accounting method: charge 3 credits for each insertion
    int operation_credits = 3;
    *total_credits += operation_credits;

    // Display costs for this operation
    printf("  Cost of this insertion: %d\n", operation_cost);
    printf("  Credits charged for this insertion: %d\n", operation_credits);
    printf("  Aggregate method - Total cost so far: %d for %d insertions (amortized: %.2f per operation)\n",
           *total_cost, T->num, (float)*total_cost / T->num);
    printf("  Accounting method - Total credits so far: %d for %d insertions (credits per op: 3)\n",
           *total_credits, T->num);

    // Check if we have enough saved credits for all operations
    int credits_spent = *total_cost;
    int credits_remaining = *total_credits - credits_spent;
    printf("  Credits spent so far: %d, Credits remaining: %d\n", credits_spent, credits_remaining);

    // Print table contents
    printf("  Table [capacity=%d, elements=%d]: ", T->size, T->num);
    for (int i = 0; i < T->num; i++)
    {
        printf("%d ", T->table[i]);
    }
    printf("\n");
}

// Free the dynamic table
void table_destroy(DynamicTable *T)
{
    if (T)
    {
        if (T->table)
            free(T->table);
        free(T);
    }
}

int main()
{
    // Create a new dynamic table
    DynamicTable *table = table_create();

    // Track total cost (for aggregate method) and total credits (for accounting method)
    int total_cost = 0;
    int total_credits = 0;

    printf("===== DYNAMIC TABLE WITH AMORTIZED ANALYSIS =====\n");
    printf("Demonstrating both aggregate and accounting methods\n\n");

    // Insert elements and track the costs
    for (int i = 1; i <= 16; i++)
    {
        table_insert(table, i, &total_cost, &total_credits);
    }

    // Summary
    printf("\n===== FINAL ANALYSIS =====\n");
    printf("Inserted 16 elements with doubling strategy\n");
    printf("Total actual operations performed: %d\n", total_cost);
    printf("Total credits charged (3 per insertion): %d\n", total_credits);
    printf("Amortized cost per operation (aggregate method): %.2f\n", (float)total_cost / 16);
    printf("Credits charged per operation (accounting method): 3\n");

    if (total_credits >= total_cost)
    {
        printf("The accounting method with 3 credits per operation is sufficient!\n");
        printf("Credits remaining: %d\n", total_credits - total_cost);
    }
    else
    {
        printf("The accounting method with 3 credits per operation is not sufficient!\n");
        printf("Credit deficit: %d\n", total_cost - total_credits);
    }

    // Clean up
    table_destroy(table);
    printf("\nDynamic table destroyed\n");

    return 0;
}