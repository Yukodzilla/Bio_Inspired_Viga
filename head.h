#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <sys/stat.h>  // Para mkdir()
#include <errno.h>     // Para manejar errores

#define Tao_max 13600.0
#define Sigma_max 30000.0
#define Delta_max 0.25
#define P_Carga 6000.0
#define L 14.0
#define E 30000000.0
#define G 12000000.0
#define tipo_PC 1

typedef struct vector {
    double x_1,x_2,x_3,x_4;
    double apt;
} vector;

typedef struct poblacion {
    vector **vectores;
    int tam_P;
} poblacion;

/*Funciones Crear e imprimir*/
int creaPoblacion(poblacion *P,int tam_P); //O(tam_P)
int liberarPoblacion(poblacion *P);//O(tam_P)
int imprimirVector(vector V);//O(1)
int imprimirPoblacion(poblacion P);//O(tam_P)
int copyVector(vector Original,vector *Copia);//O(1)
int inicializaVector(vector *V);//O(1)
int inicializaPoblacion(poblacion *P);//O(tam_P)
char* opcionesCruzYMut(char *tipo,char *opcion1,char *opcion2);
int correccionVector(vector *V);//O(1)
int imprimeTabla(int seed,int tam_pobla,float mutacion,float cruza,
    float mejor,float peor,float prom,int encabezado,int ID);//O(1)
int imprimeVector(vector *v);//O(1)

/*Funciones ED*/
int MutacionVector(poblacion P,char *tipo_mut,int indice,double F,vector *v_i);
int CruzaVector(poblacion P,vector *u_i,vector v_i,char *tipo_cruza,int indice,double CR,int tipo_ec);// O(1)
double FuncionVector(vector *V,int tipo_ec);//O(1)
int EvaluarPoblacion(poblacion P,int tipo_ec,double *min_aptitud,double *max_aptitud,double *promedio);//O(tam_P)
int comparar_aptitud(const void *a,const void *b);//O(1)
int EvolucionDiferencial(int tam_P,int MAX_GEN,int tipo_ec,int seed,double F,double CR,
    char *tipo_mut,char *tipo_cruza,const char *nombreArchivo,int guardarDatos,int *ID,double esperado);//O(MAX_GEN*tam_P)

// Restricciones y funciones del problema a optimizar
double Restricciones(vector V);//O(1)
double Constraint_G1(vector V);//O(1)
double Constraint_G2(vector V);//O(1)
double Constraint_G3(vector V);//O(1)
double Constraint_G4(vector V);//O(1)
double Constraint_G5(vector V);//O(1)
double Constraint_G6(vector V);//O(1)
double Constraint_G7(vector V);//O(1)
double function_Tao(vector V);//O(1)
double function_Tao_1(vector V);//O(1)
double function_Tao_2(vector V);//O(1)
double function_J(vector V);//O(1)
double function_R(vector V);//O(1)
double function_M(vector V);//O(1)
double function_Sigma(vector V);//O(1)
double function_Delta(vector V);//O(1)
double function_Pc(vector V);//O(1)
int Constraint_Xn(double x,double limite_inf,double limite_sup);//O(1)
int verificaLimites(vector V);//O(1)

// Funciones para manejo de archivos y directorios
int crearCarpetaResultados();
int crearSubcarpeta(const char *tipo_mut,const char *tipo_cruza);
bool archivoExiste(const char *ruta);
void generarNombreCSV(char *nombreFinal,int tam,char *mutacion,char *cruza);
void generarNombreTablaAGraficarCSV(char *nombreFinal,int tam,char *mutacion,char *cruza,int ID);
void inicializarCSV(const char *nombreArchivo,int tipo);
void guardarDatosMejores(const char *nombreArchivo,int seed,int tam_pobla,float mutacion,float cruza,
                        float mejor,float peor,float promedio,float x1,float x2,float x3,float x4,int ID);
void guardarDatosGeneracion(const char *nombreArchivo,int generacion,float mejor,float peor,float promedio);

// Prototipos para generación de scripts
int crearScriptPython();
int crearRequirementsTxt();

#include "FuncionesPrintReadCrear.c"
#include "FuncionesED.c"
#include "FuncionesConstrains.c"
#include "archivos.c"
#endif 