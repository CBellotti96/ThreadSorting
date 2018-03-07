#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 30

int values[ARRAY_SIZE];

void  *sort(void *argument) {
  int *endpoints = (int *)argument;
  int beg = endpoints[0];
  int end = endpoints[1];
  int min_index;
  printf("subarray: ");
  for (int i = beg; i < end+1; i++) {
    printf("%d ", values[i]);
  }
  printf("\n");
  fflush(stdout);
  for (int i = beg; i < end; i++) {
    min_index = i;
    for (int j = i+1; j < end + 1; j++) {
      if (values[j] < values[min_index]) {
        min_index = j;
      }
    }
    int tmp = values[min_index];
    values[min_index] = values[i];
    values[i] = tmp;
  }
  printf("sorted subarray: ");
  for (int i = beg; i < end + 1; i++) {
    printf("%d ", values[i]);
  }
  printf("\n");
  pthread_exit(NULL);
}
void *merge(void *part){
  int *p = (int *)part;
  int partition = p[0];
  printf("%d \n", partition);
  int left_array = 0;
  int right_array = partition; //15
  int solution[ARRAY_SIZE];
  int counter = 0;
  while (left_array < partition && right_array < ARRAY_SIZE){
    if (values[left_array] < values[right_array]){
      solution[counter++] = values[left_array++];
    }
    else{
      solution[counter++] = values[right_array++];
    }
  }
  while (left_array < partition){
    solution[counter++] = values[left_array++];
  }
  while (right_array < ARRAY_SIZE){
    solution[counter++] = values[right_array++];
  }
  for (int i = 0; i < ARRAY_SIZE; i++) {
    values[i] = solution[i];
  }
  pthread_exit(NULL);
}
int main(int argc, char * argv[]){

  int partition = ARRAY_SIZE/2;

  //random number generator into the global values array
  srand(time(NULL));
  for (int i = 0; i < ARRAY_SIZE; i++) {
     values[i] = (rand()%100) + 1; //values generated between 1 and 100
  }

  pid_t pid;
  pthread_t sort1_tid, sort2_tid, merge_tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  int left[2] = {0, partition-1};
  int right[2] = {partition, ARRAY_SIZE-1};

  printf("ORIGINAL ARRAY: ");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("%d ", values[i]);
  }
  printf("\n");
  fflush(stdout);

  pthread_create(&sort1_tid, NULL, sort, left);
  pthread_create(&sort2_tid, NULL, sort, right);
  pthread_join(sort1_tid, NULL);
  pthread_join(sort2_tid, NULL);
  pthread_create(&merge_tid, NULL, merge, right);
  pthread_join(merge_tid, NULL);

  printf("FINAL ARRAY: ");
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("%d, ", values[i]);
  }
  printf("\n");
  fflush(stdout);

  return 0;
}
