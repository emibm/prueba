#include "backprogram.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
using namespace std;

/** +++++++++++++++++++++++++++ CrearArchivoNuevo +++++++++++++++++++++++ */

Archivo CrearArchivoNuevo(Cadena name)
{
Archivo nuevo = new MainFile;

if(nuevo)
	{	
	strcpy(nuevo->nombre,name);
	nuevo->versiones=NULL;	
	return nuevo;
	}
return NULL;	
}

///* +++++++++++++++++++++++++++ BORRAR ARCHIVO +++++++++++++++++++++++ */

TipoRet BorrarArchivo(Archivo &archivo)
{
if(!archivo) return ERROR;
Cadena basura=new char[TAM];

while(archivo->versiones) BorrarVersion(archivo,"1",basura);

delete(basura); delete(archivo);
archivo=NULL;
return OK;
}	

/** +++++++++++++++++++++++++++ MOSTRAR TEXTO +++++++++++++++++++++++ */

TipoRet MostrarTexto(Archivo archivo, Cadena version, Cadena &error)
{
Version aux=NULL;
Linea textoaux=NULL;
aux=BuscarVersion(archivo->versiones, version);
	
if(aux)
	{
	textoaux=aux->texto;
	cout<<" [ ARCHIVO ] "; puts(archivo->nombre); 
	cout<<"  -- Ver -- # ";puts(aux->nro_ver);
	
	if(textoaux)
		{
		cout<<" ±  TEXTO  ±"<<endl;
		while(textoaux)  
			{
			cout<<"-> "<<textoaux->nro_linea;		
			cout<<" - "; puts(textoaux->txt);		
			textoaux=textoaux->siguiente;
			}
		}
	else
		{strcpy(error,"                          No hay texto para mostrar"); return ERROR;}
	}
else
	{strcpy(error,"                   --- ERROR --- Version no encontrada "); return ERROR;}

return OK;
}

/** +++++++++++++++++++++++++++ MOSTRAR CAMBIOS +++++++++++++++++++++++ */

TipoRet MostrarCambios(Archivo archivo, Cadena version, Cadena &error)
{
Version aux=NULL;
Linea textoaux=NULL;
aux=BuscarVersion(archivo->versiones, version);
	
if(aux)
	{
	textoaux=aux->cambios;
	cout<<" [ ARCHIVO ] "; puts(archivo->nombre); 
	cout<<"  -- Ver -- # ";puts(aux->nro_ver);
	
	if(textoaux)
		{cout<<" ±  CAMBIOS  ±"<<endl;
		while(textoaux) { cout<<" = "; puts(textoaux->txt);	textoaux=textoaux->siguiente;}
		}
	else
		{strcpy(error,"                          No hay cambios para mostrar"); return ERROR;}
	}
else
	{strcpy(error,"                   --- ERROR --- Version no encontrada "); return ERROR;}

return OK;
}

/** +++++++++++++++++++++++++++ SEPARAR SIGUIENTE +++++++++++++++++++++++ **/

/* Separar_Siguiente(char 1,char 2,int 3,int 4) Esta funcion separa y devuelve como cadena lo que esta entre 
el char '1' y el char '2' en la cadena base, el 3 es la cantidad de veces que ignora el char inicio y el 4 
la cantidad de veces que ignora el fin. NOTA: Los chars ignorados al fin son parte de la cadena devuelta */

Cadena Separar_Siguiente(Cadena cadena,char ini, char fin, int posini, int posfin)
{
Cadena sig = new char[TAM]; 
int subindice=0,pos=0,cont=0,cantchar=0,yes=0;

if(!sig) return NULL;
cantchar=strlen(cadena);  

if(cantchar==0) {delete sig; return NULL;}

while(cont<=posini)
	{if(ini=='0') {yes=1; break;} // parametro especial		
	while(cadena[pos] != ini && pos<cantchar) pos++;	
	if(cadena[pos]==ini && ini!='0') {pos++; yes=1;} // yes
	cont++;
	}cont=0;
if(!yes) {delete sig; return NULL;} // yes vale false si NO encontro inicio

while((pos<cantchar) && cont<=posfin) 
	{sig[subindice]=cadena[pos]; subindice++; pos++;
	if(cadena[pos]) {if(cadena[pos]==fin) {yes=0; cont++;}}}
if(yes && strcmp(sig,"salir") && strcmp(sig,"ayuda")) {delete sig; return NULL;} // yes ahora sigue valiendo true si NO encontro final

sig[subindice]='\0';
if(ini=='0')strlwr(sig);
return sig;
}

/** +++++++++++++++++++++++++++ BuscarVersion +++++++++++++++++++++++ **/

