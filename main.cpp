/*Lia Fernanda Pardo Mireles 2096765 
 * Alejandro Quintanilla Leal 2010568
 * Angel Joseph Meraz Hernandez 2067151
 * Luis Fernando Segobia Torres 2177528
 * */
#include<iostream>
#include<string>
#include<limits>
#include<sstream>//se utiliza en la funcion leerValor
#include<algorithm>//se utiliza en la funcion leerCadena

using namespace std;

// Esta funcion lee un valor de tipo T y valida que este dentro de un rango
// Si el valor no es valido, vuelve a pedir la entrada
template <typename T>
T leerValor(T menor, T mayor) {
    T valor;
    string linea;

    while (true) {
        getline(cin, linea);  // Leer toda la línea
        stringstream ss(linea);

        if (!(ss >> valor)) {
            cout << "Numero no valido, intente nuevamente.." << endl;
            continue;
        }

        // Verificar que no haya más datos después del número
        char c;
        if (ss >> c) {
            cout << "Entrada invalida (demasiados datos). Solo ingrese un numero." << endl;
            continue;
        }

        if (valor < menor || valor > mayor) {
            cout << "\n**Numero fuera de rango (" << menor << " - " << mayor << "), intente nuevamente**" << endl;
            continue;
        }

        return valor;
    }
}

// Esta funcion lee una cadena de texto y valida que no este vacia
// Si la cadena esta vacia, vuelve a pedir la entrada
string leerCadena() {
    string entrada;

    do {
        getline(cin, entrada);  

        // Verifica si la cadena está vacía o solo tiene espacios
        if (entrada.empty() || all_of(entrada.begin(), entrada.end(), ::isspace)) {
            cout << "\n**La entrada no puede estar vacia. Intenta de nuevo.**" << endl;
        } else {
            break;
        }
    } while (true);

    return entrada;
}
// Estructura del nodo (Alumno)
struct Alumno {
    int matricula;
    string nombre;
    int edad;
    float promedio;
    string direccion;
    long long telefono;
    bool situacion; // true = activo, false = baja
    Alumno* next;
    Alumno* anterior;
};

typedef Alumno* Lista;
typedef Alumno* Pila;
struct Cola{
	Alumno* first = NULL;
	Alumno* last = NULL;
};

// DECLARACION DE LAS FUNCIONES

// Funciones de ordenamiento y busqueda / manejo de estructuras
Lista buscarPorMatricula(Alumno* cabeza, int matricula, Alumno* fin);
Alumno* buscarPorNombre(Lista cabeza, const string& nombre);
void insertarOrdenado(Lista &cabeza, Alumno* nuevo);
void eliminar_lista(Lista &lista);
void mostrar(const Lista lista);
void mostrarAlumno(Alumno* alumno);
int totalAlumnos(Lista lista_alumnos_activos);

// Menu
int mostrarMenu();

// FUNCIONES ESPECIFICAS p/ inciso

// Inciso 1 - Alta de estudiantes
void altaAlumnos(Lista &lista_activos);

Lista eliminarDeLista(Lista &cabeza, int matricula);
// Inciso 2 - Baja de estudiantes
int submenu_bajas(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos);
void bajaParcial(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos);
void deshacerBaja(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos);
void bajaTotal(Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos);

// Inciso 3 - Recuperar alumno
void recuperarAlumno(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos);

// Inciso 4 - Reportes
void submenuReportes(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos);
void mostrarPorcentajes(Lista &lista_alumnos_activos);
void mostrarDatos(Lista &lista_alumnos_activos);
void mostrarAprobados(Lista &lista_alumnos_activos);
Alumno* nodo_medio(Lista cabeza, Lista fin);
int total_activos = 0;

// Inciso 5 - Modificar datos
void modificarDatos(Lista &lista_alumnos_activos);

//inciso 8 - Crear grupos
void crearGrupos(Lista alumnos_activos);

