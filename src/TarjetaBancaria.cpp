#include "../include/TarjetaBancaria.hpp"
#include "../include/TipoTarjeta.hpp"
#include "../include/TipoEstado.hpp"

#include <iostream>
#include <string>

using namespace std;

// Constructor
TarjetaBancaria::TarjetaBancaria(string numeroTarjeta, string IBAN, string fechaCaducidad, string CVV, TipoTarjeta tipoTarjeta, TipoEstado estado, string PIN) : numeroTarjeta(numeroTarjeta), IBAN(IBAN), fechaCaducidad(fechaCaducidad), CVV(CVV), tipoTarjeta(tipoTarjeta), estado(estado), PIN(PIN) {}

// Getters y setters
string TarjetaBancaria::getNumeroTarjeta(){
    return numeroTarjeta;
}
string TarjetaBancaria::getIBAN(){
    return IBAN;
}
string TarjetaBancaria::getFechaCaducidad(){
    return fechaCaducidad;
}
string TarjetaBancaria::getCVV(){
    return CVV;
}
TipoTarjeta TarjetaBancaria::getTipoTarjeta(){
    return tipoTarjeta;
}
TipoEstado TarjetaBancaria::getEstado(){
    return estado;
}

string TarjetaBancaria::getPIN() {
    return PIN;
}

void TarjetaBancaria::setIBAN(string IBAN){
    this->IBAN = IBAN;
}

void TarjetaBancaria::setCVV(string CVV){
    this->CVV = CVV;
}

void TarjetaBancaria::setFechaCaducidad(string fechaCaducidad){
    this->fechaCaducidad = fechaCaducidad;
}

void TarjetaBancaria::setEstado(TipoEstado estado){
    this->estado = estado;
}

void TarjetaBancaria::setPIN(string PIN){
    this->PIN = PIN;
}

// Métodos
string TarjetaBancaria::getTipoTarjetaStr(TipoTarjeta tipoTarjeta) {
    switch(tipoTarjeta){
        case CREDITO:
            return "Crédito";
        case DEBITO:
            return "Débito";
        default:
            return "Desconocido";
    }
}

string TarjetaBancaria::getEstadoStr(TipoEstado estado) {
    switch(estado){
        case ACTIVA:
            return "Activa";
        case BLOQUEADA:
            return "Bloqueada";
        case CADUCADA:
            return "Caducada";
        default:
            return "Desconocida";
    }
}



void TarjetaBancaria::mostrarDetalles(){
    cout << "Número de tarjeta: " << getNumeroTarjeta() << endl;
    cout << "IBAN: " << getIBAN() << endl;
    cout << "Fecha de caducidad: " << getFechaCaducidad() << endl;
    cout << "CVV: " << getCVV() << endl;
    cout << "Tipo de tarjeta: " << getTipoTarjetaStr(tipoTarjeta) << endl;
    cout << "Estado: " << getEstadoStr(estado) << endl;
}