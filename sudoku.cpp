#include <iostream> 
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#define dimension 4 // La dimension de nuestro sudoku --- En caso de 9x9 cambiar 4 por el 9

using namespace std;
/*OBS: Compilado con 
Comando: g++ sudoku.cpp -o sudoku 
Sistema Operativo: Linux Ubuntu 17.04 OS..
Terminal: zsh.
 */
vector<vector<int>> file(const char*); // declaracion de funcion abrir archivo

class Sudoku
{
private:
    struct nodo
    {
        int data; // Valor del nodo
        nodo *vecinos_fila[dimension]; // vector que contiene todos los vecinos
        nodo *vecinos_columna[dimension];
        nodo *vecinos_zona[((dimension/2) * (dimension/2) - (dimension-1))]; // (x^2 - 4x - 4) / 4 siendo x=dimension.. Ecuacion para hallar el numero de vecinos Zona que no se encuentran ni en vecinos_filas ni en vecinos_columnas
        bool modificado; // variable para verificar si se modifico el nodo alguna vez,, para backtraking
        bool borde; // para saber si es un borde o no. Porque si es un borde se verifica distinto los vecinos de la zona
    };
    vector<vector<nodo>> tablero; // Nuestro tablero a resolver 
    int colores[dimension]; // vector de los posibles colores segun el numero de dimension (ejemplo si dimension=4. colores va de 1 a 4)
    void cargarColores() // guardamos los colores(1 a Filas) en el vector colores
    {
        for (int i = 1; i <= dimension; i++)
        {
            colores[i - 1] = i;
        }
    }

    void cargarFila(nodo *temp, int fila, int columna)
    {
        //temp->vecinos_fila.resize(dimension); // redimensionamos nuestro array
        for (int i=0,j = 0; i < dimension; i++)
        {
            if(columna != i)
              temp->vecinos_fila[j++] = &tablero[fila][i];
              //temp->vecinos_fila[i].assing(tablero[fila][0],tablero[fila][4]);
        }
        //temp->vecinos_fila[dimension-1] = NULL; // colocamos el null al final de la matriz 
    }
    
    void cargarColum(nodo *temp, int fila, int columna){
        
        for (int i=0, j = 0; i < dimension; i++)
        {
            if(fila != i){
                temp->vecinos_columna[j++] = &tablero[i][columna];
            } 
        }
        //temp->vecinos_columna[dimension-1] = NULL;
    }
    void cargarZona(nodo *temp, int fila, int columna){
        if (columna != dimension-1 && fila != dimension-1)
        {
            if(fila %2 == 0){
                if (columna%2 == 0)
                {
                    temp->vecinos_zona[0] = &tablero[fila+1][columna+1];    
                }else
                {
                    temp->vecinos_zona[0] = &tablero[fila+1][columna-1];
                }
            }else
            {
                if (columna %2 == 0)
                {
                    temp->vecinos_zona[0] = &tablero[fila-1][columna+1];        
                }else
                {
                    temp->vecinos_zona[0] = &tablero[fila-1][columna-1];
                }
                
                
                
            }
            
        }else{
            temp->borde = true;
            if(fila %2 == 0){
                temp->vecinos_zona[0] = &tablero[fila+1][columna-1];    
            }else
            {
                temp->vecinos_zona[0] = &tablero[fila-1][columna-1];    
            }
        }
        //temp->vecinos_zona[1] = NULL;
    }
    void imprimir_tablero(vector<vector<nodo>> Matriz_Tablero){
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                cout << Matriz_Tablero[i][j].data << "\t";
            }
            cout << endl;
        }
    }
    /*Funcion colorear grafo que tiene como parametro el nodo a colorear y el indica del vector de colores */
    int colorear(nodo aColorear, int indColor){
        
        if(indColor > dimension-1 ){ // si el Indice es mayor a la cantidad de colores posibles .. terminamos la recursividad
            return -1; // significa que no pudo colorear el nodo
        }else
        {
            int color = colores[indColor]; // guardamos el color que vamos a examinar su posible utilizacion para este nodo
            int bandFila = 0,bandColum = 0,bandZona = 0; // iniciamos nuestas banderas
            if (aColorear.borde == true ) // verificamos si es un borde 
            {
                bandZona = dimension-1;// Colocamos el numero maximo que pueden tomar las banderas asi es igual a las otras banderas en caso que sea posible su utilizacion de esye color  
            }else
            {
                if (aColorear.vecinos_zona[0]->data != color) // examinamos el color con el nodo vecino de la zona, si es igual ya no es posible utilizar este color
                { // solo sirve para 4x4
                    bandZona = dimension-1; // mejorar poque no funciona para cuando la zona tiene mas de un elemento 
                }else
                    bandZona = -1;
            }

            if (bandZona != -1) // si el -1 bandera de la zona significa que el color no coincide con nungun valor de la zona entonces verificamos con los otros vecinos
            {
                for (int i = 0; i < dimension - 1; i++)
                {
                    if (aColorear.vecinos_fila[i]->data != color) // verificamos que el color no se encuentre entre los vacinos de la fila 
                    {
                        bandFila++; //si bandera es igual a dimension-1 entonces el color no se encuentra entre las dimension 
                    }
                    else
                    {
                        break; // en el caso de que el color se encuentre entre los vecinos rompemos la iteracion y salimos del for
                    }

                    if (aColorear.vecinos_columna[i]->data != color) // realizamos lo mismo pero con los vecinos de las cvolumnas 
                    {
                        bandColum++;// si bandera es igual a columna -1 significa que no coincide 
                    }
                    else
                    {
                        break; 
                    }
                    //agregar otro if aca para el vecinos columna si es distinto a 4x4 
                }  
            }
            // vemos si el color se puede utilizar
            if (bandFila == dimension - 1 && bandColum == dimension - 1 && bandZona == dimension - 1)
            {
                return color; //si es asi retornamos el color, que se guarda en el dato del nodo
            }
            else
            {
                indColor++;// pasamos a otro posible color del nodo
                return colorear(aColorear, indColor); //indColor++ // volvemos a examinar el nodo pero con el color nuevo
            }
        } 
    }
