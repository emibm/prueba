#include "backprogram.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
using namespace std;

int main (int argc, char *argv[]) 
{	
Archivo archivo=NULL;
int exit=0;
bool iguales;
char cadena[100],error[100]; 
Cadena comando=NULL; // Comando ingresado por el usuario
Cadena errorf=error, nombre_archivo, version_archivo, texto_archivo, nrolinea_archivo,version2_archivo;

// ++++++++++++++++++++++++++++++++++++++ INICIO DEL PROGRAMA +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

while(exit!=1)
{
comando=NULL; nombre_archivo=NULL; version_archivo=NULL; texto_archivo=NULL; nrolinea_archivo=NULL, version2_archivo=NULL;
error[0]='\0';
iguales=0;

gets(cadena);
fflush stdin;
cout<<endl;

comando=Separar_Siguiente(cadena,'0','(',0,0); // Separar el comando de la cadena

// +++++++++++++++++++++++++++++++++++++ SALIR +++++++++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"salir")) break;

// +++++++++++++++++++++++++++++++++++++ CREAR ARCHIVO +++++++++++++++++++++++++++++++++++++++++++++++ 

if(comando && !strcmp(comando,"creararchivo"))               
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);		
	
	if(nombre_archivo && !archivo)		
		{archivo=CrearArchivoNuevo(nombre_archivo);		
		strcpy(errorf,"                      +++ ARCHIVO CREADO EXITOSAMENTE +++");}
	else 
		strcpy(errorf,"                  --- ERROR --- EL ARCHIVO NO HA SIDO CREADO");		
	}

// +++++++++++++++++++++++++++++++++++++ BORRAR ARCHIVO +++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"borrararchivo")) 
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);	
	
	if(!strcmp(archivo->nombre,nombre_archivo))	
		{
		if(BorrarArchivo(archivo)==OK)
			strcpy(error,"                 --- EXITO ---  EL ARCHIVO HA SIDO BORRADO");
		else
			strcpy(error,"                --- ERROR --- NO HAY ARCHIVOS CREADOS");
		}
	else
		strcpy(error,"               --- ERROR --- No hay archivos con ese nombre");	
	}

// +++++++++++++++++++++++++++++++++++++ CREAR VERSION +++++++++++++++++++++++++++++++++++++++++++++++ 

if(comando && !strcmp(comando,"crearversion"))               
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);	
	version_archivo=Separar_Siguiente(cadena,'"','"',2,0);	
	
	if(!nombre_archivo || !archivo || !version_archivo)
		strcpy(errorf,"                  --- ERROR --- LA VERSION NO HA SIDO CREADA");
	else
		{
		if(!strcmp(nombre_archivo,archivo->nombre)) CrearVersion(archivo,version_archivo,errorf);
		else strcpy(errorf,"                  --- ERROR --- LA VERSION NO HA SIDO CREADA");	
		}
	}

// +++++++++++++++++++++++++++++++++++++ INSERTAR LINEA +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"insertarlinea"))             
	{	
	if(archivo)	
		{
		nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);
		version_archivo=Separar_Siguiente(cadena,'"','"',2,0);	
		texto_archivo=Separar_Siguiente(cadena,'"','"',4,0);
		nrolinea_archivo=Separar_Siguiente(cadena,',',')',2,0);
		
		if(!nombre_archivo || !version_archivo || !texto_archivo || !nrolinea_archivo)
			strcpy(errorf,"               --- ERROR --- Parametros incorrectos. Reintentar.");
		else
			{
			if(!strcmp(archivo->nombre,nombre_archivo))
				InsertarLinea(archivo,version_archivo,texto_archivo,strtol(nrolinea_archivo,NULL,10),errorf);
			else
				strcpy(errorf,"                 --- ERROR --- No hay archivos con ese nombre");
			}
		}
	else
		strcpy(errorf,"                   --- ERROR --- NO HAY ARCHIVOS CREADOS");
	}

// +++++++++++++++++++++++++++++++++++++ BORRAR LINEA +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"borrarlinea"))             
	{
	if(archivo)	
		{
		nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);
		version_archivo=Separar_Siguiente(cadena,'"','"',2,0);
		nrolinea_archivo=Separar_Siguiente(cadena,',',')',1,0);
			
		if(!strcmp(archivo->nombre,nombre_archivo))
			BorrarLinea(archivo,version_archivo,strtol(nrolinea_archivo,NULL,10),errorf);
		else
			strcpy(error,"                 --- ERROR --- No hay archivos con ese nombre");
			}
	else
		strcpy(error,"                --- ERROR --- NO HAY ARCHIVOS CREADOS");
		}

