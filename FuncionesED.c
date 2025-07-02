#include "head.h"

/*Funciones ED*/
int EvolucionDiferencial(int tam_P,int MAX_GEN,int tipo_ec,int seed,double F,double CR,
    char *tipo_mut,char *tipo_cruza,const char *nombreArchivo,int guardarDatos,int *ID,double esperado){//O(MAX_GEN*tam_P)
    poblacion P,New_P;
    vector v_i,u_i;
    /*
    tam_P: Tamaño de la población (ej: 50)
    F: Factor de escala (ej: 0.8)
    CR: Probabilidad de cruza (ej: 0.9)
    MAX_GEN: Número de generaciones (ej: 100)
    D: Dimensión del problema (se omitio esta variable ya que para este problema D=4)
    */
    int i,gen=0,indice,bandera=1;
    double promedio,mejor,peor;
    srand(seed);

    creaPoblacion(&P,tam_P);// población con 100 vectores de tamaño 2
    creaPoblacion(&New_P,tam_P);
    inicializaPoblacion(&P);
    EvaluarPoblacion(P,tipo_ec,&mejor,&peor,&promedio);
    while (gen<MAX_GEN){//...se ejecuta MAX_GEN*tam_P-veces si no rompe antes
        for(i=0;i<tam_P;i++){//...se ejecuta tam_P-veces
            //Se aplica la mutacion y cruza dependiendo de su tipo seleccionado
            MutacionVector(P,tipo_mut,i,F,&v_i);
            CruzaVector(P,&u_i,v_i,tipo_cruza,i,CR,tipo_ec);//O(1)
            if(u_i.apt<=P.vectores[i]->apt)//seleccion
                copyVector(u_i,New_P.vectores[i]);
            else
                copyVector(*P.vectores[i],New_P.vectores[i]);
            if(verificaLimites(*New_P.vectores[i])==-1)//Desreferencio un puntero y verifico los limites
                correccionVector(New_P.vectores[i]);
        }
        for(i=0;i<tam_P;i++)
            copyVector(*New_P.vectores[i],P.vectores[i]);
        indice=EvaluarPoblacion(P,tipo_ec,&mejor,&peor,&promedio);

        if(gen==(MAX_GEN-1) && mejor<=esperado && guardarDatos==1){
            (*ID)++;
            imprimeTabla(seed,tam_P,F,CR,mejor,peor,promedio,0,*ID);
            guardarDatosMejores(nombreArchivo,seed,tam_P,F,CR,mejor,peor,promedio,
                P.vectores[indice]->x_1,P.vectores[indice]->x_2,
                P.vectores[indice]->x_3,P.vectores[indice]->x_4,*ID);
        }
        else if(guardarDatos==0){
            guardarDatosGeneracion(nombreArchivo,gen+1,mejor,peor,promedio);
        }
        gen++;
    }
    if((mejor<=esperado && mejor > 0.0) && indice>=0 && guardarDatos==1) imprimeVector(P.vectores[indice]);

    liberarPoblacion(&P);
    liberarPoblacion(&New_P);
    return 0;
}

