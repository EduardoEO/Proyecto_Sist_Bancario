#include "../include/GestorArchivos.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/CuentaBancaria.hpp"
#include "../include/Transaccion.hpp"
#include "../include/TarjetaBancaria.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<UsuarioRegistrado>GestorArchivos::cargarUsuarios(){
    vector<UsuarioRegistrado> usuarios;
    ifstream archivo("usuariosRegistrados.txt");
    
    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de usuarios." << endl; // cerr sirve para mostrar errores
        return usuarios; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    string linea;
    while (getline(archivo, linea)){ // Con esto lees la línea completa del archivo
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string idStr, nombre, dni, contrasena;
        
        getline(ss, idStr, '|'); // Con esto lles únicamente hasta el delimitador '|'
        getline(ss, nombre, '|');
        getline(ss, dni, '|');
        getline(ss, contrasena, '|');

        if (idStr.empty() || nombre.empty() || dni.empty() || contrasena.empty()){
            cerr << "Error: Línea incompleta en el archivo de usuarios." << endl; // Si hay un error en la línea, se muestra un mensaje de error.
            continue; // Saltar a la siguiente línea
        }

        try  {
            int id = stoi(idStr); // Convertir el ID de string a int

            UsuarioRegistrado usuario(id, nombre, dni, contrasena); // Crear el objeto UsuarioRegistrado con los datos leídos de la línea
            usuarios.push_back(usuario); // Agregar el usuario al vector
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Ecepción: " << e.what() << endl;
            continue;
        }
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Usuarios cargados correctamente." << endl;
    return usuarios; // Retornar el vector de usuarios
}

