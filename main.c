#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "hashmap.h"




typedef struct{
  char *nombre;
  int prioridad;
  int numeroTareasProcedentes;
  char **tareasProcedentes;
  int estado;
  int inMap;//inMap = 0: estan fuera del heap(solo esta en el mapa) / inMap = 1 :estan dentro del      heap(tambien en el mapa)
}Tarea;




//lee el csv y agrega las tareas
void read(Heap *monticulo, HashMap *mapa) {

  char *csv=malloc(sizeof(char)*50);
  printf("Ingrese el nombre del csv para leer: ");
  scanf("%s49",csv);
  FILE *archivo = fopen(csv, "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo\n");
    return;
  }

  char *linea = NULL;
  size_t largo = 0;

  while (getline(&linea, &largo, archivo) != -1) {
    
    Tarea *tarea = malloc(sizeof(Tarea));
    
    if (tarea == NULL) {
      printf("Error al asignar memoria\n");
      return;
    }
    int max=5;
    
    char *token = strtok(linea, ",");
    for(int i=0;i<strlen(token);i++){
          if(token[i]=='\n'){
            token[i]='\0';
          }
        }
    if(token!=NULL){
      tarea->nombre = strdup(token);
      
      token = strtok(NULL, ",");
      tarea->prioridad = atoi(token);
      
      tarea->tareasProcedentes = malloc(sizeof(char*) * max);
      if (tarea->tareasProcedentes == NULL) {
        printf("Error al asignar memoria\n");
        return;
      }
      int i=0;
      tarea->numeroTareasProcedentes=0;
      while (1) {
        
        if(i==max-1){
          max*=2;
          tarea->tareasProcedentes=realloc(tarea->tareasProcedentes,sizeof(char*)*max);
        }
        
        token = strtok(NULL, " ");
        if(token==NULL)break;
        if(token[0]=='\n')break;
        for(int i=0;i<strlen(token);i++){
          if(token[i]=='\n'){
            //token=realloc(token,sizeof(char*)*i);
            token[i]='\0';
          }
        }
        tarea->tareasProcedentes[i] = strdup(token);
        tarea->numeroTareasProcedentes++;
        
        i++;
      }

    
      tarea->estado=0;
      tarea->inMap=0;
      if(i==0){
        tarea->inMap=1;
        heap_push(monticulo, tarea, tarea->prioridad);
      }
      
      insertMap(mapa, tarea->nombre, tarea);
    }
    else{
      break;
    }
  }
  
  free(linea);
  fclose(archivo);
  printf("Archivo cargado con exito\n");
}




void tareaRealizada(Heap *monticulo, HashMap *mapa) {
    Tarea *J = heap_top(monticulo);
    if (J == NULL) {
        printf("Heap vacío\n");
        return;
    }
    char *tarea = strdup(J->nombre);
   
    Pair *x = searchMap(mapa,tarea);

    if (x == NULL) {
        
        return;
    }

    Tarea *aux = x->value;
    if (aux == NULL) {
        
        return;
    }

    heap_pop(monticulo);

    aux->estado = 1;

    x = firstMap(mapa);
    if (x == NULL) {
        
        return;
    }

    aux = x->value;

    while (aux != NULL) {
        for (int i = 0; i < aux->numeroTareasProcedentes; i++) {
            if (strcmp(aux->tareasProcedentes[i], tarea) == 0) {
                for (int k = i; k < aux->numeroTareasProcedentes; k++) {
                    aux->tareasProcedentes[k] = aux->tareasProcedentes[k + 1];
                }
                aux->numeroTareasProcedentes--;
            }
        }

        if (aux->numeroTareasProcedentes == 0 && aux->inMap == 0 && aux->estado == 0) {
            heap_push(monticulo, aux, aux->prioridad);
            aux->inMap = 1;
        }

        x = nextMap(mapa);
        if (x != NULL) {
            aux = x->value;
        } else {
            aux = NULL;
        }
    }

   

    free(tarea);
}

