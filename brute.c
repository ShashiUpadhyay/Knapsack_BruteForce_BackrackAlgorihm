#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 500
#define MAX_EDGE_WEIGHT 100

#define NUM_ITEM_INDEX 0
#define CAPACITY_INDEX 3

int *include,*p,*w;
int capacity = 0;
int maxprofit = 0;
int num = 0;

/* Structure to store knapsack items */
struct knapsack_item
{
    int profit;
    int weight;
    int selected;
    int item_number;
};

struct knapsack_item *selected_items;

/* Function to allocate memory */
int** memory_allocate(int size){
    int **matrix, i;
    matrix = (int **)calloc(size, sizeof(int *));
    if(matrix == NULL){
        printf("Error in memory allocation \n");
        exit(0);
    }
    for(i = 0; i < size; i++){
        matrix[i] = (int *)malloc(size*sizeof(int));
        if(matrix[i] == NULL)
        {
            printf("Error in memory allocation \n");
            exit(0);
        }
    }
    return matrix;
}

/* Function to de-allocate memory */
void deallocate_memory(int **array, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}

/* Function to generate and initialize knapsack items */
void generate_knapsack_item_list(struct knapsack_item *item, int number_of_items, int **knapsack_data_matrix)
{
    int total_weight = 0, i;
    for(i = 0; i < number_of_items; i++)
    {
        (item+i)->weight = knapsack_data_matrix[1][i];
        total_weight += (item+i)->weight;
        (item+i)->profit = knapsack_data_matrix[2][i];
        (item+i)->selected = 0;
        (item+i)->item_number = i;
    }
}

/* Function to redirect the output to file */
void redirect_knapsack_output_file(struct knapsack_item *item, int n)
{
    int i;
    FILE *output_file;
    output_file = fopen("brute_output.txt","w");
    int *output_array, total_profit, total_weight, output_array_index;
    output_array = (int*) malloc(sizeof(int)*BUFFER_SIZE);
    output_array_index=0;
    total_profit = 0;
    total_weight = 0;
    for(i = 0; i < n; i++)
    {
        if((item+i)->selected == 1)
        {
            total_profit += (item+i)->profit;
            total_weight += (item+i)->weight;
            output_array[output_array_index] = (item+i)->item_number;
            output_array_index++;
        }
    }
    fprintf(output_file,"%d\n",total_weight);
    fprintf(output_file,"%d\n",total_profit);
    
    for(i = 0; i < output_array_index; i++)
    {
        if(i == output_array_index-1){
            fprintf(output_file,"%d",output_array[i]);
        }else{
            fprintf(output_file,"%d, ",output_array[i]);
        }
    }
    fclose(output_file);
    free(output_array);
}

//knapsack algorithm using brute force approach
int brute_force_knapsack(struct knapsack_item *item, int number_of_items, int knapsack_capacity)
{
    int i, j, m, selected_count, optimal_profit, last;
    optimal_profit = 0;
    last = (int) pow(2,number_of_items);
    selected_count = 0;
    for(i = 1; i < last; i++)
    {
        int total_weight = 0;
        int total_profit = 0;
        struct knapsack_item *temp_item_list = (struct knapsack_item *)calloc(number_of_items,sizeof(struct knapsack_item));
        int temp_item_list_index_count = 0;
        for(j = 0; j<number_of_items; j++)
        {
            if (((i >> j) & 1) != 1)
            {
                continue;
            }
            total_weight += (item + j)->weight;
            total_profit += (item + j)->profit;
            temp_item_list[temp_item_list_index_count] = item[j];
            temp_item_list_index_count++;
        }
        
        if(total_weight <= knapsack_capacity)
        {
            if(total_profit >= optimal_profit)
            {
                optimal_profit = total_profit;
                selected_items = (struct knapsack_item *)calloc(sizeof(struct knapsack_item), temp_item_list_index_count);
                selected_count = 0;
                for(m = 0; m < temp_item_list_index_count; m++)
                {
                    selected_items[m] = temp_item_list[m];
                    (selected_items + m)->selected = 1;
                    selected_count++;
                }
                free(temp_item_list);
            }
        }
    }
    return selected_count;
}


int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("Please enter just 1 arguments.\n");
        exit(0);
    }else{
        FILE *file = NULL;
        // Assume argv[1] is a filename to open
        file = fopen(argv[1],"r");
        /* fopen returns 0, the NULL pointer, on failure */
        if (file == NULL){
            printf("Could not open %s for reading\n",argv[1]);
            return 0;
        }else{
            int** knapsack_data_matrix = memory_allocate(MAX_EDGE_WEIGHT + 1);
            char line_buffer[BUFFER_SIZE];
            int matrix_size;
            matrix_size=0;
            char* filedata=(char*)malloc(sizeof(char));
            int i=0,j=0;
            // Code to read data from file and store in an array
            while(fgets(line_buffer,BUFFER_SIZE,file)!=NULL){
                filedata=strtok(line_buffer,",");
                while(filedata!=NULL){
                    knapsack_data_matrix[i][j] = atoi(filedata);
                    j++;
                    filedata=strtok(NULL,",");
                }
                i++; j=0;
                matrix_size++;
            }
            fclose(file);
            free(filedata);
            int number_of_items, knapsack_capacity, brute_force_selected_item;
            brute_force_selected_item =0;
            number_of_items = knapsack_data_matrix[NUM_ITEM_INDEX][0];
            struct knapsack_item *item = (struct knapsack_item*) malloc(sizeof(struct knapsack_item) * number_of_items);
            knapsack_capacity = knapsack_data_matrix[CAPACITY_INDEX][0];
            generate_knapsack_item_list(item, number_of_items, knapsack_data_matrix);
            brute_force_selected_item = brute_force_knapsack(item, number_of_items, knapsack_capacity);
            redirect_knapsack_output_file(selected_items, brute_force_selected_item);
            free(selected_items);
            deallocate_memory(knapsack_data_matrix,matrix_size);
            free(item);
        }
    }
}







