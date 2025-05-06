#ifndef MENU_HPP
#define MENU_HPP

#include "Banco.hpp"
#include "UsuarioRegistrado.hpp"

// Pasamos banco siempre por referencia para no tener que crear una copia del objeto cada vez que llamamos a un método de la clase Banco.
// Esto es importante para evitar problemas de memoria y mejorar el rendimiento del programa.

// Usamos punteros con las clases UsuarioRegistrado y CuentaBancaria para evitar copias innecesarias de objetos grandes y para poder modificar los objetos originales si es necesario.

class Menu {
    public:
    void mostrarMenuIniSesion(Banco& banco);
    void iniciarSesion(Banco& banco);
    void mostrarMenuRegistro(Banco& banco);
    void mostrarMenuUsuario(Banco& banco);
    void mostrarInformacionUsuario(UsuarioRegistrado* usuario);
    void cambiarContrasena(UsuarioRegistrado* usuario);
    void seleccionarCuentaBancaria(Banco& banco, UsuarioRegistrado* usuario);
    void mostrarMenuPrincipal(Banco& banco, CuentaBancaria* cuentaBancaria);
    void mostrarHistorialTransacciones(Banco& banco, CuentaBancaria* cuentaBancaria);
    void mostrarMenuSolicitarTarjetaBancaria(Banco& banco, CuentaBancaria* cuentaBancaria);
    void fusionarCuentasBancarias(Banco& banco, CuentaBancaria* cuentaBancariaActual);
    void seleccionarTarjetaBancaria(Banco& banco, CuentaBancaria* cuentaBancaria);
    void mostrarMenuTarjetaBancaria(Banco& banco, TarjetaBancaria& tarjetaBancaria);
    void cambiarPIN(TarjetaBancaria& tarjetaBancaria);
    void renovarTarjetaBancaria(Banco& banco, TarjetaBancaria& tarjetaBancaria);
};



#endif // MENU_HPP