Version BuscarVersion(Version version, Cadena ver)
{	
unsigned int cont=0;	
char comparar[TAM];
if(!version) return NULL;

while(cont<strlen(version->nro_ver)) 
	{comparar[cont]=ver[cont]; cont++;} 

if(ver[cont]!='.')	{comparar[cont]=ver[cont]; cont++;} comparar[cont]='\0'; // para que funcione hasta 2 cifras entre puntos	

if (!strcmp(version->nro_ver,ver)) return version;
else
	{if(!strcmp(version->nro_ver,comparar)) BuscarVersion(version->SubVer,ver);
	else BuscarVersion(version->siguiente,ver);}
}

/** +++++++++++++++++++++++++++ MostrarVersiones +++++++++++++++++++++++ */

TipoRet MostrarVersiones(Archivo archivo)
{
if(!archivo->versiones) return ERROR;
else MostrarV(archivo->versiones);	

return OK;
}

/** +++++++++++++++++++++++++++ MostrarV +++++++++++++++++++++++ */

TipoRet MostrarV(Version version)
{if(!version) return ERROR;
int cont=0,contpuntos=0;

while(version->nro_ver[cont]!='\0') 
	{if(version->nro_ver[cont]=='.') {contpuntos++;} cont++;} cont=0;

if(contpuntos==0) cout<<"+ "; 
else while(cont<contpuntos) {cout<<"    ";cont++;}	

puts(version->nro_ver);
MostrarV(version->SubVer);
MostrarV(version->siguiente);	
return OK;
}

/** +++++++++++++++++++++++++++ CambiarNro +++++++++++++++++++++++ */

TipoRet CambiarNro(Version &version, int &pos,int x) 
{if(!version) return ERROR;

if(version->nro_ver[pos+1]!='\0' && version->nro_ver[pos+1]!='.') pos=pos+1; // bug fix de .9 a .10 y viceversa

int cont=pos+1,subindice=0;
char save[TAM];

while(version->nro_ver[cont]!='\0') 
	{save[subindice]=version->nro_ver[cont]; subindice++; cont++;} save[subindice]='\0';

if(x==1)
	{
	if(pos<1 || version->nro_ver[pos-1]=='.')
		{
		if(version->nro_ver[pos]=='9')
			{version->nro_ver[pos]='1'; version->nro_ver[pos+1]='0'; 
			version->nro_ver[pos+2]='\0'; strcat(version->nro_ver,save);}
		else
			version->nro_ver[pos]=version->nro_ver[pos]+1;
		}
	else
		{
		if(version->nro_ver[pos]=='9')
			{version->nro_ver[pos]='0'; version->nro_ver[pos-1]=version->nro_ver[pos-1]+1;}			
		else
			version->nro_ver[pos]=version->nro_ver[pos]+1;
		}	
	}
if(x==-1)
	{
	if(pos<1 || version->nro_ver[pos-1]=='.')
		{
		if(version->nro_ver[pos-1]=='1' && version->nro_ver[pos]=='0' )
			{version->nro_ver[pos-1]='9'; version->nro_ver[pos]='\0'; strcat(version->nro_ver,save);}
		else
			version->nro_ver[pos]=version->nro_ver[pos]-1;
		}
	else
		{
		if(version->nro_ver[pos]=='0')
			{version->nro_ver[pos]='9'; version->nro_ver[pos-1]=version->nro_ver[pos-1]-1;}			
		else
			version->nro_ver[pos]=version->nro_ver[pos]-1;
		}			
	}
CambiarNro(version->SubVer,pos,x);
CambiarNro(version->siguiente,pos,x);
return OK;
}

/** +++++++++++++++++++++++++++ CrearVersion +++++++++++++++++++++++ */