int main(){
	int op;
	Lista lista_alumnos_activos = NULL;
	Lista lista_alumnos_inactivos = NULL;
	Pila pila_alumnos_inactivos = NULL;
	Cola cola_alumnos_activos;
	
	do {
		op = mostrarMenu();

        switch (op) {
            case 1: altaAlumnos(lista_alumnos_activos); break;
            case 2: submenu_bajas(lista_alumnos_activos, lista_alumnos_inactivos, pila_alumnos_inactivos); break;
            case 3: recuperarAlumno(lista_alumnos_activos, lista_alumnos_inactivos, pila_alumnos_inactivos); break;
            case 4: submenuReportes(lista_alumnos_activos, lista_alumnos_inactivos); break;
            case 5: modificarDatos(lista_alumnos_activos); break;
            case 6: break;
            case 7: crearGrupos(lista_alumnos_activos);break;
            case 8: 
            	cout << "\nSaliendo....";
				eliminar_lista(lista_alumnos_activos);
				eliminar_lista(lista_alumnos_inactivos);
				eliminar_lista(pila_alumnos_inactivos);
				cout << "\n**Memoria liberada**\n";
				break;
        }

    } while (op != 8);
    return 0;
}

//Se utiliza para insertar un nuevo nodo en la lista de alumnos activos de manera ordenada
void insertarOrdenado(Lista &cabeza, Alumno* nuevo) {
    if (cabeza == NULL || nuevo->matricula < cabeza->matricula) {//si la lista esta vacia o si la matricula es menor que la cabeza agregamos al inicio
        nuevo->next = cabeza;
        cabeza = nuevo;
    } else { // en caso contrario recorremos la lista hasta encontrar null o que la matricula sea mayor que la siguiente
        Alumno* actual = cabeza;
        while (actual->next != NULL && actual->next->matricula < nuevo->matricula) {
            actual = actual->next;
        }
        nuevo->next = actual->next;
        actual->next = nuevo;
    }
}
// Se utiliza para buscar un nodo en la lista de alumnos activos por matricula se utiliza busqueda binaria (con recursividad), cuando lo encuentra retorna al Alumno
Alumno* buscarPorMatricula(Alumno* cabeza, int matricula, Alumno* fin) {
    system("cls");
    cout << "\n--- Buscando por matricula ---\n";
    if(cabeza==fin || cabeza==NULL) return NULL;

    Alumno* medio = nodo_medio(cabeza, fin);

    if (medio->matricula == matricula) {
        return medio;
    }

    if (matricula < medio->matricula) {//si la matricula es menor que la del medio, buscamos en la parte izquierda, por eso se pasa el medio como fin
        return buscarPorMatricula(cabeza, matricula, medio);
    }
    // si la matricula es mayor que la del medio, buscamos en la parte derecha, por eso se pasa el medio como cabeza
    return buscarPorMatricula(medio->next, matricula, fin);
}

// Se utiliza para buscar un nodo en la lista de alumnos por nombre, se utiliza busqueda secuencial y retorna el Alumno
Alumno* buscarPorNombre(Lista cabeza, const string& nombre) {
    system("cls");
    cout << "\n--- Buscando por nombre ---\n";
    Alumno* actual = cabeza;
    Alumno* encontrado = NULL;// este solo va a funcionar cuando hay un solo alumno con ese nombre
    int cont=0;
    int mat=0;
    while (actual != NULL) {
        if(actual->nombre == nombre) {
            encontrado = actual;
            mostrarAlumno(actual);
            cont++;
        }
        actual = actual->next;
    }

    if (cont > 1)//quiere decir que hubo mas de un alumno con el mismo nombre
    {
        cout<< "Se encontraron " << cont << " alumnos con el mismo nombre." << endl;
        cout << "Por favor, busque por matricula." << endl;
        cout << "Ingrese la matricula del alumno: ";
        mat = leerValor<int>(0,99999999);
        actual = buscarPorMatricula(cabeza, mat, NULL);
        system("cls");
        if (actual == NULL)
        {
            return NULL;
        }
        else
        {
            cout << "Se encontro el alumno con la matricula " << mat << endl;
            return actual;
        }   
    }else if (cont ==1){//solo hay un alumno con ese nombre
        system("cls");
        cout << "Se encontro el alumno con el nombre " << nombre << endl;
        return encontrado;
    }else{
        system("cls");
        cout << "No se encontro el alumno con el nombre " << nombre << endl;
        return NULL;
    }
    
    return NULL;
}

