#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "heap.h"

typedef struct nodo{
   void* data;
   int priority;
}heapElem;

typedef struct Heap{
  heapElem* heapArray;
  int size;
  int capac;
} Heap;

void* heap_top(Heap* pq){
  if(pq->size==0)return NULL;
  return pq->heapArray[0].data;
}

void upHeap(Heap *pq,int index){
  int indexPadre = (index - 1) / 2;
  if(index > 0 && pq->heapArray[index].priority < pq->heapArray[indexPadre].priority){
    heapElem aux = pq->heapArray[indexPadre];
    pq->heapArray[indexPadre] = pq->heapArray[index];
    pq->heapArray[index] = aux;
    upHeap(pq, indexPadre);
  }
}

void heap_push(Heap* pq, void* data, int priority){
  heapElem node = {data,priority};
  
  if(pq->size >= pq->capac){
    pq->capac = ((pq->capac)*2)+1;
    pq->heapArray = realloc(pq->heapArray, sizeof(heapElem)*pq->capac);
  }
  
  pq->heapArray[pq->size] = node;
  upHeap(pq, pq->size);
  pq->size++;
}

void downHeap(Heap *pq,int index){
  int leftIndex = index * 2 + 1;
  int rightIndex = index * 2 + 2;

  int smallestIndex = index;

  if(leftIndex < pq->size && pq->heapArray[leftIndex].priority < pq->heapArray[smallestIndex].priority){
    smallestIndex = leftIndex;
  }
  if(rightIndex < pq->size && pq->heapArray[rightIndex].priority < pq->heapArray[smallestIndex].priority){
    smallestIndex = rightIndex;
  }

  if(smallestIndex != index){
    heapElem aux = pq->heapArray[smallestIndex];
    pq->heapArray[smallestIndex] = pq->heapArray[index];
    pq->heapArray[index] = aux;
    downHeap(pq, smallestIndex);
  }
}

void heap_pop(Heap* pq){
  if(pq->size == 0) return;

  heapElem ultimo = pq->heapArray[pq->size-1];
  pq->heapArray[0] = ultimo;
  pq->size--;
  if(pq->size > 0 ) downHeap(pq, 0);
}

Heap* createHeap(){
  Heap *local = malloc(sizeof(Heap));
  if(local == NULL){
    printf("No se pudo asignar memoria al Heap");
    exit(1);
  }
  local->heapArray = malloc(sizeof(heapElem)*3);
  local->size = 0;
  local->capac = 3;

  return local;
}

