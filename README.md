INSTRUCTIVO PARA EJECUTAR EL PROYECTO DE VIGAS SOLDADAS

Requisitos previos:

Tener instalado GCC (versión 9.4 o superior)
Tener Python 3.8 o superior instalado
Tener pip actualizado

1. COMPILACIÓN DEL PROGRAMA:
Ejecute el siguiente comando en terminal (válido para Windows y Ubuntu/Linux):
gcc viga.c -o viga -lm

2. EJECUCIÓN DEL PROGRAMA:
Para Windows:
.\viga

Para Ubuntu/Linux:
./viga

*Nota: El programa generará automáticamente archivos CSV con los resultados en la carpeta ED_resultados/

3. CONFIGURACIÓN DEL ENTORNO VIRTUAL PYTHON:
Cree el entorno virtual con:
Para Windows:
python -m venv env
Para Ubuntu/Linux:
python3 -m venv env

Active el entorno:
Para Windows:
.\env\Scripts\activate

Para Ubuntu/Linux:
source env/bin/activate

4. INSTALACIÓN DE DEPENDENCIAS:
Instale los paquetes necesarios con:
pip install -r requirements.txt

5. VISUALIZACIÓN DE RESULTADOS:
Ejecute el visualizador:
Para Windows:
python graficadora.py

Para Ubuntu/Linux:
python3 graficadora.py

6. ESTRUCTURA DE ARCHIVOS:
Los resultados se organizan en subcarpetas dentro de ED_resultados/:

ED_best_1_bin/
ED_best_1_exp/
ED_rand_1_bin/
ED_rand_1_exp/

*Ejemplo de archivo válido para graficar:ED_best_1_bin_ID_1.csv
*Advertencia: No grafique archivos directamente de la carpeta ED_resultados/ (debe usar los de las subcarpetas)

7. PERSONALIZACIÓN:
Si desea ajustar el criterio de aceptación:

Edite el archivo viga.c
Modifique el valor de la variable "esperado" (se encuentra en la línea 7)
Recompile el programa

