#include "head.h"

// Incluye direct.h solo en Windows
#ifdef _WIN32
#include <direct.h>
#define mkdir(dir, mode) _mkdir(dir)  // Redefinimos mkdir para Windows
#endif

// Función para crear la carpeta 'ED_resultados'
int crearCarpetaResultados(){
    const char* nombreCarpeta = "ED_resultados";

    // Intentar crear la carpeta
    #ifdef _WIN32
        // Para Windows
        int resultado = _mkdir(nombreCarpeta);
    #else
        // Para Linux/macOS (permisos 0755: rwx para el dueño, rx para grupo/otros)
        int resultado = mkdir(nombreCarpeta, 0755);
    #endif

    // Verificar si se creó correctamente o si ya existe
    if(resultado == 0){
        printf("Carpeta '%s' creada exitosamente.\n", nombreCarpeta);
        return 0;
    } else {
        if(errno == EEXIST){
            printf("La carpeta '%s' ya existe.\n", nombreCarpeta);
            return 0;
        } else {
            perror("Error al crear la carpeta");
            return -1;
        }
    }
}

//Funcion para crea la subcarpeta con base a su nombre
int crearSubcarpeta(const char *tipo_mut, const char *tipo_cruza){
    char nombreSubcarpeta[256];
    
    // Usar separador correcto según el SO
    #ifdef _WIN32
        const char separador = '\\';
    #else
        const char separador = '/';
    #endif

    snprintf(nombreSubcarpeta, sizeof(nombreSubcarpeta), "ED_resultados%cED_%s_1_%s", separador, tipo_mut, tipo_cruza);

    errno = 0;  // Resetear errno

    #ifdef _WIN32
        int resultado = _mkdir(nombreSubcarpeta);
    #else
        int resultado = mkdir(nombreSubcarpeta, 0755);
    #endif

    if(resultado == 0){
        printf("Subcarpeta '%s' creada exitosamente.\n", nombreSubcarpeta);
        return 0;
    } else if(errno == EEXIST){
        printf("Subcarpeta '%s' ya existe. Se usará la existente.\n", nombreSubcarpeta);
        return 0;
    } else {
        perror("Error al crear subcarpeta");
        return -1;
    }
}

// Función para verificar si un archivo existe
bool archivoExiste(const char *ruta){
    struct stat buffer;
    return (stat(ruta, &buffer) == 0);
}

// Función para generar un nombre de archivo único (sobrescribe si existe) de los mejores indices
void generarNombreCSV(char *nombreFinal, int tam, char *mutacion, char *cruza){
    snprintf(nombreFinal, tam, "ED_resultados/ED_%s_1_%s.csv", mutacion, cruza);
    
    // Si el archivo existe, se sobrescribirá al abrirlo con "w" en inicializarCSV
    printf("Archivo a generar: %s\n", nombreFinal); // Opcional: para depuración
}
// Función para generar un nombre de archivo único (sobrescribe si existe) de la tabla de los datos a graficar
void generarNombreTablaAGraficarCSV(char *nombreFinal,int tam,char *mutacion,char *cruza,int ID){
    snprintf(nombreFinal, tam, "ED_resultados/ED_%s_1_%s/ED_%s_1_%s_ID_%d.csv",mutacion,cruza
        ,mutacion,cruza,ID);
}

void inicializarCSV(const char *nombreArchivo,int tipo){
    FILE *archivo = fopen(nombreArchivo, "w");
    if(archivo == NULL){
        perror("Error al crear el archivo CSV");
        return;
    }
    if(tipo==1)
        fprintf(archivo, "ID,seed,poblacion,mutacion,cruza,mejor,peor,promedio,x1,x2,x3,x4\n");
    else
        fprintf(archivo, "Generacion,Mejor Fitness,Peor Fitness,Fitness Promedio\n");
    fclose(archivo);
}