void impresionDePrecedentesMapaOG(Heap * monticulo, HashMap *mapa, HashMap *mapaOG){

  //creamos las variables para manipular el heap auxiliar y el mapa original
  Tarea *aja;
  Pair *x;
  Tarea *aux;
  printf("Nombre, Prioridad, Tareas precedentes\n");
 
  while((aja = heap_top(monticulo)) != NULL){
    
    //buscamos los datos del dato que esta en el heap en el mpaa original para su impresion con la totalidad de sus tareas precedentes
   
    x=searchMap(mapaOG, aja->nombre);
    if(x==NULL)break;
    aux=x->value;
   
    printf("%s, %d",aux->nombre,aux->prioridad);
    
    //imprime si tiene tareas pecedentes
    if(aux->numeroTareasProcedentes>0){
      printf(", (");
      for(int i=0;i<aux->numeroTareasProcedentes;i++){
        printf("%s",aux->tareasProcedentes[i]);
        if(i!=aux->numeroTareasProcedentes-1){
          printf(", ");
        }
      }
      printf(")");
    }
    
    
    printf("\n");

    tareaRealizada(monticulo, mapa);


  }

}


void creacionFalsoHeap(HashMap *mapa, HashMap *mapaOG){
  //en esta funcion no tocaremos el mapa original sino que lo usaremos solo para pasarloa  la siguiente funcion
  //creamos un neuvo heap
  Heap *hp=createHeap();
  Tarea *aux=malloc(sizeof(Tarea));
  Pair *x=firstMap(mapaOG);
  aux=x->value;

  if(aux==NULL){
    return;
  }

  while(aux!=NULL){
    if(aux->numeroTareasProcedentes==0 && aux->inMap==1 && aux->estado==0){
      heap_push(hp, aux, aux->prioridad);
    }
    x=nextMap(mapaOG);
    if(x!=NULL) aux=x->value;
    else aux=NULL;
  }
  impresionDePrecedentesMapaOG(hp,mapa,mapaOG);
 

}




void print_tasks(Heap * monticulo, HashMap *mapa){

  //en esta funcion crearemos una copia del mapa original en un mapa aux

  long max=map_size(mapa);
  HashMap *mapAux=createMap(max);

  
  Pair *x=firstMap(mapa);
  if(x==NULL) return; // Verificar que x no es nulo antes de continuar
  
  Tarea *aux=x->value;

  
  // se inserta cada tarea del mapa original en el mapa auxiliar
  while(aux!=NULL){
    
    Tarea *ex=malloc(sizeof(Tarea));
    ex->estado=aux->estado;
    ex->inMap=aux->inMap;
    ex->nombre=strdup(aux->nombre); 
    ex->numeroTareasProcedentes=aux->numeroTareasProcedentes;
    ex->prioridad=aux->prioridad;
    
    ex->tareasProcedentes=malloc(sizeof(char*) * ex->numeroTareasProcedentes);
    for(int i = 0; i < ex->numeroTareasProcedentes; i++) {
      ex->tareasProcedentes[i] = strdup(aux->tareasProcedentes[i]);
    }
    insertMap(mapAux, ex->nombre, ex);
    x=nextMap(mapa);
    if(x!=NULL) aux=x->value;
    else aux=NULL;
  }
  creacionFalsoHeap(mapAux,mapa);

  
}

