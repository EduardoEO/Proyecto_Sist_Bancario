#ifndef TARJETABANCARIA_HPP
#define TARJETABANCARIA_HPP

#include "TipoTarjeta.hpp"
#include "TipoEstado.hpp"

#include <string>

using namespace std;

class TarjetaBancaria{
    private:
        string numeroTarjeta; // 16 dígitos
        string IBAN;
        string fechaCaducidad; // 5 años de validez, formato MM/AA siendo el último dia del MM cuando caduca.
        string CVV; // 3 dígitos
        TipoTarjeta tipoTarjeta;
        TipoEstado estado;
        string PIN; // 6 dígitos, puede contener caractéres, al crear una tarjeta tu eres el que se asigna el PIN. 

    public:
        // Constructor
        TarjetaBancaria(string numeroTarjeta, string IBAN, string fechaCaducidad, string CVV, TipoTarjeta TipoTarjeta, TipoEstado estado, string PIN);
        // Getters y setters
        string getNumeroTarjeta();
        string getIBAN() ;
        string getFechaCaducidad();
        string getCVV() ;
        TipoTarjeta getTipoTarjeta();
        TipoEstado getEstado();
        string getPIN();
        void setIBAN(string IBAN);
        void setFechaCaducidad(string fechaCaducidad);
        void setEstado(TipoEstado estado);
        void setCVV(string CVV);
        void setPIN(string PIN);

        // Métodos
        string getTipoTarjetaStr(TipoTarjeta tipoTarjeta); // Convierte el tipo de tarjeta a string.
        string getEstadoStr(TipoEstado estado) ; // Convierte el estado a string.
        void mostrarDetalles(); // Muestra los detalles de la tarjeta bancaria.
};

#endif // TARJETABANCARIA_HPP