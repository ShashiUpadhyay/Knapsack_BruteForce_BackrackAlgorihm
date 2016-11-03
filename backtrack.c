#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUFFER_SIZE 500
#define MAX_EDGE_WEIGHT 100

#define NUM_ITEM_INDEX 0
#define CAPACITY_INDEX 3

int *include, *best_case, *p, *w;
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

/* Function to allocate memory */
int** memory_allocate(int size){
    int **matrix, i;
    matrix = (int **)calloc(size, sizeof(int *));
    if(matrix == NULL){
        printf("Error in memory allocation \n");
        exit(0);
    }
    for(i = 0; i < size; i++){
        matrix[i] = (int *) malloc(size*sizeof(int));
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
void generate_knapsack_item_list(struct knapsack_item *item, int n, int **knapsack_data_matrix)
{
    int total_weight = 0, i;
    for(i = 0; i < n; i++)
    {
        (item+i)->weight = knapsack_data_matrix[1][i];
        total_weight += (item+i)->weight;
        (item+i)->profit = knapsack_data_matrix[2][i];
        (item+i)->selected = 0;
        (item+i)->item_number = i;
    }
}

/* Function to sort the knapsack items */
struct knapsack_item* sort_Knapsack_item_list(struct knapsack_item *knapsack,int n)
{
    struct knapsack_item temp, *new_Knapsack;
    float profit_per_unit_i, profit_per_unit_j;
    int i = 0,j;
    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            profit_per_unit_i = (float)((float)knapsack[i].profit/(float)knapsack[i].weight);
            profit_per_unit_j = (float)((float)knapsack[j].profit/(float)knapsack[j].weight);
            
            if(profit_per_unit_i <= profit_per_unit_j)
            {
                temp = knapsack[i];
                knapsack[i] = knapsack[j];
                knapsack[j] = temp;
            }
        }
    }
    new_Knapsack = malloc(sizeof(struct knapsack_item)*(n+1));
    
    for(i=0;i<n;i++)
    {
        new_Knapsack[i+1] = knapsack[i];
    }
    new_Knapsack[0].item_number = -1;
    new_Knapsack[0].profit = 0;
    new_Knapsack[0].weight = 0;
    return new_Knapsack;
}

float knapsack_weight_profit(int weight_index,int weight,int profit,int n)
{
    int item_selection_index[n], j;
    float bound = (float)profit;
    j=weight_index;
    
    while(j<n)
    {
        item_selection_index[j] = 0;
        j++;
    }
    
    while(weight < capacity && weight_index<=n)
    {
        if(weight+w[weight_index] > capacity)
        {
            float weight_index_ex = ((float)capacity - (float)weight)/(float)w[weight_index];
            bound += ((float)p[weight_index] * weight_index_ex);
            weight = capacity;
        }
        else
        {
            item_selection_index[weight_index] = 1;
            weight += w[weight_index];
            bound += (float)p[weight_index];
        }
        weight_index++;
    }
    return bound;
}

int Promising(int weight_index,int weight,int profit,int n)
{
    float bound;
    if (weight >= capacity){
        return 0;
    }
    bound = knapsack_weight_profit(weight_index,weight,profit,n);
    return (bound>(float)maxprofit);
}

void Knapsack(int weight_index,int profit,int weight,int n)
{
    int k;
    if(weight <= capacity)
    {
        if(profit > maxprofit)
        {
            maxprofit = profit;
            num = weight_index;
            for(k = 1;k<n;k++){
                best_case[k] = include[k];
            }
        }
    }
    if(Promising(weight_index,weight,profit,n))
    {
        include[weight_index+1] = 1;
        Knapsack(weight_index+1,profit+p[weight_index+1],weight+w[weight_index+1],n);
        include[weight_index+1] = 0;
        Knapsack(weight_index+1,profit,weight,n);
    }
}

void backtracking(struct knapsack_item *knapsack,int n)
{
    struct knapsack_item *new_Knapsack = sort_Knapsack_item_list(knapsack,n);
    include = malloc(sizeof(int)*(n+1));
    best_case = malloc(sizeof(int)*(n+1));
    p = malloc(sizeof(int)*(n+1));
    w = malloc(sizeof(int)*(n+1));
    int i=0;
    while(i<n+1)
    {
        best_case[i] = include[i] = 0;
        p[i] = new_Knapsack[i].profit;
        w[i] = new_Knapsack[i].weight;
        i++;
    }
    Knapsack(0,0,0,n+1);
}

/* Function to redirect the output to file */
void redirect_backtracking_output_file(int *best_case, int n)
{
    int i=0, max_weight = 0;
    int *output_array;
    output_array = (int*) malloc(sizeof(int)*BUFFER_SIZE);
    int output_array_index;
    output_array_index=0;
    for(i = 1; i <= n; i++)
    {
        if(best_case[i] == 1)
        {
            output_array[output_array_index] = i;
            output_array_index++;
            max_weight = max_weight + w[i];
        }
    }
    
    FILE *output_file;
    output_file = fopen("backtrack_output.txt","w");
    if (output_file == NULL){
        printf("Couldn't open backtrack_output.txt for writing\n");
    }else{
        fprintf(output_file,"%d\n",max_weight);
        fprintf(output_file,"%d\n",maxprofit);
        
        for(i = 0; i < output_array_index; i++)
        {
            if(i == output_array_index-1){
                fprintf(output_file,"%d", output_array[i]);
            }else{
                fprintf(output_file,"%d, ", output_array[i]);
            }
        }
        fclose(output_file);
    }
    free(p);
    free(w);
    free(output_array);
}


int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("Please enter just 1 arguments.\n");
        exit(0);
    }else{
        FILE *file = NULL;
        file = fopen(argv[1],"r");
        if (file == NULL){
            printf("Couldn't open %s for reading\n",argv[1]);
            return 0;
        }else{
            int** knapsack_data_matrix = memory_allocate(MAX_EDGE_WEIGHT + 1);
            char line_buffer[BUFFER_SIZE];
            int matrix_size; matrix_size=0;
            char* filedata=(char*)malloc(sizeof(char));
            int i=0,j=0;
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
            int number_of_items, knapsack_capacity;
            number_of_items = knapsack_data_matrix[NUM_ITEM_INDEX][0];
            struct knapsack_item *item = (struct knapsack_item*) malloc(sizeof(struct knapsack_item) * number_of_items);
            knapsack_capacity = knapsack_data_matrix[CAPACITY_INDEX][0];
            generate_knapsack_item_list(item, number_of_items, knapsack_data_matrix);
            capacity = knapsack_capacity;
            backtracking(item,number_of_items);
            redirect_backtracking_output_file(best_case, num);
            deallocate_memory(knapsack_data_matrix,matrix_size);
            free(item);
        }
    }
}