TipoRet CrearVersion(Archivo &archivo,Cadena ver, Cadena &error)
{	
Version insertar=NULL;
Cadena anterior=Anterior(ver); 
int cantletras=strlen(ver)-1;
int ctp=0,ctpa=0,nrolinea=1;
Linea aux=NULL;
unsigned int cont=0;

if((!archivo->versiones && strcmp(ver,"1"))) 
	{strcpy(error,"                     --- ERROR --- ARCHIVO NUEVO: DEBE CREAR V.|1|"); delete anterior; return ERROR;}

Version nuevo = new Nodo_Version; if(!nuevo) {delete anterior; return ERROR;}

strcpy(nuevo->nro_ver,ver);	
nuevo->texto=NULL;
nuevo->cambios=NULL;
nuevo->SubVer=NULL;
nuevo->siguiente=NULL;

if(!archivo->versiones && !strcmp(ver,"1"))   // Primer Nodo
	{archivo->versiones=nuevo; 
	strcpy(error,"                 +++ EXITO +++ LA  PRIMER VERSION HA SIDO CREADA"); delete anterior; return OK;}

if(archivo->versiones && !strcmp(ver,"1")) // BUG arreglado de no poder ingresar dos veces version 1 porque no hay anterior
	{nuevo->siguiente=archivo->versiones;
	archivo->versiones=nuevo;
	CambiarNro(nuevo->siguiente, cantletras, 1);
	strcpy(error,"                    +++ EXITO +++ LA VERSION HA SIDO CREADA");
	delete anterior; return OK;}

insertar=BuscarVersion(archivo->versiones, anterior);  // insertar apunta al anterior

if(insertar)
	{
	while(cont<strlen(nuevo->nro_ver)) 
		{
		if(cont<strlen(insertar->nro_ver)) {if(insertar->nro_ver[cont]=='.') ctp++;}
		if (nuevo->nro_ver[cont]=='.') ctpa++;
		cont++;
		}		
	if(ctp==ctpa)                                 
		{nuevo->siguiente=insertar->siguiente;  // Si tiene la misma cantidad de puntos, insertar al inicio del siguiente
		insertar->siguiente=nuevo;}
	else
		{nuevo->siguiente=insertar->SubVer;  // Sino, insertar al inicio de SubVersiones
		insertar->SubVer=nuevo;
		
		aux=insertar->texto;
		
		while(aux) // Aqui se heredan las lineas de texto si estoy creando una SubVersion
			{
			InsertarLinea(archivo,nuevo->nro_ver,aux->txt,nrolinea,error);
			delete(nuevo->cambios); nuevo->cambios=NULL;  // Cada vez que inserto se crean cambios, BORRAR
			nrolinea++;
			aux=aux->siguiente;
			}		
		}
	
	if(nuevo->siguiente) {CambiarNro(nuevo->siguiente, cantletras,1);}
	
	strcpy(error,"                    +++ EXITO +++ LA VERSION HA SIDO CREADA");
	delete anterior; return OK;
	}
else
	{strcpy(error,"           --- ERROR --- NO SE ENCUENTRA LA VERSION ANTERIOR"); 
	 delete(nuevo); delete anterior; return ERROR;}
}

/** +++++++++++++++++++++++++++ AnteriorVersion +++++++++++++++++++++++ */

Cadena Anterior(Cadena ver)
{
int cantletras=strlen(ver);
Cadena anterior=new char[TAM]; strcpy(anterior,ver);

if(anterior[cantletras-1]=='0')	
	{
	if(anterior[cantletras-2]=='1')
		{
		anterior[cantletras-1]='\0';	
		anterior[cantletras-2]='9';	
		}
	else 
		{anterior[cantletras-2]=anterior[cantletras-2]-1;	// funciona para 2 cifras
		anterior[cantletras-1]='9';}
	}
else anterior[cantletras-1]=anterior[cantletras-1]-1;

cantletras=strlen(anterior);

if(anterior[cantletras-2]=='.' && anterior[cantletras-1]=='0') 
	anterior[cantletras-2]='\0';	

return anterior;
}

/** +++++++++++++++++++++++++++ BorrarVersion +++++++++++++++++++++++ */

TipoRet BorrarVersion(Archivo &archivo,Cadena ver,Cadena &errorf)
{
Version borrar=BuscarVersion(archivo->versiones,ver);
Cadena ant=NULL;
Version anterior=NULL, aux=NULL;
int cant=0;
	
if(borrar)
	{	
	if(borrar->SubVer) 
		{VaciarV(archivo,borrar->SubVer,errorf);
		BorrarV(borrar->SubVer);		
		}	
	while(borrar->texto) BorrarLinea(archivo,borrar->nro_ver,1,errorf);
	while(borrar->cambios) BorrarLinea(archivo,borrar->nro_ver,-32000,errorf);
	}
else
	{strcpy(errorf,"           --- ERROR --- NO SE ENCUENTRA LA VERSION A BORRAR"); return ERROR;}

if(!strcmp(ver,"1"))	
	{if(archivo->versiones->siguiente) {archivo->versiones=archivo->versiones->siguiente; aux=archivo->versiones;}
	else archivo->versiones=NULL;}		
else
	{ant=Anterior(ver);
	anterior=BuscarVersion(archivo->versiones,ant); delete(ant);}

if(anterior)
	{
	if(strlen(borrar->nro_ver) > strlen(anterior->nro_ver))
		{if(borrar->siguiente) {anterior->SubVer=borrar->siguiente; aux=anterior->SubVer;}	
		else anterior->SubVer=NULL;}
	else
		if(borrar->siguiente) {anterior->siguiente=borrar->siguiente; aux=anterior->siguiente;}
		else anterior->siguiente=NULL;
	}
delete (borrar);
if(aux) {cant=strlen(aux->nro_ver)-1; CambiarNro(aux,cant,-1);}
strcpy(errorf,"             --- EXITO --- VERSION Y SUBVERSIONES BORRADAS");
return OK;		
}

