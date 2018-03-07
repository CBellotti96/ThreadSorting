#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define ARRAY_SIZE 30

using namespace std;

int values[ARRAY_SIZE];

void *sort(void *argument) {
  int *endpoints = (int *)argument;
  int beg = endpoints[0];
  int end = endpoints[1];
  for (int i = 0; i < end+1; i++) { //bubblesort
    for (int j = 0; j < end-i; j++) {
      if (values[beg+j]>values[beg+j+1]) {
        int tmp = values[beg+j];
        values[beg+j] = values[beg+j+1];
        values[beg+j+1] = tmp;
      }
    }
  }
  pthread_exit(NULL);
}
void *merge(void *part){
  int *p = (int *)part;
  int partition = p[1];
  int right = 0;
  int left = partition; //15
  int counter = 0;
  int solution[ARRAY_SIZE];
  while (counter < ARRAY_SIZE-1){
    if (values[right] < values[left]){
      solution[counter++] = values[right++];
    }
    else{
      solution[counter++] = values[left++];
    }
  }
  if (left < ARRAY_SIZE){
    solution[ARRAY_SIZE-1] = values[ARRAY_SIZE-1];
  }
  else {
    solution[ARRAY_SIZE-1] = values[partition+1];
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

  cout<<"ORIGINAL ARRAY:"<<endl;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    cout << values[i] << " ";
  }
  cout << endl;

  pthread_create(&sort1_tid, &attr, sort, &left);
  pthread_create(&sort2_tid, &attr, sort, &right);
  pthread_join(sort1_tid, NULL);
  pthread_join(sort2_tid, NULL);
  int i = 15;
  //pthread_create(&merge_tid, &attr, merge, &left);
  //pthread_join(merge_tid, NULL);

  cout<<"FINAL ARRAY:"<<endl;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    cout << values[i] << " ";
  }
  cout << endl;

  return 1;
}