void agregarTarea(Heap *hp, HashMap *mapa){

  //variables necesarias
  Tarea *tarea=malloc(sizeof(Tarea));
  int max=5;
  tarea->tareasProcedentes=malloc(sizeof(char*)*max);
  tarea->numeroTareasProcedentes=0;
  tarea->inMap=0;
  tarea->nombre=malloc(sizeof(char)*50);

  
  char *nombre=malloc(sizeof(char)*50);
  int prioridad;
  char *k=malloc(sizeof(char)*50);

  //se pide el nombre de la tarea a ingresar
  printf("Ingrese el nombre de la tarea: ");
  scanf("%20s",nombre);
  
  
  if(nombre==NULL){
    printf("Nombre invalido :(");
    return;
  }

  //se verifica que la atrea no exista enel mapa
  Pair *aux=searchMap(mapa, nombre);
 
  if(aux!=NULL){
    printf("Tarea ya ingresada");
    return;
  }

  //se ingresa la prioridad de la tarea
  printf("Ingrese la prioridad de la tarea: ");
  scanf("%d", &prioridad);
  if(prioridad<=0 ){
    printf("Prioridad invalida :(");
    return;
  }

  //se pregunta si quiere que tena tareas precedentes
  printf("Desea agregar tareas precedentes?(si/no)\n");
  scanf("%s",k);
  if(k==NULL){
    printf("Opcion invalida :(");
    return;
  }
  if(strcmp(k,"si")==0){
    while(1){
      
      char *tareaP=malloc(sizeof(char)*50);
      printf("Ingrese la tarea precedente: ");
      scanf("%s",tareaP);
      //se asegura que la tarea exista para poenrla de precedente
      Pair *J=searchMap(mapa, tareaP);
      
      if(J==NULL){
        printf("Tarea no encontrada\n");
      }
      else{
        //se verifica que el largo del arreglo de tareas precedentes nose quede sin espacio
        if(tarea->numeroTareasProcedentes==max){
          max *=2;
          tarea->tareasProcedentes=realloc(tarea->tareasProcedentes,sizeof(char*)*max);
        }
        //se agrega la tarea
        tarea->tareasProcedentes[tarea->numeroTareasProcedentes]=strdup(tareaP);
       
        tarea->numeroTareasProcedentes++;
      }
      free(tareaP);
      printf("Desea agregar otra tarea de precedencia?(si/no)\n");
      scanf("%s",k);
      if(strcmp(k,"no")==0){
        break;
      }
    } 
    
  }
  tarea->nombre=strdup(nombre);
  tarea->prioridad=prioridad;
  tarea->estado=0;
  
  insertMap(mapa, tarea->nombre, tarea);
  //si la tarea no tiene pecedentes se inserta en el heap
  if(tarea->numeroTareasProcedentes==0){
    tarea->inMap=1;
    heap_push(hp, tarea, tarea->prioridad);
  }
  

}

void marcarTareaCompletada(Heap *hp, HashMap *map){
  //variables necesarias
  char *j=malloc(sizeof(char)*50);
  char *k=malloc(sizeof(char)*50);

  //se rellenan la variables necesarias
  printf("Ingrese el nombre de la tarea que desea marcar como completada: ");
  scanf("%49s",j);

  
  Pair *x=searchMap(map, j);
  if(x==NULL || x->value==NULL || x->key==NULL){
    printf("Tarea no encontrada");
    return;
  }

  
  Tarea *aux=x->value;
  if(aux->numeroTareasProcedentes>0){
    printf("¿estás seguro que desea eliminar la tarea?(si/no)");
    scanf("%49s",k);
    if(strcmp(k,"no")==0){
      printf("Ok, la tarea no eliminada :)");
      return;
    }
  }
  aux->estado=1;
  if(aux->inMap==1)
  {
    free(hp);
    hp=createHeap();
    x=firstMap(map);
    if(x==NULL){
      printf("No mapa");
      return;
    }
    aux=x->value;
    
    while(aux!=NULL){
      if(aux->estado==0 && aux->inMap==1 ){
        heap_push(hp, aux, aux->prioridad);
      }
      if(aux->numeroTareasProcedentes>0){
        for(int i=0;i<aux->numeroTareasProcedentes;i++){
          if(strcmp(aux->tareasProcedentes[i],j)==0){
            for(int p=i;p<aux->numeroTareasProcedentes;p++){
              aux->tareasProcedentes[p]=aux->tareasProcedentes[p+1];
            }
            aux->numeroTareasProcedentes--;
          }
        }
      }
      if(aux->estado==0 && aux->inMap==0 && aux->numeroTareasProcedentes==0){
        aux->inMap=1;
        heap_push(hp, aux, aux->prioridad);
      }
      x=nextMap(map);
      
      if(x==NULL)aux=NULL;
      else{
        aux=x->value;
      }
      
    }
    
  }
  else{
    
    x=firstMap(map);
    if(x==NULL){
      printf("No mapa");
      return;
    }
    aux=x->value;

    while(aux!=NULL){
      if(aux->numeroTareasProcedentes>0){
        for(int i=0;i<aux->numeroTareasProcedentes;i++){
          if(strcmp(aux->tareasProcedentes[i],j)==0){
            for(int p=i;p<aux->numeroTareasProcedentes;p++){
              aux->tareasProcedentes[p]=aux->tareasProcedentes[p+1];
            }
            aux->numeroTareasProcedentes--;
          }
        }
      }
      if(aux->estado==0 && aux->inMap==0 && aux->numeroTareasProcedentes==0){
        aux->inMap=1;
        heap_push(hp, aux, aux->prioridad);
      }
      x=nextMap(map);
      
      if(x==NULL)aux=NULL;
      else{
        aux=x->value;
      }
      
    }
  }
  
}