vector<CuentaBancaria>GestorArchivos::cargarCuentasBancarias(){
    vector<CuentaBancaria> cuentas;
    ifstream archivo("cuentasBancarias.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de cuentas." << endl;
        return cuentas; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    string linea;
    while (getline(archivo, linea)){
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string iban, idUsuarioStr, saldoStr, tipoCuentaStr;

        getline(ss, iban, '|');
        getline(ss, idUsuarioStr, '|');
        getline(ss, saldoStr, '|');
        getline(ss, tipoCuentaStr, '|');

        if (iban.empty() ||idUsuarioStr.empty() || saldoStr.empty() || tipoCuentaStr.empty()){
            cerr << "Error: Línea con campos vacíos: " << linea << endl;
        }

        try {
            int idUsuario = stoi(idUsuarioStr); // Convertir el ID de string a int
            double saldo = stod(saldoStr); // Convertir el saldo de string a double
            int tipoCuenta = stoi(tipoCuentaStr); // Convertir el tipo de cuenta de string a int
            TipoCuenta tipoCuentaEnum = static_cast<TipoCuenta>(tipoCuenta); // Convertir el tipo de cuenta de un int a TipoCuenta. Si no se hace esto, el constructor no funcionará correctamente ya que solo acepta TipoCuenta.

            CuentaBancaria cuenta(iban, idUsuario, saldo, tipoCuentaEnum); // Crear el objeto CuentaBancaria con los datos leídos de la línea
            cuentas.push_back(cuenta); // Agregar la cuenta al vector
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Ecepción: " << e.what() << endl;
            continue;
        }
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Cuentas cargadas correctamente." << endl;
    return cuentas; // Retornar el vector de cuentas
}

vector<Transaccion>GestorArchivos::cargarTransacciones(){
    vector<Transaccion> transacciones;
    ifstream archivo("transacciones.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de transacciones." << endl;
        return transacciones; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    string linea;
    while (getline(archivo, linea)){
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string idTransaccionStr, ibanOrigen, ibanDestino, montoStr, concepto, fecha;

        getline(ss, idTransaccionStr, '|');
        getline(ss, ibanOrigen, '|');
        getline(ss, ibanDestino, '|');
        getline(ss, montoStr, '|');
        getline(ss, concepto, '|');
        getline(ss, fecha, '|');

        if (idTransaccionStr.empty() || ibanOrigen.empty() ||ibanDestino.empty() || montoStr.empty() || concepto.empty() || fecha.empty()){
            cerr << "Error: Línea con campos vacíos: " << linea << endl;
            continue;
        }

        try {
            int idTransaccion = stoi(idTransaccionStr); // Convertir el ID de string a int
            double monto = stod(montoStr); // Convertir el monto de string a double
        
            Transaccion transaccion(idTransaccion, ibanOrigen, ibanDestino, monto, concepto, fecha); // Crear el objeto Transaccion con los datos leídos de la línea
            transacciones.push_back(transaccion); // Agregar la transacción al vector
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
        }  
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Transacciones cargadas correctamente." << endl;
    return transacciones; // Retornar el vector de transacciones
}

vector<TarjetaBancaria>GestorArchivos::cargarTarjetasBancarias(){
    vector<TarjetaBancaria> tarjetas;
    ifstream archivo("tarjetasBancarias.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de tarjetas." << endl;
        return tarjetas; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    string linea;
    while (getline(archivo, linea)){
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string numeroTarjeta, iban, fechaCaducidad, cvv, tipoTarjetaStr, estadoStr, pin;

        getline(ss, numeroTarjeta, '|');
        getline(ss, iban, '|');
        getline(ss, fechaCaducidad, '|');
        getline(ss, cvv, '|');
        getline(ss, tipoTarjetaStr, '|');
        getline(ss, estadoStr, '|');
        getline(ss, pin, '|');

        if (numeroTarjeta.empty() || iban.empty() || fechaCaducidad.empty() || cvv.empty() || tipoTarjetaStr.empty() || estadoStr.empty() || pin.empty()){
            cerr << "Error: línea con campos vacíos: " << linea << endl;
            continue;
        }


        try {
            int tipoTarjeta = stoi(tipoTarjetaStr); // Convertir el tipo de tarjeta de string a int
            TipoTarjeta tipoTarjetaEnum = static_cast<TipoTarjeta>(tipoTarjeta); // Convertir el tipo de tarjeta de un int a TipoTarjeta. Si no se hace esto, el constructor no funcionará correctamente ya que solo acepta TipoTarjeta.

            int estado = stoi(estadoStr); // Convertir el estado de la tarjeta de string a int
            TipoEstado estadoEnum = static_cast<TipoEstado>(estado); // Convertir el estado de un int a TipoEstado. Si no se hace esto, el constructor no funcionará correctamente ya que solo acepta TipoEstado.

            TarjetaBancaria tarjeta(numeroTarjeta, iban, fechaCaducidad, cvv, tipoTarjetaEnum, estadoEnum, pin); // Crear el objeto TarjetaBancaria con los datos leídos de la línea
            tarjetas.push_back(tarjeta); // Agregar la tarjeta al vector
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
            continue; // Si hay un error en la línea, se muestra un mensaje de error y se continúa con la siguiente línea.
        }
        
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Tarjetas cargadas correctamente." << endl;
    return tarjetas; // Retornar el vector de tarjetas
}

void GestorArchivos::guardarUsuarios(vector<UsuarioRegistrado>& usuarios){
    ofstream archivo("usuariosRegistrados.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de usuarios para guardar." << endl;
        return;
    }

    for (UsuarioRegistrado& usuario : usuarios){
        archivo << usuario.getId() << "|"
                << usuario.getNombre() << "|"
                << usuario.getDni() << "|"
                << usuario.getContrasena() << endl; // Guardar los datos del usuario en el archivo 
    }
    archivo.close();
    cout << "Usuarios guardados correctamente." << endl;
}

void GestorArchivos::guardarCuentas(vector<CuentaBancaria>& cuentas){
    ofstream archivo("cuentasBancarias.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de cuentas para guardar." << endl;
        return;
    }

    for (CuentaBancaria& cuenta : cuentas){
        archivo << cuenta.getIBAN() << "|"
                << cuenta.getIdUsuario() << "|"
                << cuenta.getSaldo() << "|"
                << static_cast<int>(cuenta.getTipoCuenta()) << endl; // Transformarlo en un int.
    }
    archivo.close();
    cout << "Cuentas guardadas correctamente." << endl;
}

void GestorArchivos::guardarTransacciones(vector<Transaccion>& transacciones){
    ofstream archivo("transacciones.txt");

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de transacciones para guardar." << endl;
        return;
    }

    for (Transaccion& transaccion : transacciones){
        archivo << transaccion.getId() << "|"
                << transaccion.getOrdenante() << "|"
                << transaccion.getBeneficiario() << "|"
                << transaccion.getMonto() << "|"
                << transaccion.getConcepto() << "|"
                << transaccion.getFecha() << endl;
    }

    archivo.close();
    cout << "Transacciones guardadas correctamente." << endl;
}

void GestorArchivos::guardarTarjetas(vector<TarjetaBancaria>& tarjetas){
    ofstream archivo("tarjetasBancarias.txt", ios::trunc);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de tarjetas para guardar." << endl;
        return;
    }

    for (TarjetaBancaria& tarjeta : tarjetas){
        if (tarjeta.getNumeroTarjeta().empty() || tarjeta.getIBAN().empty()){
            cerr << "Error: tarjeta con campos vacíos." << endl;
            continue; // Si hay un error en la tarjeta, se muestra un mensaje de error y se continúa con la siguiente tarjeta.
        }
        
        archivo << tarjeta.getNumeroTarjeta() << "|"
                << tarjeta.getIBAN() << "|"
                << tarjeta.getFechaCaducidad() << "|"
                << tarjeta.getCVV() << "|"
                << static_cast<int>(tarjeta.getTipoTarjeta()) << "|"
                << static_cast<int>(tarjeta.getEstado()) << "|"
                << tarjeta.getPIN() << endl;
    }

    archivo.close();
    cout << "Tarjetas guardadas correctamente." << endl;
}