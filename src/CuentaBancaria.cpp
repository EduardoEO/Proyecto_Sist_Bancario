#include "../include/CuentaBancaria.hpp"
#include "../include/TipoCuenta.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/Transaccion.hpp"
#include "../include/TarjetaBancaria.hpp"
#include "../include/TipoTarjeta.hpp"
#include "../include/TipoEstado.hpp"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constructor
CuentaBancaria::CuentaBancaria(string IBAN, int idUsuario, double saldo, TipoCuenta tipoCuenta, vector<Transaccion> transacciones, vector<TarjetaBancaria> tarjetasBancarias){
    this->IBAN = IBAN;
    this->idUsuario = idUsuario;
    this->saldo = saldo;
    this->tipoCuenta = tipoCuenta;
}
// Constructor adicional para crear una cuenta bancaria sin transacciones ni tarjetas bancarias. Usado para introducir los datos al .txt
CuentaBancaria::CuentaBancaria(string IBAN, int idUsuario, double saldo, TipoCuenta tipoCuenta) : IBAN(IBAN), idUsuario(idUsuario), saldo(saldo), tipoCuenta(tipoCuenta), transacciones(), tarjetasBancarias() {}

// Getters y setters

string CuentaBancaria::getIBAN(){
    return IBAN;
}

int CuentaBancaria::getIdUsuario(){
    return idUsuario;
}

double CuentaBancaria::getSaldo(){
    return saldo;
}

TipoCuenta CuentaBancaria::getTipoCuenta(){
    return tipoCuenta;
}

string CuentaBancaria::getTipoCuentaStr(){
    switch(tipoCuenta){
        case AHORRO:
            return "Ahorro";
        case CORRIENTE:
            return "Corriente";
        case NOMINA:
            return "Nómina";
        case INVERSION:
            return "Inversión";
        case EMPRESA:
            return "Empresa";
    }
    return "Desconocido";
}

vector<Transaccion>& CuentaBancaria::getTransacciones(){
    return transacciones;
}

vector<TarjetaBancaria>& CuentaBancaria::getTarjetasBancarias(){
    return tarjetasBancarias;
}

void CuentaBancaria::setSaldo(double saldo){
    this->saldo = saldo;
}

void CuentaBancaria::retirar(double monto){
    if (monto > saldo){
        cout << "Error: Saldo insuficiente." << endl;
        return;
    }
    setSaldo(saldo - monto);
}

void CuentaBancaria::depositar(double monto){
    if (monto <= 0){
        cout << "Error: Monto inválido." << endl;
        return;
    }
    setSaldo(saldo + monto);
}

bool CuentaBancaria::esTarjetaCompatible(TarjetaBancaria tarjeta) {
    if (tipoCuenta == TipoCuenta::INVERSION && (tarjeta.getTipoTarjeta() == TipoTarjeta::CREDITO || tarjeta.getTipoTarjeta() == TipoTarjeta::DEBITO)){
        return false;
    }
    if (tipoCuenta == TipoCuenta::AHORRO && tarjeta.getTipoTarjeta() == TipoTarjeta::CREDITO){
        return false;
    }
    return true;
}

void CuentaBancaria::agregarTransaccion(const Transaccion& transaccion){
    transacciones.push_back(transaccion);
}

void CuentaBancaria::agregarTarjeta(const TarjetaBancaria& tarjeta){
    tarjetasBancarias.push_back(tarjeta);
}

CuentaBancaria& CuentaBancaria::operator+=(CuentaBancaria& otraCuentaBancaria){
    this->saldo += otraCuentaBancaria.saldo;

    this->transacciones.insert(this->transacciones.end(), otraCuentaBancaria.transacciones.begin(), otraCuentaBancaria.transacciones.end());
    
    //Transferir las tarjetas y asignarlas a la cuenta actual.
    auto it = otraCuentaBancaria.tarjetasBancarias.begin();
    while (it != otraCuentaBancaria.tarjetasBancarias.end()){
        if (this->esTarjetaCompatible(*it)){
            it->setIBAN(this->IBAN); // Cambiar el IBAN de la tarjeta a la cuenta actual.
            this->tarjetasBancarias.push_back(*it); // Agregar la tarjeta a la cuenta actual.
            it = otraCuentaBancaria.tarjetasBancarias.erase(it);
        } else {
            cout << "La tarjeta " << it->getNumeroTarjeta() << " no es compatible con la cuenta actual y será eliminada." << endl;
            it = otraCuentaBancaria.tarjetasBancarias.erase(it);
        }
    }

    otraCuentaBancaria.transacciones.clear();

    return *this;
}
