#ifndef CUENTABANCARIA_HPP
#define CUENTABANCARIA_HPP

#include "TipoCuenta.hpp"
#include "Transaccion.hpp"
#include "TarjetaBancaria.hpp"

#include <string>
#include <vector>

using namespace std;

class CuentaBancaria {
    private:
        string IBAN; // El IBAN es un identificador único de la cuenta bancaria. Se compone de 24 caracteres, siendo los dos primeros el código del país, los dos siguientes el dígito de control y los 20 restantes el número de cuenta.
        int idUsuario;
        double saldo;
        TipoCuenta tipoCuenta;
        vector<Transaccion> transacciones;
        vector<TarjetaBancaria> tarjetasBancarias;
    
    public:
        // Constructor
        CuentaBancaria(string IBAN, int idUsuario, double saldo, TipoCuenta tipoCuenta, vector<Transaccion> transacciones, vector<TarjetaBancaria> tarjetasBancarias);
        CuentaBancaria(string IBAN, int idUsuario, double saldo, TipoCuenta tipoCuenta); // Constructor adicional para crear una cuenta bancaria sin transacciones ni tarjetas bancarias. Usado para introducir los datos al .txt

        // Getters y setters
        string getIBAN();
        int getIdUsuario();
        double getSaldo();
        TipoCuenta getTipoCuenta();
        string getTipoCuentaStr();
        vector<Transaccion>& getTransacciones(); // Devuelve una referencia al vector de transacciones.
        vector<TarjetaBancaria>& getTarjetasBancarias(); // Devuelve una referencia al vector de tarjetas bancarias.
       
        void setSaldo(double saldo);

        void retirar(double monto);
        void depositar(double monto);

        bool esTarjetaCompatible(TarjetaBancaria tarjeta);

        void agregarTransaccion(const Transaccion& transaccion); // Agrega una transacción al vector de transacciones.
        void agregarTarjeta(const TarjetaBancaria& tarjeta); // Agrega una tarjeta al vector de punteros de tarjetas bancarias.
    
        CuentaBancaria& operator+=(CuentaBancaria& otraCuentaBancaria);
};
#endif // CUENTABANCARIA_HPP