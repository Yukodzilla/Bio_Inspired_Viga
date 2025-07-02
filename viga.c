#include "head.h"

int main(){
    printf("\n=== OPTIMIZADOR DE VIGAS SOLDADAS (Welded Beam) ===\n");
    printf("Este programa usa Evolucion Diferencial (DE) para\n");
    printf("encontrar el diseno optimo de una viga soldada que:\n");
    printf("- Minimiza el costo de fabricacion\n");
    printf("- Cumple restricciones de ingenieria\n\n");

    printf("CONFIGURACION:\n");
    printf("- Poblaciones: 50, 100, 200\n");
    printf("- Factores de mutacion (F): 0.01, 0.1, 0.2\n");
    printf("- Tasas de cruza (CR): 0.6, 0.8, 0.9\n");
    printf("- Semillas aleatorias: 10 valores distintos\n");
    printf("- Variantes DE: best/1/bin, best/1/exp, rand/1/bin, rand/1/exp\n\n");

    printf("SALIDAS:\n");
    printf("- Resultados en archivos CSV organizados\n");
    printf("- Datos de convergencia generacional\n");
    printf("- Listo para visualizacion con Python\n");
    printf("===============================================\n\n");
    int poblacion[]={50,100,200},MAX_GEN=100,tipo_ec=1,
    seed[]={7,11,13,17,19,23,29,31,37,41},
    i=0,j=0,k=0,l=0,guardarDatos=1,ID=0,existeRes=-1,existeSubRes=-1;
    double F[]={0.01,0.1,0.2},CR[]={0.6,0.8,0.9},esperado=1.725;//1.725 o 1.74 o incluso 1000
    char *tipo_mut,*tipo_cruza;
    // Generar nombre único para el CSV
    char nombreCSV[256];

    tipo_mut=opcionesCruzYMut("tipo de mutacion","best","rand");
    tipo_cruza=opcionesCruzYMut("tipo de cruza","bin","exp");

    if(crearCarpetaResultados()==0) {
        existeRes=0;
        // Crear archivos auxiliares si no existen
        existeSubRes=crearSubcarpeta(tipo_mut,tipo_cruza);
        crearScriptPython();
        crearRequirementsTxt();
        printf("Operacion completada.\n");
    } 
    else
        printf("Fallo la creacion de la carpeta.\n");
    generarNombreCSV(nombreCSV,sizeof(nombreCSV),tipo_mut,tipo_cruza);
    // Inicializar archivo CSV con cabeceras
    inicializarCSV(nombreCSV,1);

    imprimeTabla(seed[l],poblacion[i],F[j],CR[k],1.0,1.0,1.0,1,ID);
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            for(k=0;k<3;k++){
                for(l=0;l<10;l++){
                    EvolucionDiferencial(poblacion[i],MAX_GEN,tipo_ec,seed[l],F[j],CR[k],
                        tipo_mut,tipo_cruza,nombreCSV,guardarDatos,&ID,esperado);
                }
            }
        }
    }
    //A partir de aqui el programa se dedica a crear
    if (existeRes==0 && existeSubRes==0) {
        ID=0;//Inicializamos solo por los jajaja's
        guardarDatos=0;//Con esto escribimos otros datos en archivo y no los mismo
        printf("Leyendo el archivo: %s",nombreCSV);
        
        FILE *archivo = fopen(nombreCSV,"r");
        if (archivo==NULL) {
            perror("Error al abrir el archivo");
            return -1;
        }

        char linea[1024];
        int semilla,tam_poblacion;
        float mutacion,cruza;

        // Leer y descartar la cabecera
        if (fscanf(archivo,"%[^\n]\n",linea) != 1) {
            printf("\nError al leer la cabecera o archivo vacío");
            fclose(archivo);
            return -1;
        }

        // Leer datos con delimitadores de coma
        while (fscanf(archivo,"%d,%d,%d,%f,%f,%*f,%*f,%*f,%*f,%*f,%*f,%*f",
                    &ID,&semilla,&tam_poblacion,&mutacion,&cruza)==5) {
            generarNombreTablaAGraficarCSV(nombreCSV,sizeof(nombreCSV),tipo_mut,tipo_cruza,ID);
            // Inicializar archivo CSV con cabeceras
            inicializarCSV(nombreCSV,2);
            EvolucionDiferencial(tam_poblacion,MAX_GEN,tipo_ec,semilla,mutacion,cruza,
                        tipo_mut,tipo_cruza,nombreCSV,guardarDatos,&ID,esperado);
            printf("\nArchivo %s creado",nombreCSV);
        }

        fclose(archivo);
    }
    printf("\n\nFin del programa...\n");
    return 0;
}
