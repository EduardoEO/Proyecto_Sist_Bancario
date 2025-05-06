#include "../include/Usuario.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/CuentaBancaria.hpp"

#include <iostream>

using namespace std;

UsuarioRegistrado::UsuarioRegistrado(int id, string nombre, string dni, string contrasena) : Usuario(nombre, dni){
    this->id = id;
    this->contrasena = contrasena;
}

int UsuarioRegistrado::getId(){
    return id;
}

string UsuarioRegistrado::getContrasena(){
    return contrasena;
}


vector<CuentaBancaria*>& UsuarioRegistrado::getCuentasBancarias(){
    return cuentasBancarias;
}

void UsuarioRegistrado::setContrasena(string contrasena){
    this->contrasena = contrasena;
}

void UsuarioRegistrado::mostrarInformacion(){
    cout << "Nombre: " << getNombre() << endl;
    cout << "DNI: " << getDni() << endl;
    cout << "Contraseña: " << getContrasena() << endl;
    cout << endl;
}

void UsuarioRegistrado::agregarCuentaBancaria(CuentaBancaria& cuentaBancaria){
    cuentasBancarias.push_back(&cuentaBancaria);
}
