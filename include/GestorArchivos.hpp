#ifndef GESTORARCHIVOS_HPP
#define GESTORARCHIVOS_HPP

#include "../include/UsuarioRegistrado.hpp"
#include "../include/CuentaBancaria.hpp"
#include "../include/Transaccion.hpp"
#include "../include/TarjetaBancaria.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class GestorArchivos {
public:

    // Métodos para cargar datos en archivos
    vector<UsuarioRegistrado> cargarUsuarios(); // El objetivo del método es cargar los usuarios registrados desde usuariosRegistrados.txt. Desde un vector vacío de objetos UsuarioRegistrado, se cargan los datos desde el archivo y se devuelven como un vector de objetos UsuarioRegistrado.
    vector<CuentaBancaria> cargarCuentasBancarias();
    vector<Transaccion> cargarTransacciones();
    vector<TarjetaBancaria> cargarTarjetasBancarias();

    // Métodos para guardar datos en archivos
    void guardarUsuarios(vector<UsuarioRegistrado>& usuarios);
    void guardarCuentas(vector<CuentaBancaria>& cuentas);
    void guardarTransacciones(vector<Transaccion>& transacciones);
    void guardarTarjetas(vector<TarjetaBancaria>& tarjetas);


};

#endif // GESTORARCHIVOS_HPP