void guardarDatosMejores(const char *nombreArchivo,int seed,int tam_pobla,float mutacion,float cruza,
    float mejor,float peor,float promedio,float x1,float x2,float x3,float x4,int ID){
    FILE *archivo = fopen(nombreArchivo, "a");  // Modo 'append'
    if(archivo == NULL){
        perror("Error al abrir el archivo CSV");
        return;
    }
    fprintf(archivo, "%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        ID,seed,tam_pobla,mutacion,cruza,mejor,peor,promedio,x1,x2,x3,x4);
    fclose(archivo);
}

void guardarDatosGeneracion(const char *nombreArchivo,int generacion,
    float mejor,float peor,float promedio){
    FILE *archivo = fopen(nombreArchivo, "a");  // Modo 'append'
    if(archivo == NULL){
        perror("Error al abrir el archivo CSV");
        return;
    }
    fprintf(archivo, "%d,%f,%f,%f\n",generacion,mejor,peor,promedio);
    fclose(archivo);
}

// Función para crear el script de Python si no existe
int crearScriptPython() {
    const char* nombreArchivo = "graficadora.py";
    FILE *archivo;
    // Verificar si el archivo ya existe
    if (archivoExiste(nombreArchivo)) {
        printf("El archivo '%s' ya existe. No se sobrescribirá.\n", nombreArchivo);
        return 0;
    }
    // Crear el archivo y escribir el contenido
    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        perror("Error al crear el archivo Python");
        return -1;
    }
    fprintf(archivo, 
        "import tkinter as tk\n"
        "from tkinter import filedialog, messagebox\n"
        "import pandas as pd\n"
        "import matplotlib.pyplot as plt\n"
        "from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg\n\n"
        "class VisualizadorED:\n"
        "    def __init__(self, root):\n"
        "        self.root = root\n"
        "        self.root.title(\"Visualizador de Resultados de ED\")\n"
        "        self.root.geometry(\"400x200\")\n"
        "        self.root.protocol(\"WM_DELETE_WINDOW\", self.salir_seguro)\n"
        "        \n"
        "        plt.switch_backend('TkAgg')\n"
        "        \n"
        "        self.lbl_instruccion = tk.Label(\n"
        "            self.root, \n"
        "            text=\"Seleccione el archivo CSV con los resultados\\nque desea graficar\",\n"
        "            font=('Arial', 12)\n"
        "        )\n"
        "        self.lbl_instruccion.pack(pady=20)\n"
        "        \n"
        "        self.btn_seleccionar = tk.Button(\n"
        "            self.root, \n"
        "            text=\"Buscar archivo CSV\", \n"
        "            command=self.seleccionar_archivo,\n"
        "            font=('Arial', 10),\n"
        "            height=2,\n"
        "            width=20\n"
        "        )\n"
        "        self.btn_seleccionar.pack()\n"
        "        \n"
        "        self.btn_salir = tk.Button(\n"
        "            self.root, \n"
        "            text=\"Salir\", \n"
        "            command=self.salir_seguro,\n"
        "            font=('Arial', 10)\n"
        "        )\n"
        "        self.btn_salir.pack(pady=10)\n"
        "        \n"
        "        self.fig = None\n"
        "        self.ventana_grafico = None\n\n"
        "    def seleccionar_archivo(self):\n"
        "        archivo_csv = filedialog.askopenfilename(\n"
        "            title=\"Seleccione el archivo CSV\",\n"
        "            filetypes=[(\"CSV\", \"*.csv\")],\n"
        "            initialdir=\"./ED_resultados\"\n"
        "        )\n"
        "        if archivo_csv:\n"
        "            self.graficar_datos(archivo_csv)\n\n"
        "    def graficar_datos(self, ruta_csv):\n"
        "        try:\n"
        "            if self.ventana_grafico:\n"
        "                self.ventana_grafico.destroy()\n"
        "                plt.close(self.fig)\n"
        "            \n"
        "            datos = pd.read_csv(ruta_csv)\n"
        "            self.fig = plt.figure(figsize=(10, 6))\n"
        "            ax = self.fig.add_subplot(111)\n"
        "            \n"
        "            ax.plot(datos['Generacion'], datos['Mejor Fitness'], 'g:', label='Mejor Fitness')  # Línea verde punteada\n"
        "            ax.plot(datos['Generacion'], datos['Peor Fitness'], 'r:', label='Peor Fitness')    # Línea roja punteada \n"
        "            ax.plot(datos['Generacion'], datos['Fitness Promedio'], 'b:', label='Fitness Promedio')  # Línea azul punteada\n"
        "            \n"
        "            ax.set_title(f'Evolución del Fitness\\n{ruta_csv.split(\"/\")[-1]}')\n"
        "            ax.set_xlabel('Generación')\n"
        "            ax.set_ylabel('Fitness')\n"
        "            ax.grid(True, linestyle='--', alpha=0.7)\n"
        "            ax.legend()\n"
        "            \n"
        "            self.ventana_grafico = tk.Toplevel(self.root)\n"
        "            self.ventana_grafico.title(f\"Gráfico - {ruta_csv.split('/')[-1]}\")\n"
        "            \n"
        "            canvas = FigureCanvasTkAgg(self.fig, master=self.ventana_grafico)\n"
        "            canvas.draw()\n"
        "            canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)\n"
        "            \n"
        "            btn_cerrar = tk.Button(\n"
        "                self.ventana_grafico, \n"
        "                text=\"Cerrar Gráfico\", \n"
        "                command=self.cerrar_grafico\n"
        "            )\n"
        "            btn_cerrar.pack(pady=5)\n"
        "            \n"
        "        except Exception as e:\n"
        "            messagebox.showerror(\"Error\", f\"Error al leer el archivo:\\n{str(e)}\")\n\n"
        "    def cerrar_grafico(self):\n"
        "        if self.ventana_grafico:\n"
        "            self.ventana_grafico.destroy()\n"
        "        if self.fig:\n"
        "            plt.close(self.fig)\n\n"
        "    def salir_seguro(self):\n"
        "        self.cerrar_grafico()\n"
        "        self.root.quit()\n"
        "        self.root.destroy()\n"
        "        plt.close('all')\n\n"
        "if __name__ == \"__main__\":\n"
        "    root = tk.Tk()\n"
        "    app = VisualizadorED(root)\n"
        "    try:\n"
        "        root.mainloop()\n"
        "    except KeyboardInterrupt:\n"
        "        app.salir_seguro()\n"
    );
    fclose(archivo);
    printf("Archivo '%s' creado exitosamente.\n", nombreArchivo);
    return 0;
}

// Función para crear el requirements.txt si no existe
int crearRequirementsTxt() {
    const char* nombreArchivo = "requirements.txt";
    FILE *archivo;
    if (archivoExiste(nombreArchivo)) {
        printf("El archivo '%s' ya existe. No se sobrescribirá.\n", nombreArchivo);
        return 0;
    }
    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        perror("Error al crear requirements.txt");
        return -1;
    }
    fprintf(archivo,
        "contourpy==1.3.1\n"
        "cycler==0.12.1\n"
        "fonttools==4.57.0\n"
        "kiwisolver==1.4.8\n"
        "matplotlib==3.10.1\n"
        "numpy==2.2.4\n"
        "packaging==24.2\n"
        "pandas==2.2.3\n"
        "pillow==11.1.0\n"
        "pyparsing==3.2.3\n"
        "python-dateutil==2.9.0.post0\n"
        "pytz==2025.2\n"
        "six==1.17.0\n"
        "tzdata==2025.2\n"
    );
    fclose(archivo);
    printf("Archivo '%s' creado exitosamente.\n", nombreArchivo);
    return 0;
}