int MutacionVector(poblacion P,char *tipo_mut,int indice,double F,vector *v_i){// O( tam_P*log(tam_P) + #veces condition es 1)
    vector *r1,*r2,*r3,*r_aux;
    int i;
    r_aux=P.vectores[indice];
    if(strcmp(tipo_mut,"best")==0){
        double aux[P.tam_P][2];
        for(i=0;i<P.tam_P;i++){
            aux[i][0]=i;//indice
            aux[i][1]=P.vectores[i]->apt;//aptitud
        }
        qsort(aux, P.tam_P, sizeof(aux[0]), comparar_aptitud);//Ordeno respecto a la aptitud O(tam_P*log(tam_P))
        i=0;
        do{
            r1=P.vectores[(int)aux[i][0]];
            if(r1==r_aux) i=((i+1)%P.tam_P);
            else{
                r2=P.vectores[rand()%P.tam_P];
                r3=P.vectores[rand()%P.tam_P];
                if(r_aux!=r2 && r_aux!=r3 && r1!=r2 && r1!=r3 && r2!=r3) break;
            }
        }while(1);
    }
    else if(strcmp(tipo_mut,"rand")==0){
        do{
            r1=P.vectores[rand()%P.tam_P];
            r2=P.vectores[rand()%P.tam_P];
            r3=P.vectores[rand()%P.tam_P];
            if(r_aux!=r1 && r_aux!=r2 && r_aux!=r3 && r1!=r2 && r1!=r3 && r2!=r3) break;
        }while(1);
    }
    v_i->x_1 = r1->x_1 + (F*(r2->x_1 - r3->x_1));
    v_i->x_2 = r1->x_2 + (F*(r2->x_2 - r3->x_2));
    v_i->x_3 = r1->x_3 + (F*(r2->x_3 - r3->x_3));
    v_i->x_4 = r1->x_4 + (F*(r2->x_4 - r3->x_4));
    r_aux=NULL;
    r1=NULL;
    r2=NULL;
    r3=NULL;
    return 0;
}
int CruzaVector(poblacion P,vector *u_i,vector v_i,char *tipo_cruza,int indice,double CR,int tipo_ec){// O(1)
    int j,j_rand=rand()%4;
    double proba;
    if(strcmp(tipo_cruza,"bin")==0)
        for(j=0;j<4;j++){//...se ejecuta 4-veces
            proba=(double) rand()/(double)RAND_MAX;
            if(proba<CR || j==j_rand){
                     if(j==0) u_i->x_1 = v_i.x_1;
                else if(j==1) u_i->x_2 = v_i.x_2;
                else if(j==2) u_i->x_3 = v_i.x_3;
                else if(j==3) u_i->x_4 = v_i.x_4;
            }
            else{
                     if(j==0) u_i->x_1 = P.vectores[indice]->x_1;
                else if(j==1) u_i->x_2 = P.vectores[indice]->x_2;
                else if(j==2) u_i->x_3 = P.vectores[indice]->x_3;
                else if(j==3) u_i->x_4 = P.vectores[indice]->x_4;
            }
        }
    else if(strcmp(tipo_cruza,"exp")==0){
        int ind1, ind2, n = 0;
        ind1 = rand() % 4;  // Índice inicial aleatorio [0,3]
        // Calcular longitud del bloque
        proba = (double)rand() / (double)RAND_MAX;
        while (proba < CR && n < 4) {
            n++;
            proba = (double)rand() / (double)RAND_MAX;
        }
        ind2 = (ind1 + n) % 4;
        // Copiar el bloque
        for(j=0;j<4;j++){
            if ((ind1<=ind2 && j >= ind1 && j<=ind2) || //si j esta en el intervalo [ind1,ind2] con ind1<ind2
                (ind1 > ind2 && (j >= ind1 || j<=ind2)) ){//si j esta en el intervalo [0,ind2]U[ind1,3] con ind2<ind1
                     if(j==0) u_i->x_1 = v_i.x_1;
                else if(j==1) u_i->x_2 = v_i.x_2;
                else if(j==2) u_i->x_3 = v_i.x_3;
                else if(j==3) u_i->x_4 = v_i.x_4;
            }
            else {
                     if(j==0) u_i->x_1 = P.vectores[indice]->x_1;
                else if(j==1) u_i->x_2 = P.vectores[indice]->x_2;
                else if(j==2) u_i->x_3 = P.vectores[indice]->x_3;
                else if(j==3) u_i->x_4 = P.vectores[indice]->x_4;
            }
        }
    }
    FuncionVector(u_i, tipo_ec);
    return 0;
}
double FuncionVector(vector *V,int tipo_ec){//O(1)
    double F=-1.0,x_1,x_2,x_3,x_4,constraints;
    if(V==NULL){
        printf("\nError en FuncionVector");
        return F;
    }
    x_1=V->x_1;
    x_2=V->x_2;
    x_3=V->x_3;
    x_4=V->x_4;
    if(tipo_ec==1) //Funcion del articulo de evaporacion del agua
        F=1.10471*(x_1*x_1)*x_2+0.04811*x_3*x_4*(14.0+x_2);
    else if(tipo_ec==2) //Funcion del articulo Multi-Swarm
        F=1.10471*(x_1*x_1)*x_2+0.04811*x_3*x_4*(14.0-x_2);
    constraints=Restricciones(*V);
    V->apt=F+constraints;
    return V->apt;
}
int EvaluarPoblacion(poblacion P, int tipo_ec, double *min_aptitud, double *max_aptitud, double *promedio) {
    double val, min_val = DBL_MAX, max_val = DBL_MIN, suma = 0.0;
    int indice_mejor = -1,i;  // Índice del vector con mejor (mínima) aptitud

    if(P.vectores == NULL) {
        printf("\nError en EvaluarPoblacion");
        return -1;
    }

    for(i=0;i<P.tam_P;i++) {
        val = FuncionVector(P.vectores[i], tipo_ec);
        // Buscamos el mínimo (para minimización)
        if (val < min_val) {
            min_val = val;
            indice_mejor = i;
        }
        // Buscamos el máximo (solo para referencia)
        if (val > max_val)
            max_val = val;
        suma += val;
    }

    *min_aptitud = min_val;  // Mejor aptitud (mínimo)
    *max_aptitud = max_val;  // Peor aptitud (máximo)
    *promedio = suma / P.tam_P;

    return indice_mejor;  // Devuelve el índice del mejor vector
}
int comparar_aptitud(const void *a, const void *b) {//O(1)
    const double *filaA = (const double *)a;
    const double *filaB = (const double *)b;

    if (filaA[1] < filaB[1]) return -1;  // de menor a mayor
    if (filaA[1] > filaB[1]) return 1;
    return 0;
}
