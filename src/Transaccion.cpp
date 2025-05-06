#include "../include/Transaccion.hpp"

#include <iostream>
#include <sstream>

using namespace std;

Transaccion::Transaccion(int id, string ordenante, string beneficiario, double monto, string concepto, string fecha) : id(id), ordenante(ordenante), beneficiario(beneficiario), monto(monto), concepto(concepto), fecha(fecha){
    if (!fecha.empty() && fecha.back() == '\n'){
        fecha.pop_back(); // Eliminar el salto de línea al final de la fecha.
    }
    this->fecha = fecha;
}

int Transaccion::getId(){
    return id;
}

string Transaccion::getOrdenante(){
    return ordenante;
}

string Transaccion::getBeneficiario(){
    return beneficiario;
}

double Transaccion::getMonto(){
    return monto;
}

string Transaccion::getConcepto(){
    return concepto;
}

string Transaccion::getFecha(){
    if (!fecha.empty() && fecha.back() == '\n'){
        fecha.pop_back(); // Eliminar el salto de línea al final de la fecha.
    }
    return fecha;
}

void Transaccion::mostrarDetalles() const{
    cout << "ID de la transacción: " << id << endl;
    cout << "Ordenante: " << ordenante << endl;
    cout << "Beneficiario: " << beneficiario << endl;
    cout << "Monto: " << monto << endl;
    cout << "Concepto: " << concepto << endl;
    cout << "Fecha: " << fecha << endl;
}

string Transaccion::mostrarDetallesStr() const{
    ostringstream oss;
    oss << "ID de la transacción: " << id << endl;
    oss << "Ordenante: " << ordenante << endl;
    oss << "Beneficiario: " << beneficiario << endl;
    oss << "Monto: " << monto << endl;
    oss << "Concepto: " << concepto << endl;
    oss << "Fecha: " << fecha << endl;
    oss << "-----------------------------------------" << endl;
    return oss.str();
}