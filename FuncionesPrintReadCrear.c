#include "head.h"

/*Funciones Crear e imprimir*/
int imprimeTabla(int seed,int tam_pobla,float mutacion,float cruza,
    float mejor,float peor,float prom,int encabezado,int ID){
    if(encabezado==1)
        printf("| ID |seed|poblacion|mutacion| cruza  | mejor  |  peor  |promedio|   x1   |   x2   |   x3   |   x4   |\n");
    else
        printf("|%4d| %2d |   %3d   |  %.2f  |  %.2f  |%f|%f|%f|"
            ,ID,seed,tam_pobla,mutacion,cruza,mejor,peor,prom);
    return 0;
}
int imprimeVector(vector *v){
    printf("%f|%f|%f|%f|\n",v->x_1,v->x_2,v->x_3,v->x_4);
    return 0;
}
char* opcionesCruzYMut(char *tipo,char *opcion1,char *opcion2){
    char *opcion=NULL;
    int seleccion;
    do{
        printf("\nElija el tipo de %s: ",tipo);
        printf("\n1.- %s",opcion1);
        printf("\n2.- %s",opcion2);
        printf("\nDigite el numero su opcion: ");
        scanf("%d",&seleccion);
        if(seleccion==1){
            opcion=opcion1;
            break;
        }
        else if(seleccion==2){
            opcion=opcion2;
            break;
        }
        else printf("\nSeleccion invalida, por favor...");
    }while(1);
    return opcion;
}
int creaPoblacion(poblacion *P,int tam_P) {//O(tam_P)
    P->vectores = (vector **)malloc(tam_P * sizeof(vector *));  // Asignar memoria para tam_P punteros a vector
    P->tam_P = tam_P;
    for (int i = 0; i < tam_P; i++)
        P->vectores[i] = (vector *)malloc(sizeof(vector));  // Asignar memoria para cada vector
    return 0;
}
int liberarPoblacion(poblacion *P) {//O(tam_P)
    if(P==NULL) return -1;  // Verificar que la población existe

    // Liberar cada vector de la población
    for(int i = 0; i < P->tam_P; i++)
        if(P->vectores[i] != NULL)
            free(P->vectores[i]);// Liberar la estructura vector

    free(P->vectores);// Liberar el array de punteros a vectores
    P->vectores = NULL;
    P->tam_P=0;
    return 0;
}
int imprimirVector(vector V){//O(1)
    printf("[%f, %f, %f, %f]",V.x_1,V.x_2,V.x_3,V.x_4);
    return 0;
}
int imprimirPoblacion(poblacion P){//O(tam_P)
    if(P.vectores==NULL) {
        printf("\nPoblacion Vacia");
        return -1;
    }
    printf("\nPoblacion"
            "\n|"
            "\nv"
            );
    for(int i=0;i<P.tam_P;i++){
        printf("\nV[%3d]->",i);
        imprimirVector(*P.vectores[i]);
        printf(" - Apt=%.2f",P.vectores[i]->apt);
    }
    return 0;
}
int copyVector(vector Original,vector *Copia){//O(1)
    Copia->apt=Original.apt;
    Copia->x_1=Original.x_1;
    Copia->x_2=Original.x_2;
    Copia->x_3=Original.x_3;
    Copia->x_4=Original.x_4;
    return 0;
}
int inicializaVector(vector *V){//O(1)
    if(V==NULL){
        printf("\nError en inicializaVector");
        return -1;
    }
    double decimal;
    int n=2;
    V->x_1 = rand()%n;
    decimal=(double) rand()/(double)RAND_MAX;
    V->x_1 += decimal;

    n=10;
    V->x_2 = rand()%n;
    decimal=(double) rand()/(double)RAND_MAX;
    V->x_2 += decimal;

    n=10;
    V->x_3 = rand()%n;
    decimal=(double) rand()/(double)RAND_MAX;
    V->x_3 += decimal;

    n=2;
    V->x_4 = rand()%n;
    decimal=(double) rand()/(double)RAND_MAX;
    V->x_4 += decimal;

    if(V->x_4<V->x_1){
        decimal=V->x_1;
        V->x_1=V->x_4;
        V->x_4=decimal;
    }
    return 0;
}
int correccionVector(vector *V){//O(1)
    if(V==NULL){
        printf("\nError en correccionVector");
        return -1;
    }
    double decimal;
    if(V->x_1>2.0) V->x_1 = 2.0;
    else if (V->x_1<0.1) V->x_1 = 0.1;
    if(V->x_2>10.0) V->x_2 = 10.0;
    else if (V->x_2<0.1) V->x_2 = 0.1;
    if(V->x_3>10.0) V->x_3 = 10.0;
    else if (V->x_3<0.1) V->x_3 = 0.1;
    if(V->x_4>2.0) V->x_4 = 2.0;
    else if (V->x_4<0.1) V->x_4 = 0.1;
    if(V->x_4<V->x_1){
        decimal=V->x_1;
        V->x_1=V->x_4;
        V->x_4=decimal;
    }
    return 0;
}
int inicializaPoblacion(poblacion *P) {//O(tam_P)
    if(P==NULL) {
        printf("\nPoblacion Vacia");
        return -1;
    }
    for (int i = 0; i < P->tam_P; i++)
        inicializaVector(P->vectores[i]);//O(1)
    return 0;
}
