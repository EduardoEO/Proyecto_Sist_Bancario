#include "../include/Usuario.hpp"

#include <iostream>

using namespace std;

Usuario::Usuario(string nombre, string dni){
    this->nombre = nombre;
    this->dni = dni;
}
string Usuario::getNombre(){
    return nombre;
}
string Usuario::getDni(){
    return dni;
}

void Usuario::setNombre(string nombre){
    this->nombre = nombre;
}

void Usuario::setDni(string dni){
    this->dni = dni;
}
 
void Usuario::mostrarInformacion() {}

