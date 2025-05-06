#ifndef USUARIOREGISTRADO_HPP
#define USUARIOREGISTRADO_HPP

#include "Usuario.hpp"
#include "CuentaBancaria.hpp"


#include <string>
#include <vector>

using namespace std;

class UsuarioRegistrado : public Usuario{
    private:
        int id;
        string contrasena;
        vector<CuentaBancaria*> cuentasBancarias; // Vector de punteros a cuentas bancarias, si no se usan los punteros, se copiarían las cuentas bancarias al vector y se duplicarían en memoria ya que tenemos dos vectores de cuentasBancarias uno aquí y otro en la clase Banco

    public:

    UsuarioRegistrado(int id, string nombre, string dni, string contrasena);


    int getId();
    string getContrasena();
    vector<CuentaBancaria*>& getCuentasBancarias(); // Devuelve una referencia al vector de cuentas bancarias.

    void setContrasena(string contrasena);

    void mostrarInformacion() override;

    void agregarCuentaBancaria(CuentaBancaria& cuentaBancaria);


};

#endif // USUARIOREGISTRADO_HPP