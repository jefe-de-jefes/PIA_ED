/*Lia Fernanda Pardo Mireles 2096765​ 
 * Alejandro Quintanilla Leal 2010568​
 * Angel Joseph Meraz Hernandez 2067151
 * Luis Fernando Segobia Torres 2177528
 * */
#include<iostream>
#include<ctime>
#include<limits>

using namespace std;
template <typename T>
T leerValor(T menor, T mayor){
    T valor;
    while(true)
    {
        cin >> valor;
        if(cin.fail())
        {
            cout << "Numero no valido, intente nuevamente.." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if(valor < menor || valor > mayor)
        {
            cout << "\n**Numero fuera de rango, intente nuevamente**" << endl;
			continue;
        }else{
            return valor;
        }
    }

}

struct Alumno{
    int matricula;
    string name;
    int edad;
};



int main(){

    return 0;
}