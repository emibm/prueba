#ifndef BACKPROGRAM_H
#define BACKPROGRAM_H

const int TAM=60;
	
/* ------------------------ Estructuras Utilizadas ---------------------- */

typedef char* Cadena;

struct Nodo_Texto         // Contiene las lineas de Archivo
	{int nro_linea;			
	char txt[TAM];
	Nodo_Texto* siguiente;
	};typedef Nodo_Texto* Linea;

struct Nodo_Version // Versiones
	{char nro_ver[TAM];	
	Nodo_Texto* texto;
	Nodo_Texto* cambios;
	Nodo_Version* SubVer;
	Nodo_Version* siguiente;
	};typedef Nodo_Version* Version;

struct MainFile  // Archivo
	{char nombre[TAM];
	Nodo_Version* versiones;
	};typedef MainFile* Archivo;

enum retorno //TipoRet
	{OK, ERROR, NO_IMPLEMENTADA};
	typedef enum retorno TipoRet;

/* ---------------------------------------------------------------------- */
Archivo CrearArchivoNuevo(Cadena);
Cadena Separar_Siguiente(Cadena,char,char,int,int); // Separa lo que esta entre los chars y posicion especificos
Version BuscarVersion(Version,Cadena); // Devuelve un puntero hacia la version, si existe
Cadena Anterior(Cadena); // Devuelve una cadena new con el nro de version anterior
void Ayuda(); // Muestra los comandos
void VaciarV(Archivo &,Version &,Cadena &); // Borra todo el contenido de una version y sus subversiones
void BorrarV(Version &); // Borra todas las subversiones de una version

TipoRet BorrarArchivo(Archivo &);
TipoRet CrearVersion(Archivo &,Cadena,Cadena &);
TipoRet BorrarVersion(Archivo &,Cadena,Cadena &);
TipoRet MostrarVersiones(Archivo);
TipoRet MostrarV(Version);
TipoRet CambiarNro(Version &,int &,int);
TipoRet MostrarTexto(Archivo,Cadena,Cadena &);
TipoRet MostrarCambios(Archivo,Cadena,Cadena &);
TipoRet InsertarLinea(Archivo &,Cadena,Cadena,int,Cadena &);
TipoRet LineInsOrd(Version &,Cadena,Cadena,Cadena &);
TipoRet BorrarLinea(Archivo &,Cadena,int ,Cadena &);
TipoRet Iguales(Archivo, Cadena, Cadena, bool &, Cadena &);
#endif