/** +++++++++++++++++++++++++++ VaciarV +++++++++++++++++++++++ */

void VaciarV(Archivo &archivo,Version &borrar, Cadena &errorf)
{
if(!borrar) return;	

while(borrar->texto) BorrarLinea(archivo,borrar->nro_ver,1,errorf);
while(borrar->cambios) BorrarLinea(archivo,borrar->nro_ver,-32000,errorf);

VaciarV(archivo,borrar->SubVer,errorf);
VaciarV(archivo,borrar->siguiente,errorf);	
return;	
}

/** +++++++++++++++++++++++++++ BorrarV +++++++++++++++++++++++ */

void BorrarV(Version &version)
{
Version borrar=version;

if(!version) return;

if(version->SubVer)
	{	
	version->SubVer=version->SubVer->siguiente;
	delete (borrar);
	BorrarV(version->SubVer);
	}
if(version->siguiente)
	{
	borrar=version;
	version=version->siguiente;
	delete (borrar);
	BorrarV(version->siguiente);	
	}

delete borrar;
version=NULL;
return;
}

/** +++++++++++++++++++++++++++ INSERTAR LINEA +++++++++++++++++++++++ */

TipoRet InsertarLinea(Archivo &archivo,Cadena nroversion,Cadena text_line,int nrolinea,Cadena &error) 
{
Cadena NroLinea=new char[20];
Version insertar=NULL;

insertar=BuscarVersion(archivo->versiones,nroversion);

itoa(nrolinea,NroLinea,10);

if(insertar) LineInsOrd(insertar,NroLinea,text_line,error);
else
	{strcpy(error,"             --- ERROR --- Numero de version no encontrado");
	return ERROR;}	

delete(NroLinea);
return OK;
}

/** +++++++++++++++++++++++++++ LineInsOrd +++++++++++++++++++++++ */

TipoRet LineInsOrd(Version &version,Cadena nrolinea,Cadena texto,Cadena &error)
{	
Linea nuevo = new Nodo_Texto; // Texto
Linea change = new Nodo_Texto; // Cambios
Linea antaux=NULL, aux=version->texto, cbs=version->cambios;
int yes=0;

nuevo->nro_linea=strtol(nrolinea,NULL,10);
strcpy(nuevo->txt,texto);
nuevo->siguiente=NULL;

strcpy(change->txt,"IL");                         // CAMBIOS
strcat(change->txt,"     ");
strcat(change->txt,nrolinea);
strcat(change->txt,"     ");
strcat(change->txt,texto);
change->siguiente=NULL;

if(!cbs) version->cambios=change;
else
	{while(cbs->siguiente) {cbs=cbs->siguiente;} cbs->siguiente=change;}  // FIN CAMBIOS

if(nuevo->nro_linea==1 && !aux)
	{
	version->texto=nuevo;
	strcpy(error,"               +++ EXITO +++ La linea se inserto correctamente !!");	
	return OK;	
	}
if(nuevo->nro_linea!=1 && !aux)
	{
	strcpy(error,"           --- ERROR --- No hay texto previo, debe crear en linea 1");	
	if(!cbs)version->cambios=NULL;
	else cbs->siguiente=NULL;	
	delete (change);
	delete (nuevo);	
	return ERROR;
	}

while(aux)
	{
	if(aux->nro_linea == nuevo->nro_linea)
		{
		yes=1;
		if(antaux) antaux->siguiente=nuevo;
		else version->texto=nuevo;		
		nuevo->siguiente=aux;
		}
	if(yes)
		{while(aux) {aux->nro_linea++; aux=aux->siguiente; }
		strcpy(error,"               +++ EXITO +++ La linea se inserto correctamente !!");
		return OK; }
	antaux=aux;
	aux=aux->siguiente;		
	}

if(!aux && nuevo->nro_linea != antaux->nro_linea +1)
	{strcpy(error,"               --- ERROR --- Numero de linea fuera de limites");
	if(!cbs)version->cambios=NULL;
	else cbs->siguiente=NULL;	
	delete (change);
	delete (nuevo);	
	return ERROR;}

antaux->siguiente = nuevo;
strcpy(error,"               +++ EXITO +++ La linea se inserto correctamente !!");
return OK;
}

