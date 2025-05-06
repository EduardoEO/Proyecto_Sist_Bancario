#include "../include/UsuarioNoRegistrado.hpp"
#include "../include/Usuario.hpp"

#include <iostream>

using namespace std;

UsuarioNoRegistrado::UsuarioNoRegistrado(string nombre, string dni, string contrasena) : Usuario(nombre, dni){
    this->contrasena = contrasena;
}

string UsuarioNoRegistrado::getContrasena(){
    return contrasena;
}

void UsuarioNoRegistrado::mostrarInformacion() {}

