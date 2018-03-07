#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 20

int global_arr[SIZE];

void *select_sort(void *params) { //passing in start and end bounds of current thread's subarray
  int *bounds = (int *)params;
  int start = bounds[0];
  int end = bounds[1];
  int min_index;
  printf("subarray for thread %d: ", (start%(start-1)));
  for (int i = start; i < end+1; i++) {
    printf("%d ", global_arr[i]);
  }
  printf("\n");
  fflush(stdout);
  for (int i = start; i < end; i++) { //performing selection sort on subarray
    min_index = i;
    for (int j = i+1; j < end + 1; j++) {
      if (global_arr[j] < global_arr[min_index]) {
        min_index = j;
      }
    }
    int tmp = global_arr[min_index];
    global_arr[min_index] = global_arr[i];
    global_arr[i] = tmp;
  }
  printf("sorted subarray for thread %d: ", (start%(start-1)));
  for (int i = start; i < end + 1; i++) {
    printf("%d ", global_arr[i]);
  }
  printf("\n");
  pthread_exit(NULL); //halts execution of thread, return to main function and end pthread_join()
}
void *merge(void *param){ //passing in bounds of the subarray which contains the partition value as it's start, we will only use this value
  int *sub_size = (int *)param;
  int partition = sub_size[0]; //copy partition value from param[]
  int left_array = 0;
  int right_array = partition;
  int merge_subs[SIZE];
  int index = 0; //signifies the current index of the fully merged array
  while (left_array < partition && right_array < SIZE){ //walk through each list, compare values and place in merge array
    if (global_arr[left_array] < global_arr[right_array]){
      merge_subs[index++] = global_arr[left_array++];
    }
    else{
      merge_subs[index++] = global_arr[right_array++];
    }
  }
  //once one list is empty, walk through the rest of the other list and place the values in merge array
  while (left_array < partition){
    merge_subs[index++] = global_arr[left_array++];
  }
  while (right_array < SIZE){
    merge_subs[index++] = global_arr[right_array++];
  }
  for (int i = 0; i < SIZE; i++) {
    global_arr[i] = merge_subs[i];
  }
  pthread_exit(NULL);
}
int main(int argc, char * argv[]){

  //randomly generating numbers between 0 & 500 for global array
  srand(time(NULL));
  for (int i = 0; i < SIZE; i++) {
     global_arr[i] = (rand()%500) + 1; //values generated between 1 and 100
  }

  pthread_t thr[3];

  int partition = SIZE/2;
  int left_array[2] = {0, partition-1};
  int right_array[2] = {partition, SIZE-1};

  printf("Printing randomized, unsorted array: ");
  for (int i = 0; i < SIZE; i++) {
    printf("%d ", global_arr[i]);
  }
  printf("\n");
  fflush(stdout);

  pthread_create(&thr[0], NULL, select_sort, left_array);
  pthread_create(&thr[1], NULL, select_sort, right_array);
  pthread_join(thr[0], NULL);
  pthread_join(thr[1], NULL);
  pthread_create(&thr[2], NULL, merge, right_array);
  pthread_join(thr[2], NULL);

  printf("Printing final sorted array: ");
  for (int i = 0; i < SIZE; i++) {
    printf("%d, ", global_arr[i]);
  }
  printf("\n");
  fflush(stdout);

  return 0;
}
