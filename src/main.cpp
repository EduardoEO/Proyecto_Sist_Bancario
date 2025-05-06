#include "../include/CuentaBancaria.hpp"
#include "../include/Menu.hpp"
#include "../include/Usuario.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/UsuarioNoRegistrado.hpp"
#include "../include/Transaccion.hpp"
#include "../include/TipoCuenta.hpp"
#include "../include/TipoTarjeta.hpp"
#include "../include/TipoEstado.hpp"
#include "../include/Banco.hpp"
#include "../include/GestorArchivos.hpp"

#include <iostream>
#include <windows.h>

using namespace std;

int main(){  
    try{  
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        Banco banco("Maze Bank");
    
        banco.cargarDatosDesdeArchivos();
    
        Menu menu;
        menu.mostrarMenuIniSesion(banco);

        // Guardar los datos al salir del programa
        banco.guardarDatos();
    
        cout << "Saliendo del programa..." << endl;
    } catch (const exception& e){
        cerr << "Error inesperado: " << e.what() << endl; 
    } catch (...){
        cerr << "Error desconocido. El programa se cerrará." << endl;
    }
    
    return 0;
}