void establecerPrecedencia(Heap *hp, HashMap *map){
  char *j=malloc(sizeof(char)*50);
  char *k=malloc(sizeof(char)*50);

  printf("Ingrese la tarea a la que desee agregar predencia: ");
  scanf("%49s",j);
  Pair *x=searchMap(map, j);
  if(x==NULL || x->value==NULL || x->key==NULL){
    printf("Tarea no encontrada :(");
    return;
  }
  Tarea *aux=x->value;
  if(aux->estado==1){
    printf("Tarea ya realizada");
    return;
  }

  if(aux->numeroTareasProcedentes>0){
    for(int i=0;i<aux->numeroTareasProcedentes;i++){
      if(strcmp(aux->tareasProcedentes[i],j)==0){
        printf("Tarea ya asignada como precedencia");
        return;
      }
    }
  }
  
  printf("Ingrese el nombre de la tarea que desea agregar como precedencia: ");
  scanf("%49s",k);
  if(aux->numeroTareasProcedentes>0){
    for(int i=0;i<aux->numeroTareasProcedentes;i++){
      if(strcmp(aux->tareasProcedentes[i],k)==0){
        printf("Tarea ya asignada como precedencia\n");
        return;
      }
      
    }
  }


  x=searchMap(map, k);
  if(x==NULL || x->value==NULL || x->key==NULL){
    printf("Tarea no encontrada :(\n");
    return;
  }
  aux=x->value;
  if(aux->estado==1){
    printf("Tarea ya realizada\n");
    return;
  }

  if(aux->numeroTareasProcedentes>0){
    for(int i=0;i<aux->numeroTareasProcedentes;i++){
      if(strcmp(aux->tareasProcedentes[i],j)==0){
        printf("Tarea por asignar necesita de la tarea anterior\n");
        return;
      }
      
    }
  }

  x=searchMap(map, j);
  aux=x->value;
  
  if(aux->inMap==1){
    aux->inMap=0;
    free(hp);
    hp=createHeap();
    x=firstMap(map);
    if(x==NULL){
      printf("No mapa\n");
      return;
    }
    aux=x->value;
    
    while(aux!=NULL){
      if(aux->estado==0 && aux->inMap==1 && aux->numeroTareasProcedentes==0){
        heap_push(hp, aux, aux->prioridad);
      }
      
      x=nextMap(map);
      
      if(x==NULL)aux=NULL;
      else{
        aux=x->value;
      }
      
    }
  }
  x=searchMap(map, j);
  
  aux=x->value;
  int o=aux->numeroTareasProcedentes;
  aux->numeroTareasProcedentes++;
  int max=aux->numeroTareasProcedentes*2;
  aux->tareasProcedentes=realloc(aux->tareasProcedentes,sizeof(char*)*max);
  aux->tareasProcedentes[o]=strdup(k);
  

}



int main() {
    Heap* heap = createHeap();
    HashMap* map = createMap(100);
    
    // Variables que necesitaremos

    int opcion;
    
    do {
        printf("\n╭──────────────────────────────────────────────────────────╮");
        printf("\n│                     MENU PRINCIPAL                       │");
        printf("\n├──────────────────────────────────────────────────────────┤");
        printf("\n│ 1. Agregar tarea                                         │");
        printf("\n│ 2. Mostrar tareas                                        │");
        printf("\n│ 3. Establecer precedencia entre dos tareas               │");
        printf("\n│ 4. Marcar tarea como completada                          │");
        printf("\n│ 5. Cargar datos de tareas desde un archivo de texto      │");
        printf("\n│ 0. Salir                                                 │");
        printf("\n╰──────────────────────────────────────────────────────────╯");
        printf("\n\nIngrese una opción: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                
                agregarTarea(heap, map);
                break;
            case 2:
              print_tasks(heap,map);
              break;
            case 3:
                establecerPrecedencia(heap, map);
                break;
            case 4:
                marcarTareaCompletada(heap, map);
                break;
     
            case 5:
              read(heap, map);
              break;
                
            case 0:
              printf("Saliendo del programa...");
              break;
            default:
                printf("Opción no válida. Por favor, intente de nuevo.\n");
        }
    } while (opcion != 0);

    free(heap);
    free(map);

    return 0;
}