// Se utiliza para eliminar un nodo de la lista de alumnos, se busca por matricula y se elimina el nodo, retorna el nodo eliminado
Lista eliminarDeLista(Lista &cabeza, int matricula) {
    Alumno* actual = cabeza;
    Alumno* anterior = NULL;

    while (actual != NULL && actual->matricula != matricula) {
        anterior = actual;
        actual = actual->next;
    }

    if (actual == NULL) return NULL; // No encontrado

    if (anterior == NULL) {
        cabeza = actual->next; // era el primero
    } else {
        anterior->next = actual->next;
    }

    actual->next = NULL;
    return actual;
}

// Se utiliza para mostrar un alumno en especifico, se le pasa el nodo y se imprime
void mostrarAlumno(Alumno* alumno) {
    cout << "\n-----------------------------------\n";
    if(alumno == NULL){
        cout << "Error al mostrar alumno" << endl;
    }

    cout << "Matricula: " << alumno->matricula << endl
        << "Nombres: " << alumno->nombre << endl
        << "Edad: " << alumno->edad << endl
        << "Promedio: " << alumno->promedio << endl
        << "Direccion: " << alumno->direccion << endl
        << "Telefono: " << alumno->telefono << endl
        << "Situacion: " << (alumno->situacion == 1 ? "**Activo**" : "**Inactivo**") << endl;
    cout << endl;
}