// +++++++++++++++++++++++++++++++++++++ MOSTRAR TEXTO +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && (!strcmp(comando,"mostrartexto") || !strcmp(comando,"mostrarcambios")) && !archivo)	
	strcpy(error,"               --- ERROR ---  NO HAY ARCHIVOS CREADOS");	

if(comando && (!strcmp(comando,"mostrartexto") || !strcmp(comando,"mostrarcambios")) && archivo)             
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);	
	version_archivo=Separar_Siguiente(cadena,'"','"',2,0);
	
	if(!nombre_archivo || !version_archivo)
		strcpy(errorf,"               --- ERROR --- Parametros incorrectos. Reintentar.");
	else
		{
		if(!strcmp(archivo->nombre,nombre_archivo) && !strcmp(comando,"mostrartexto"))	MostrarTexto(archivo,version_archivo,errorf);		
		if(!strcmp(archivo->nombre,nombre_archivo) && !strcmp(comando,"mostrarcambios")) MostrarCambios(archivo,version_archivo,errorf);
		
		if(strcmp(archivo->nombre,nombre_archivo)) strcpy(errorf,"                 --- ERROR --- No hay archivos con ese nombre");
		}
	}

// +++++++++++++++++++++++++++++++++++++ MOSTRAR VERSIONES +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"mostrarversiones"))             
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);	
	
	if(!nombre_archivo)
		strcpy(errorf,"               --- ERROR --- Parametros incorrectos. Reintentar.");
	else
		{
		if(archivo) 
			if(!strcmp(archivo->nombre,nombre_archivo))
				{cout<<"             --------------- MOSTRANDO VERSIONES ---------------"<<endl<<endl; 
				 if(MostrarVersiones(archivo) == OK) 1;
				 else cout<<"                          NO HAY VERSIONES CREADAS :( "<<endl;
				 }
			else strcpy(errorf,"                 --- ERROR --- No hay archivos con ese nombre");
		
		else cout<<"                            NO HAY ARCHIVOS :( "<<endl;
		}
	}

// +++++++++++++++++++++++++++++++++++++ BORRAR VERSION +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"borrarversion"))             
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);
	version_archivo=Separar_Siguiente(cadena,'"','"',2,0);
	
	if(!nombre_archivo || !version_archivo)
		strcpy(errorf,"               --- ERROR --- Parametros incorrectos. Reintentar.");
	else
		{
		if(archivo && archivo->versiones) 
			if(!strcmp(archivo->nombre,nombre_archivo))
				{BorrarVersion(archivo,version_archivo,errorf);}
			else strcpy(errorf,"                 --- ERROR --- No hay archivos con ese nombre");
		
		else cout<<"                    NO HAY ARCHIVO O VERSIONES CREADAS :( "<<endl;
		}	
	}	

// +++++++++++++++++++++++++++++++++++++ IGUALES +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"iguales"))
	{
	nombre_archivo=Separar_Siguiente(cadena,'"','"',0,0);
	version_archivo=Separar_Siguiente(cadena,'"','"',2,0);	
	version2_archivo=Separar_Siguiente(cadena,'"','"',4,0);
	
	if(!nombre_archivo || !version_archivo || !version2_archivo)
		strcpy(errorf,"               --- ERROR --- Parametros incorrectos. Reintentar.");
	else
		{if(archivo && archivo->versiones)
			{Iguales(archivo,version_archivo,version2_archivo,iguales,errorf);}
		else cout<<"                    NO HAY ARCHIVO O VERSIONES CREADAS :( "<<endl;
		}
	if(iguales==true)  cout<<"                           +++ SON IGUALES +++"<<endl;	
	}

// +++++++++++++++++++++++++++++++++++++ AYUDA +++++++++++++++++++++++++++++++++++++++++++++++++

if(comando && !strcmp(comando,"ayuda")) Ayuda();	
	
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

puts(errorf); // Muestra cambios exitosos o fail
cout<<endl;

//       Liberar memoria usada por Separar_Siguiente       //

if(comando) delete comando;
if(version_archivo) delete version_archivo;
if(version2_archivo) delete version2_archivo;
if(nombre_archivo) delete nombre_archivo;
if(texto_archivo) delete texto_archivo;
if(nrolinea_archivo) delete nrolinea_archivo;

}	// fin del while

return 0;

} // fin del main




















