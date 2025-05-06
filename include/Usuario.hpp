#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
using namespace std;


class Usuario {
    protected:
        string nombre;
        string dni;

    public:
        Usuario(string nombre, string dni);
        string getNombre();
        string getDni();
        void setNombre(string nombre);
        void setDni(string dni);

        virtual void mostrarInformacion() = 0;
};

#endif // USUARIO_HPP