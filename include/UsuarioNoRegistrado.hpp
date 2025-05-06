#ifndef USUARIONOREGISTRADO_HPP
#define USUARIONOREGISTRADO_HPP

#include "Usuario.hpp"

using namespace std;

class UsuarioNoRegistrado : public Usuario{
    // Atributos
    private:
        string contrasena;
    // Métodos
    public:
    UsuarioNoRegistrado(string nombre, string dni, string contrasena);

    string getContrasena();
    void setContrasena(string contrasena);

    void mostrarInformacion() override;
};

#endif // USUARIONOREGISTRADO_HPP