/** +++++++++++++++++++++++++++ BorrarLinea +++++++++++++++++++++++ */

TipoRet BorrarLinea(Archivo &archivo,Cadena Nroversion,int NROlinea,Cadena &error)
{
char Nro[20];
Linea change = new Nodo_Texto; // Cambios
Version insertar=NULL;
Linea aux=NULL, antaux=NULL, borrar=NULL;

if(!archivo->versiones)
	{strcpy(error,"                  --- ERROR --- No hay versiones creadas");
	delete(change); return ERROR;}

insertar=BuscarVersion(archivo->versiones,Nroversion);

if(!insertar)
    {strcpy(error,"               --- ERROR --- Numero de version no encontrado"); delete(change); return ERROR;}	

aux=insertar->texto;

if(NROlinea==-32000) // Parametro especial BorrarVersion
	{
	while(insertar->cambios)
		{
		borrar=insertar->cambios;
		insertar->cambios=insertar->cambios->siguiente;
		delete(borrar);
		}
	error[0]='\0';
	return OK;
	}

if(!aux)
	{strcpy(error,"                  --- ERROR --- No hay linea creadas"); delete(change); return ERROR;}

while(aux)
	{if(aux->nro_linea == NROlinea)	{borrar=aux; break;}	
	antaux=aux;
	aux=aux->siguiente;}

if(!aux)	
	{strcpy(error,"                 --- ERROR --- Numero de linea no encontrado");
	delete(change); return ERROR;}

if(!antaux)	insertar->texto = aux->siguiente;
else antaux->siguiente = aux->siguiente;

aux=aux->siguiente;

while(aux) {aux->nro_linea--; aux=aux->siguiente;}  // CAMBIOS

delete borrar;

Linea cbs=insertar->cambios;

itoa(NROlinea,Nro,10);
strcpy(change->txt,"BL");
strcat(change->txt,"     ");
strcat(change->txt,Nro);
change->siguiente=NULL;

if(!cbs) insertar->cambios=change;
else
	{while(cbs && cbs->siguiente) {cbs=cbs->siguiente;} cbs->siguiente=change;}    // FIN CAMBIOS

strcpy(error,"              +++ EXITO +++ La linea ha sido borrada");

return OK;	
}

/** +++++++++++++++++++++++++++ Iguales +++++++++++++++++++++++ **/

TipoRet Iguales(Archivo archivo, Cadena v1, Cadena v2, bool &ig, Cadena &error)
{
Version ver1=BuscarVersion(archivo->versiones,v1);	
Version ver2=BuscarVersion(archivo->versiones,v2);

if(!ver1 || !ver2) 
	{strcpy(error,"             --- ERROR --- No se encuentra alguna de las versiones"); return ERROR;}

Linea l1=ver1->texto, l2=ver2->texto;
if(!l1 && !l2) {ig=1; strcpy(error,"                       Ambas versiones estan vacias"); return OK;}

while(l1 && l2)
	{if(!strcmp(l1->txt,l2->txt)) {l1=l1->siguiente; l2=l2->siguiente;}
	else l1=l1->siguiente;}

if(!l1 && !l2) {ig=1; return OK;}

strcpy(error,"                              --- NO SON IGUALES ---");
return ERROR;
}

/** +++++++++++++++++++++++++++ Ayuda +++++++++++++++++++++++ */

void Ayuda()
{
cout<<"             --------------- MOSTRANDO COMANDOS ---------------"<<endl<<endl;
cout<<"(?) CrearArchivo('nombre_archivo');"<<endl;
cout<<"(?) BorrarArchivo('nombre_archivo');"<<endl;
cout<<"(?) CrearVersion('nombre_archivo','nro_version');"<<endl;
cout<<"(?) BorrarVersion('nombre_archivo','nro_version');"<<endl;
cout<<"(?) MostrarVersiones('nombre_archivo');"<<endl;
cout<<"(?) Iguales('nro_version1','nro_version2');"<<endl;
cout<<"(?) InsertarLinea('nombre_archivo','nro_version','texto',nro_linea);"<<endl;
cout<<"(?) BorrarLinea('nombre_archivo','nro_version',nro_linea);"<<endl;
cout<<"(?) MostrarTexto('nombre_archivo','nro_version');"<<endl;
cout<<"(?) MostrarCambios('nombre_archivo','nro_version');"<<endl;
cout<<endl<<"       NOTA: Apostrofes |'| hacen referencia a comillas tradicionales "<<endl;
}








