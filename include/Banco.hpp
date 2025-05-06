#ifndef BANCO_HPP
#define BANCO_HPP

#include "CuentaBancaria.hpp"
#include "UsuarioRegistrado.hpp"
#include "Transaccion.hpp"
#include "TarjetaBancaria.hpp"
#include "UsuarioNoRegistrado.hpp"

#include <string>
#include <vector>

using namespace std;

class Banco {
    private:
        string nombre;
        vector<UsuarioRegistrado> usuariosRegistrados;
        vector<CuentaBancaria*> cuentasBancarias;
        vector<Transaccion> transacciones;
        vector<TarjetaBancaria> tarjetasBancarias;
        UsuarioRegistrado* usuarioAutenticado; // Con esto nos ahorramos buscar en cada método si el usuario está autenticado o no.
    
    public:
    // Constructor
    Banco(string nombre); // Usuario autenticado por defecto es nulo.
    ~Banco();

    // Getters
    string getNombre();
    UsuarioRegistrado* getUsuarioAutenticado(); // ¡¡¡IMPORTANTE!!! Devuelve un puntero al usuario autenticado.
    vector<Transaccion>& getTransacciones();

    // Método de carga de datos desde archivos

    void cargarDatosDesdeArchivos();

    void asociarUsuariosConCuentas();
    void asociarCuentasConTransacciones();
    void asociarCuentasConTarjetas();

    // Métodos de guardado de datos en archivos

    void guardarDatos();

    // Métodos relacionados al usuarioRegistrado
    UsuarioRegistrado* iniciarSesion(const string& dni, const string& contrasena);
    void registrarUsuario(UsuarioNoRegistrado& usuarioNoRegistrado);
    
    UsuarioRegistrado* buscarUsuario(int id);
    string mostrarCuentasBancariasUsuario(UsuarioRegistrado* usuario);
    void crearCuentaBancaria(UsuarioRegistrado* usuario);
    bool ibanExistente(const string& IBAN);

    string mostrarTarjetasBancarias(CuentaBancaria* CuentaBancaria);

    // Métodos relacionados a las cuentas bancarias
    void retirar(CuentaBancaria* cuentaBancaria, double monto);
    void depositar(CuentaBancaria* cuentaBancaria, double monto);
    void transferencia(const string& ibanOrigen, const string& ibanDestino, double monto);
    
    void crearTarjetaBancaria(CuentaBancaria* cuentaBancaria);
    bool numeroTarjetaExistente(const string& numeroTarjeta);

    void eliminarCuenta(CuentaBancaria* cuenta);

    void eliminarUsuario(UsuarioRegistrado* usuario);
};

#endif // BANCO_HPP