public:
    Sudoku(vector<vector<int>> );
    void cargarDatos(){
        
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                cargarFila(&tablero[i][j],i,j);
                cargarColum(&tablero[i][j],i,j);
                cargarZona(&tablero[i][j],i,j);
            }
        }
        //tablero[3][3].borde = true;
        //tablero[3]
        //tablero[][3]
    }
    int resolver(){
        bool band=0;
       for (int i = 0; i < dimension; i++)
       {
           for (int j = 0; j < dimension; j++)
           {
               if (tablero[i][j].data == 0)
               {
                   tablero[i][j].data = colorear(tablero[i][j],0);
                   if (tablero[i][j].data == -1)
                   {
                       band = 1;
                       break;
                   }
                   
               }
           }
           if(band == 1){
               break;
           }
       }
       if (band == 1)
       {
           cout << "No se pudo resolver el SUDOKU" << endl;
       }
       else
       {
           cout << "Tablero Resuelto" << endl;
           imprimir_tablero(tablero);
       }
   }
    ~Sudoku();
};
// constructor de la clase con parametro Matriz inicial
Sudoku::Sudoku(vector<vector<int>> Matriz_Tablero)
{
    cargarColores(); // calculamos las colores posibles para nuestro juego
    tablero.resize(dimension); // redimensionamos nuestro array
    for (int i = 0; i < dimension; ++i)
        tablero[i].resize(dimension);
    
    for (int i = 0; i < dimension; i++) // copiamos nuestra matriz de enteros a nuestra matriz de Nodos
    {
        for (int j = 0; j < dimension; j++)
        {
            tablero[i][j].data = Matriz_Tablero[i][j];
            //tablero[i][j]->vecinos_
        } 
        cout<<endl;
    }
    
    cout<<endl<<"Tablero Orgininal (cero representa celdas vacias)"<<endl;
    imprimir_tablero(tablero);
}

Sudoku::~Sudoku(){
    //liberar  memoria;
}

int main(){
    
    /*creamos nuestra matriz multidimensional */
    vector<vector<int>> Matriz_Tablero;           // creamos la matriz de enteros
    Matriz_Tablero.resize(dimension); // redimensionamos nuestro array
    for (int i = 0; i < dimension; ++i)
        Matriz_Tablero[i].resize(dimension); // cada fila redimensinamos con el valor de las dimension

    Matriz_Tablero = file("puntos.txt"); // abrimos el archivo y lo guardamos en Matriz
    Sudoku juego(Matriz_Tablero); // Iniciamos nuestro objeto juego
    juego.cargarDatos(); // cargamos los datos iniciales para comenzar el juego
    juego.resolver();// Resolvemos el juego
    
return 0;
}
//Funcion guardar Matriz desde un archivo
vector<vector<int>> file(const char* name_file){
    
    ifstream fs;
    fs.open(name_file); 
    int numero;
    vector<vector<int>> Matriz_Tablero;           // creamos la matriz de enteros
    Matriz_Tablero.resize(dimension); // redimensionamos nuestro array
    for (int i = 0; i < dimension; ++i)
        Matriz_Tablero[i].resize(dimension); // cada fila redimensinamos con el valor de las dimension

    string line,line2;
    
    if (fs.is_open()) 
    {
        cout<<"El archivo esta abierto"<<endl;
        int j=0;
        while (!fs.eof())
        {
            getline(fs, line); // separamos por lineas
            istringstream iss(line);
            for (int i = 0; i < dimension; i++)
            {
                if (iss >> numero)
                {
                    Matriz_Tablero[j][i] = numero;
                }
            }
            j++;
        }
        for (int i = 0; i < dimension; i++)
        {
            for (int j = 0; j < dimension; j++)
            {
                cout << Matriz_Tablero[i][j] << "\t";
            }
            cout << endl;
        }
        //imprimir_tablero(Matriz_Tablero);
        //cout<<"\n"<<endl;
        fs.close();
    }else{
        cout<<"Error al abrir el archivo"<<endl;
    }

    return Matriz_Tablero;
    
}

/* tratar de implementar backt. para dimensiones mas altas*/


/* Mensaje
    Profe.. Me podrias enviar mi codigo pero corregido obviamente con los errores cometidos, porque creo que me falta liberar memoria y Null algunos punteros que no se muy bien donde poner
 */