// Se utiliza para dar de baja a un alumno, se le pasa la lista de alumnos activos y la lista de alumnos inactivos
//se busca el alumno por matricula o nombre y se elimina de la lista de activos y se agrega a la lista de inactivos igual que a la pila de inactivos
void bajaParcial(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos) { 
	cout << "\n--- Baja Parcial ---\n";
	
	cout << "\n***Lista de alumnos activos***"<< endl;
	mostrar(lista_alumnos_activos);
	cout << "\n-------------------------------";
	cout << "\n-------------------------------\n";
	
	int opcion;
    cout << "Buscar por:\n1. Matricula\n2. Nombre\nOpcion: ";
    opcion = leerValor<int>(1,2);

    

    Alumno* encontrado = NULL;
    if (opcion == 1) {
        int mat;
        cout << "Ingrese matricula: ";
        mat = leerValor<int>(0,9999999);
        encontrado = buscarPorMatricula(lista_alumnos_activos, mat, NULL);
    } 
	else {
        string nombre;
        cout << "Ingrese nombre: ";
        nombre = leerCadena();
        encontrado = buscarPorNombre(lista_alumnos_activos, nombre);
	}

    if (encontrado == NULL) {
        cout << "Alumno no encontrado.\n";
        system("pause");
    }
	else{
		// Eliminarlo de la lista de activos
		Alumno* baja = eliminarDeLista(lista_alumnos_activos, encontrado->matricula);
    	if (baja != NULL){
        	baja->situacion = false;
	
    	    // Insertar ordenadamente en la lista de bajas parciales
        	insertarOrdenado(lista_alumnos_inactivos, baja);
	
    	    // Insertar en la pila de bajas (al principio)
        	Alumno* copia = new Alumno(*baja); // copia todos los datos, pero crea un nodo nuevo
			copia->next = pila_alumnos_inactivos;
			pila_alumnos_inactivos = copia;
	
    	    cout << "Alumno dado de baja parcialmente.\n";
    	    
        	cout << "\n***Lista de alumnos inactivos***"<< endl;
            mostrar(lista_alumnos_inactivos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";
        	cout << "\n***Pila de alumnos inactivos***"<< endl;
        	mostrar(pila_alumnos_inactivos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";
    	} else {
	        cout << "Error al procesar baja.\n";
	    }
	    system("pause");
	}
}

// Se utiliza para deshacer la baja de un alumno, se le pasa la lista de alumnos activos, la lista de alumnos inactivos y la pila de alumnos inactivos
//se busca el alumno en la pila de inactivos y se elimina de la pila y lista de inactivos y se agrega a la lista de activos
void deshacerBaja(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos) {
	cout << "\n--- Deshacer Baja Parcial ---\n";
	
    if (pila_alumnos_inactivos == NULL) {
        cout << "No hay bajas recientes que deshacer.\n";
        system("pause");
    }
	else{
		// Sacar al alumno del tope de la pila
	    Alumno* alumno = pila_alumnos_inactivos;
	    pila_alumnos_inactivos = pila_alumnos_inactivos->next;
	    alumno->next = NULL;
	
	    // Eliminar de la lista de bajas parciales
	    Alumno* eliminado = eliminarDeLista(lista_alumnos_inactivos, alumno->matricula);
	    if (eliminado == NULL) {
	        cout << "Error: El alumno no se encuentra en la lista de bajas parciales.\n";
	        system("pause");
	    }
	    else{
	    	// Cambiar situacion a activo y agregarlo a la lista de activos
		    eliminado->situacion = true;
		    insertarOrdenado(lista_alumnos_activos, eliminado);
		
		    cout << "Baja deshecha exitosamente. Alumno con matricula " << eliminado->matricula << " reactivado.\n";
		    
		    cout << "\n***Pila de alumnos inactivos***"<< endl;
		    mostrar(pila_alumnos_inactivos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";
			system("pause");
		}
	
	    
	}
}

// Se utiliza para dar de baja total a un alumno, se le pasa la lista de alumnos inactivos y la pila de alumnos inactivos
//No hay manera de despues darlo de baja aqui poder recuperarlo
void bajaTotal(Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos) {
    cout << "\n--- Baja Total ---\n";
    if (lista_alumnos_inactivos == NULL) {
        cout << "No hay alumnos inactivos.\n";
        system("pause");
        return;
    }

    
    
    cout << "\n***Lista de alumnos inactivos***"<< endl;
	mostrar(lista_alumnos_inactivos);
	cout << "\n-------------------------------";
	cout << "\n-------------------------------\n";
		
    int opcion;
    do{
    	cout << "Buscar por:\n1. Matricula\n2. Nombre\nOpcion: ";
    	opcion = leerValor<int>(1,2);
		cout << opcion << " <-";	
    	if (opcion != 1 && opcion != 2)
    		cout << "Opcion invalida\n";
	}while (opcion != 1 && opcion != 2);

    Alumno* encontrado = NULL;
    if (opcion == 1) {
        int mat;
        cout << "Ingrese matricula: ";
        mat = leerValor<int>(0,999999);
        encontrado = buscarPorMatricula(lista_alumnos_inactivos, mat, NULL);
    } 
	else{
        string nombre;
        cout << "Ingrese nombre: ";
        nombre = leerCadena();
        encontrado = buscarPorNombre(lista_alumnos_inactivos, nombre);
	}

    if (encontrado == NULL) {
        cout << "Alumno no encontrado en la lista de bajas parciales.\n";
        system("pause");
    }
    else{
    	// Eliminar de la lista de bajas parciales
	    Alumno* eliminado = eliminarDeLista(lista_alumnos_inactivos, encontrado->matricula);
	
	    // Tambien eliminar de la pila
	    eliminarDeLista(pila_alumnos_inactivos, eliminado->matricula);
        //almacenar matricula para mostrarla
        int matricula = eliminado->matricula;

	    // Liberar memoria
	    delete eliminado;
	
	    cout << "Baja total completada. Alumno con matricula " << matricula <<" eliminado permanentemente.\n";
	    
	    
	    //**Eliminar las siguientes 3 lineas para no mostrar listas**
	    cout << "\n***Lista de alumnos inactivos***"<< endl;
	    mostrar(lista_alumnos_inactivos);
		cout << "\n-------------------------------";
		cout << "\n-------------------------------\n";
	    system("pause");
	}
}

//Se utiliza para libterar la memoria de la lista de alumnos activos, inactivos y pila de alumnos inactivos
void eliminar_lista(Lista &lista){
	Lista p;
    p = lista;
	cout<< endl;
	
	while(p!=NULL){
		lista = lista->next;
        delete(p);
        p = lista;
        }
}

//Imprime el menu principal y devuelve la opcion que ingresa el usuario
int mostrarMenu() {
    system("cls");	
	cout << "\n***MENU PRINCIPAL***"<< endl;
	cout << "\n**MENU**" << endl;
    cout << "1. Alta de alumnos\n";
    cout << "2. Baja de estudiantes\n";
    cout << "3. Recuperar alumno\n";
    cout << "4. Reportes\n";
    cout << "5. Modificacion de datos\n";
    cout << "6. Control de inscripciones\n";
    cout << "7. Creacion de grupos\n";
    cout << "8. Salir\n";
    cout << "Seleccione una opcion: ";
	int op = leerValor<int>(1,8);
	cout << op << " <-";
	return op;
}

//Es el submenu de la opcion de bajas, devuelve la opcion que ingresa el usuario
int submenu_bajas(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos){
	int opcion;
	do{
		system("cls");	
		cout << "\n***SUBMENU DE BAJA***"<< endl;
		cout << "\n**MENU**" << endl;
    	cout << "1. Baja parcial\n";
    	cout << "2. Deshacer ultima baja\n";
    	cout << "3. Baja total\n";
    	cout << "4. Salir\n";
    	cout << "Seleccione una opcion: ";
		opcion = leerValor<int>(1,4);
		cout << opcion << " <-";
		system("cls");	
		switch (opcion) {
            case 1: bajaParcial(lista_alumnos_activos, lista_alumnos_inactivos, pila_alumnos_inactivos); break;
            case 2:	deshacerBaja(lista_alumnos_activos, lista_alumnos_inactivos, pila_alumnos_inactivos); break;
            case 3: bajaTotal(lista_alumnos_inactivos, pila_alumnos_inactivos); break;
            case 4: cout << "Volviendo al menu principal...\n"; break;
        }
	}while (opcion != 4);
	
	return opcion;
}

//Aqui agregamos al alumno, se pide la informacion y lo agrega a la lista de alumnos activos
void altaAlumnos(Lista &lista_activos) {
	system("cls");	
    Alumno* nuevo = new Alumno;

    cout << "\n--- Alta de Alumno ---\n";
    cout << "Matricula: ";
    nuevo->matricula = leerValor<int>(0,99999999);

    // Validar si la matri­cula ya existe
    Alumno* temp = lista_activos;
    while (temp != NULL) {
        if (temp->matricula == nuevo->matricula) {
            cout << "Error: La matricula ya existe.\n";
            delete nuevo;
            cout << "Regresando al menu principal...\n";
            system("pause");
            return;
        }
        temp = temp->next;
    }
    cout << "Nombre: ";
    nuevo->nombre = leerCadena();
    cout << "Edad: ";
    nuevo->edad = leerValor<int>(1,100);
    cout << "Promedio: ";
    nuevo->promedio = leerValor<float>(0,100.0);
    cout << "Direccion: ";
    nuevo->direccion = leerCadena();
    cout << "Telefono: ";
    nuevo->telefono = leerValor<long long>(1000000000, 9999999999);

    nuevo->situacion = true; // Activo
    nuevo->next = NULL;

    insertarOrdenado(lista_activos, nuevo);
    total_activos++;
    cout << "Alumno agregado exitosamente.\n";

    cout << "\n******Lista de alumnos activos.******\n";
    mostrar(lista_activos);
	cout << "\n-------------------------------";
	cout << "\n-------------------------------\n";
	system("pause");
}

// Se utiliza para mostrar la lista de alumnos, se le pasa la lista y se imprime
void mostrar(const Lista lista){
	Alumno *aux = lista;
	if(lista == NULL){
		cout << "Lista vacia" << endl;
	}
	while(aux != NULL){
        cout << "\n-----------------------------------\n";
		cout << "Matricula: " << aux->matricula << endl
			<< "Nombres: " << aux->nombre << endl
			<< "Edad: " << aux->edad << endl
			<< "Promedio: " << aux->promedio << endl
			<< "Direccion: " << aux->direccion << endl
			<< "Telefono: " << aux->telefono << endl
			<< "Situacion: " << (aux->situacion == 1 ? "**Activo**" : "**Inactivo**") << endl;
		aux = aux->next;
	}
    cout << endl;
}

//Se utiliza para imprimir el menu de reportes y proceder con cada opcion que ingrese el usuario
void submenuReportes(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos){
	int opcion;
	do{
		system("cls");	
		cout << "\n***SUBMENU DE REPORTES***"<< endl;
		cout << "\n**MENU**" << endl;
    	cout << "1. Alumnos aprobados" << endl;
    	cout << "2. Porcentajes" << endl;
    	cout << "3. Datos generales" << endl;
    	cout << "4. Alumnos inactivos" << endl;
        cout << "5. Salir " << endl;
    	cout << "Seleccione una opcion: ";
		opcion = leerValor<int>(1,5);
		cout << opcion << " <-";
		system("cls");	
		switch (opcion) {
            case 1: mostrarAprobados(lista_alumnos_activos); system("pause"); break;
            case 2:	mostrarPorcentajes(lista_alumnos_activos); system("pause"); break;
            case 3: mostrarDatos(lista_alumnos_activos); break;
            case 4: mostrar(lista_alumnos_inactivos); system("pause"); break;
            case 5: cout << "Volviendo al menu principal...\n"; break;
        }
	}while (opcion != 5);
}

// Se utiliza para mostrar los alumnos aprobados, se le pasa la lista de alumnos activos y si cumple con promedio >= 70 se imprime la info del alumno
void mostrarAprobados(Lista &lista_alumnos_activos) {
    Alumno *aux = lista_alumnos_activos;
    if(lista_alumnos_activos == NULL) {
        cout << "No hay alumnos disponibles..." << endl;
    }
    while (aux != NULL) {
        if(aux->promedio >= 70) {
            cout << "\n-----------------------------------\n";
		    cout << "Matricula: " << aux->matricula << endl
			<< "Nombres: " << aux->nombre << endl
			<< "Promedio: " << aux->promedio << endl;
        }
        aux = aux->next;
    }
}

// Se utiliza para mostrar los porcentajes de alumnos aprobados y reprobados, se le pasa la lista de alumnos activos y se calcula el porcentaje
void mostrarPorcentajes(Lista &lista_alumnos_activos) { 
    Alumno *aux = lista_alumnos_activos;
    float n_aprobados = 0, n_reprobados = 0, n_total = 0;
    float porcentaje_aprobados, porcentaje_reprobados;
    if(lista_alumnos_activos == NULL) {
        cout << "No hay alumnos disponibles..." << endl;
    }
	while(aux != NULL){
        if(aux->promedio >= 70) {
            n_aprobados++;
        }
        else {
            n_reprobados++;
        }
        n_total++;
		aux = aux->next;
	}

    porcentaje_aprobados = (n_aprobados/n_total) * 100;
    porcentaje_reprobados = (n_reprobados/n_total) * 100;
    

    cout << "Porcentaje reprobados " << porcentaje_reprobados << "%" << endl;
    cout << "Porcentaje aprobados " << porcentaje_aprobados << "%" << endl;
}

//Aqui se muestran los datos del alumno dependiendo de la matricula o nombre que brinde el usuario 
void mostrarDatos(Lista &lista_alumnos_activos) {
    int opcion, matricula;
    string nombre;
    Alumno* estudiante;
    do{
		system("cls");	
		cout << "\n***SUBMENU DE REPORTES***"<< endl;
		cout << "\n**MENU DE BUSQUEDA   **" << endl;
    	cout << "1. Buscar por matricula" << endl;
    	cout << "2. Buscar por nombre" << endl;
        cout << "3. Salir" << endl;
    	cout << "Seleccione una opcion: ";
		opcion = leerValor<int>(1,3);
		cout << opcion << " <-";
		system("cls");	
        switch (opcion)
        {
        case 1:
            cout << "Ingrese la matricula del alumno: ";
            matricula = leerValor<int>(0,99999999);
            estudiante = buscarPorMatricula(lista_alumnos_activos,matricula,NULL);
            if(estudiante != NULL){
                cout << "\n-----------------------------------\n";
                cout << "Nombre: " << estudiante->nombre << endl;
                cout << "Edad: " << estudiante->edad << endl;
                cout << "Direccion: " << estudiante->direccion << endl;
                cout << "Telefono: " << estudiante->telefono << endl;
                system("pause");
            } else {
                cout << "Alumno no encontrado..." << endl;
                system("pause");
            }
            break;

        case 2:
            cout << "Ingrese el nombre del alumno: ";
            nombre = leerCadena();
            estudiante = buscarPorNombre(lista_alumnos_activos, nombre);
            if(estudiante != NULL){
                cout << "\n-----------------------------------\n";
                cout << "Nombre: " << estudiante->nombre << endl;
                cout << "Edad: " << estudiante->edad << endl;
                cout << "Direccion: " << estudiante->direccion << endl;
                cout << "Telefono: " << estudiante->telefono << endl;
                system("pause");
            } else {
                cout << "Alumno no encontrado..." << endl;
                system("pause");
            }
            break;
        case 3:
            break;
        }
    }while(opcion != 3);
}

//Esta funcion nos devuelve el nodo que esta en medio de la lista que utilizamos para la busqueda binaria
Alumno* nodo_medio(Lista cabeza, Lista fin){
    if (cabeza == NULL) {
        return NULL;
    }

    Alumno* lento = cabeza;
    Alumno* rapido = cabeza;

    
    while (rapido != fin && rapido->next != fin) {
        lento = lento->next;
        rapido = rapido->next->next;
    }

    return lento;
}

//Se utiliza la funcion de recuperar alumno para eliminar el nodo de la pila y lista de inactivos y pasarla nuevamente a la lista de activos
void recuperarAlumno(Lista &lista_alumnos_activos, Lista &lista_alumnos_inactivos, Pila &pila_alumnos_inactivos){
    system("cls");
    cout << "\n--- Recuperar alumno ---\n";
        if (pila_alumnos_inactivos == NULL) {
        cout << "No hay alumnos inactivos.\n";
        system("pause");
        return;
    }
	
	//**Eliminar las siguientes 7 lineas para no mostrar listas**
	cout << "\n***Lista de alumnos inactivos***"<< endl;
	mostrar(lista_alumnos_inactivos);
	cout << "\n-------------------------------";
	cout << "\n-------------------------------\n";
    cout << "\n***Pila de alumnos inactivos***"<< endl;
    mostrar(pila_alumnos_inactivos);
	cout << "\n-------------------------------";
	cout << "\n-------------------------------\n";

    int opcion;
    cout << "Buscar por:\n1. Matricula\n2. Nombre\nOpcion: ";
    opcion = leerValor<int>(1,2);

    Alumno* encontrado = NULL;
    if(opcion == 1){
        int mat;
        cout << "Ingrese matricula: ";
        mat = leerValor<int>(0,9999999);
        encontrado = buscarPorMatricula(lista_alumnos_inactivos, mat, NULL);
    } 
	else {
        string nombre;
        cout << "Ingrese nombre: ";
        nombre = leerCadena();
        encontrado = buscarPorNombre(lista_alumnos_inactivos, nombre);
	}

    if (encontrado == NULL) {
        cout << "Alumno no encontrado.\n";
        system("pause");
    }
	else{
		// Eliminarlo de la lista de inactivos
		Alumno* alta = eliminarDeLista(lista_alumnos_inactivos, encontrado->matricula);
    	if (alta != NULL){
        	alta->situacion = true;
	
    	    // Insertar ordenadamente en la lista de activos
        	insertarOrdenado(lista_alumnos_activos, alta);
	
    	    // Retirar de la pila de bajas 
			eliminarDeLista(pila_alumnos_inactivos, encontrado->matricula);
	
    	    cout << "Alumno reactivado con exito.\n";
    	    
    	    //**Eliminar las siguientes 6 lineas para no mostrar listas**
        	cout << "\n***Lista de alumnos activos***"<< endl;
            mostrar(lista_alumnos_activos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";
        	cout << "\n***Pila de alumnos inactivos***"<< endl;
        	mostrar(pila_alumnos_inactivos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";
    	} else {
	        cout << "Error al procesar alta.\n";
	    }
	    system("pause");
	}
}

//Utilizado en el menu de modificar datos, dependiendo los datos que quiera modificar el usuario
void modificarDatos(Lista &lista_alumnos_activos) {
    system("cls");
    cout << "\n--- Modificar datos de alumno activo ---\n";

    	    //**Eliminar las siguientes 6 lineas para no mostrar listas**
        	cout << "\n***Lista de alumnos activos***"<< endl;
            mostrar(lista_alumnos_activos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";

    int opcionBusqueda;
    cout << "Buscar por:\n1. Matricula\n2. Nombre\nOpcion: ";
    opcionBusqueda = leerValor<int>(1, 2);

    Alumno* encontrado = NULL;
    if(opcionBusqueda == 1){
        int mat;
        cout << "Ingrese matricula: ";
        mat = leerValor<int>(0, 9999999);
        encontrado = buscarPorMatricula(lista_alumnos_activos, mat, NULL);
    } else {
        string nombre;
        cout << "Ingrese nombre: ";
        nombre = leerCadena();
        encontrado = buscarPorNombre(lista_alumnos_activos, nombre);
    }

    if (encontrado == NULL) {
        cout << "Alumno no encontrado.\n";
        system("pause");
        return;
    }

    int opcionCampo;
    cout << "\nModificar:\n1. Matricula\n2. Nombre\n3. Edad\n4. Promedio general\n5. Direccion\n6. Telefono\nOpcion: ";
    opcionCampo = leerValor<int>(1, 6);

    //se modficia el campo especifico que se requiera modificar
    switch (opcionCampo) {
        case 1: { // Matricula
            cout << "Nueva matricula: ";
            int nuevaMat = leerValor<int>(0, 99999999);

            // Validar que no exista
            Alumno* temp = lista_alumnos_activos;
            while (temp != NULL) {
                if (temp != encontrado && temp->matricula == nuevaMat) {
                    cout << "Error: La matricula ya existe.\n";
                    system("pause");
                    return;
                }
                temp = temp->next;
            }

            // Eliminar y reinsertar para mantener orden
            Alumno* modificado = eliminarDeLista(lista_alumnos_activos, encontrado->matricula);
            modificado->matricula = nuevaMat;
            insertarOrdenado(lista_alumnos_activos, modificado);

            cout << "Matricula modificada exitosamente.\n";
            break;
        }
        case 2: { // Nombre
            cout << "Nuevo nombre: ";
            encontrado->nombre = leerCadena();
            break;
        }
        case 3: { // Edad
            cout << "Nueva edad: ";
            encontrado->edad = leerValor<int>(1, 100);
            break;
        }
        case 4: { // Promedio
            cout << "Nuevo promedio: ";
            encontrado->promedio = leerValor<float>(0, 100.0);
            break;
        }
        case 5: { // Direccion
            cout << "Nueva direccion: ";
            encontrado->direccion = leerCadena();
            break;
        }
        case 6: { // Telefono
            cout << "Nuevo telefono: ";
            encontrado->telefono = leerValor<long long>(1000000000, 9999999999);
            break;
        }
    }

     	    //**Eliminar las siguientes 6 lineas para no mostrar listas**
        	cout << "\n***Lista de alumnos activos***"<< endl;
            mostrar(lista_alumnos_activos);
			cout << "\n-------------------------------";
			cout << "\n-------------------------------\n";   

    cout << "Datos modificados correctamente.\n";
    system("pause");
}

//Contabiliza el total de alumnos en la lista de activos
int totalAlumnos(Lista lista_alumnos_activos){
    if (lista_alumnos_activos == NULL)  // Si la lista está vacía
    {
        cout << "La lista de alumnos activos está vacía." << endl;
        return 0;
    }
    
    int total = 0;
    Alumno* aux = lista_alumnos_activos;
    while(aux != NULL){
        total++;
        aux = aux->next;
    }
    return total;
}

//funcion utilizada para crear los grupos de manera equitativa, con una diferencia de un alumno entre los grupos cuando mucho
void crearGrupos(Lista alumnos_activos){
    system("cls");
    if (alumnos_activos == NULL) {
        cout << "No hay alumnos disponibles para crear grupos." << endl;
        return;
    }
    Alumno* aux = alumnos_activos;

    
    cout << "\n--- Crear grupos ---\n";
    cout << "Ingrese el numero de grupos: ";
    int grupos = leerValor<int>(1, totalAlumnos(alumnos_activos));//Se pide el total de grupos
    int alumnosPorGrupo = totalAlumnos(alumnos_activos) / grupos;//Obtenemos el total de alumnos por grupo (los restantes no los contamos aun)
    int alumnosRestantes = totalAlumnos(alumnos_activos) % grupos;//se obtiene el numero de alumnos restantes (seran los que se agreguen como extra a cada grupo)

    //Es el for principal donde hacemos los ciclos para asignar los grupos
    for (int i = 0; i < grupos; i++)
    {
        cout << "\n***Grupo " << i + 1 << "***" << endl;
        //Se utiliza este for para agregar la cantidad de alumnos que le toca a cada grupo
        for (int j = 0; j < alumnosPorGrupo; j++)
        {
            if (alumnos_activos != NULL)
            {
                mostrarAlumno(aux);
                aux = aux->next;
            }
        }
        if (alumnosRestantes > 0)//Si hay alumnos restantes se agrega uno por cada pasada, asi a cada grupo se le agregara uno y sera de manera equitativa la reparticion
        {
            mostrarAlumno(aux);
            aux = aux->next;
            alumnosRestantes--;
        }
        cout << endl;
    }
    cout << "\nGrupos creados exitosamente.\n";
    system("pause");
}