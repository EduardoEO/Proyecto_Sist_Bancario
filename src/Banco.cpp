#include "../include/Banco.hpp"
#include "../include/UsuarioNoRegistrado.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/CuentaBancaria.hpp"
#include "../include/Transaccion.hpp"
#include "../include/TarjetaBancaria.hpp"
#include "../include/GestorArchivos.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip> // Para std::setw() y std::setfill()
#include <unordered_map> // Para usar unordered_map en el método de carga de datos desde archivos nos evitamos la complejidad de O(n^2).

using namespace std;

// Constructor
Banco::Banco(string nombre) : nombre(nombre), usuarioAutenticado(nullptr) {} 

Banco::~Banco(){
    for (CuentaBancaria* cuenta : cuentasBancarias){
        delete cuenta;
    }
}
// Getters
string Banco::getNombre(){
    return nombre;
}

UsuarioRegistrado* Banco::getUsuarioAutenticado(){
    return usuarioAutenticado;
}

vector<Transaccion>& Banco::getTransacciones(){
    return transacciones;
}

// Metodo de carga de datos desde archivos

void Banco::cargarDatosDesdeArchivos(){
    GestorArchivos gestorArchivos; // Creamos un objeto de la clase GestorArchivos para poder cargar los datos de los archivos, es decir, acceder a los métodos de la clase GestorArchivos.

    // Carga de usuarios registrados
    vector<UsuarioRegistrado> usuariosCargados = gestorArchivos.cargarUsuarios();
    for (UsuarioRegistrado& usuario : usuariosCargados){
        usuariosRegistrados.push_back(usuario); // Agregamos los usuarios cargados al vector de usuarios registrados.
    }

    // Carga de cuentas bancarias
    vector<CuentaBancaria> cuentasCargadas = gestorArchivos.cargarCuentasBancarias();
    for (CuentaBancaria& cuenta : cuentasCargadas){
        // Creamos un objeto dinámico de tipo CuentaBancaria y lo agregamos al vector de cuentas bancarias.
        CuentaBancaria* nuevaCuenta = new CuentaBancaria(cuenta.getIBAN(), cuenta.getIdUsuario(), cuenta.getSaldo(), cuenta.getTipoCuenta(), {}, {});
        cuentasBancarias.push_back(nuevaCuenta); // Agregamos las cuentas cargadas al vector de cuentas bancarias.
    }

    // Carga de transacciones
    vector<Transaccion> transaccionesCargadas = gestorArchivos.cargarTransacciones();
    for (Transaccion& transaccion : transaccionesCargadas){
        transacciones.push_back(transaccion); // Agregamos las transacciones cargados al vector de transacciones.
    }

    // Carga de tarjetas bancarias
    vector<TarjetaBancaria> tarjetasCargadas = gestorArchivos.cargarTarjetasBancarias();
    for (TarjetaBancaria& tarjeta : tarjetasCargadas){
        tarjetasBancarias.push_back(tarjeta); // Agregamos las tarjetas cargadas al vector de tarjetas bancarias.
    }

    // Asociamos lo correspondiente a cada vector.
    asociarUsuariosConCuentas();
    asociarCuentasConTransacciones();
    asociarCuentasConTarjetas();
}

// La diferecia de los for-each de cada es según el tipo de objeto que tiene el vector. En el caso de los usuarios registrados, es un objeto de tipo UsuarioRegistrado y en el caso de las cuentas bancarias, es un puntero a un objeto de tipo CuentaBancaria. Por tanto, en el primer for-each no se necesita el operador * para acceder al objeto, pero en el segundo sí, además de la creación del objeto dinámico.

void Banco::asociarUsuariosConCuentas(){
    unordered_map<int, UsuarioRegistrado*> mapaUsuarios; // Creamos un mapa hash para asociar el idUsuario con el UsuarioRegistrado correspondiente. Usamos un int por el idUsuario y un puntero por el objeto UsuarioRegistrado.

    // Este primer for-each sirve para recorrer el vector de usuariosRegistrados y por cada usuario por el que pasa el for, se guarda su idUsuario como clave (un int) y se guarda el puntero a ese usuario como valor (un puntero al objeto UsuarioRegistrado, UsuarioRegistrado*).
    for (UsuarioRegistrado& usuario : usuariosRegistrados){
        mapaUsuarios[usuario.getId()] = &usuario;
    }

    // Este segundo for-each, recorre el vector cuentasBancarias y para cada cuenta bancaria se obtiene el idUsuario propietario de esa cuenta. Luego, se busca ese idUsuario en el mapaUsuarios y si existe (es decir, hay un usuario con ese ID) pues se llama al método agregarCuentaBancaria() del usuario, pasándole la cuenta. Así cada usuario va acumulando sus propias cuentas bancarias en su estructura interna (su vector en su clase).
    for (CuentaBancaria* cuenta : cuentasBancarias){
        if(mapaUsuarios.find(cuenta->getIdUsuario()) != mapaUsuarios.end()){
            mapaUsuarios[cuenta->getIdUsuario()]->agregarCuentaBancaria(*cuenta);
        }
    }

    cout << "Cuentas bancarias asociadas a los usuarios registrados correctamente." << endl;
}

void Banco::asociarCuentasConTransacciones(){
    unordered_map<string, CuentaBancaria*> mapaCuentas; // En esta caso distinto al anterior usamos un string ya que el IBAN es un string.
    for (CuentaBancaria* cuenta : cuentasBancarias){
        mapaCuentas[cuenta->getIBAN()] = cuenta;
    }

    for (Transaccion& transaccion : transacciones){
        if (mapaCuentas.find(transaccion.getOrdenante()) != mapaCuentas.end()){
            mapaCuentas[transaccion.getOrdenante()]->agregarTransaccion(transaccion);
        }
        if (mapaCuentas.find(transaccion.getBeneficiario()) != mapaCuentas.end()){
            mapaCuentas[transaccion.getBeneficiario()]->agregarTransaccion(transaccion);
        }
    }
    cout << "Transacciones asociadas a las cuentas bancarias correctamente." << endl;
}

void Banco::asociarCuentasConTarjetas(){
    unordered_map<string, CuentaBancaria*> mapaCuentas;
    for (CuentaBancaria* cuenta : cuentasBancarias){
        mapaCuentas[cuenta->getIBAN()] = cuenta;
    }

    for (TarjetaBancaria& tarjeta : tarjetasBancarias){
        if (mapaCuentas.find(tarjeta.getIBAN()) != mapaCuentas.end()){
            mapaCuentas[tarjeta.getIBAN()]->agregarTarjeta(tarjeta);
        } else {
            cerr << "Error: Tarjeta con IBAN no encontrado: " << tarjeta.getIBAN() << endl; // Si la tarjeta no tiene una cuenta asociada, se muestra un mensaje de error.
        }
    }

    cout << "Tarjetas asociadas a las cuentas bancarias correctamente." << endl;
}

// Guardar datos en los archivos

void Banco::guardarDatos(){
    GestorArchivos gestorArchivos;

    gestorArchivos.guardarUsuarios(usuariosRegistrados);

    vector<CuentaBancaria> cuentas;
    // Al ser un vector de punteros, tendremos que "traducir" los punteros a objetos de tipo CuentaBancaria para poder guardarlos en el archivo. Por eso usamos el operador * para acceder al objeto al que apunta el puntero. Luego, lo copia y finalmente lo guarda.
    for (CuentaBancaria* cuenta : cuentasBancarias) {
        if (cuenta == nullptr) {
            cout << "Error: cuenta nula" << endl;
            continue; // Si la cuenta es nula, pasamos a la siguiente iteración del bucle.
        }
        cuentas.push_back(*cuenta); // Guardamos el objeto de tipo CuentaBancaria en el vector de cuentas.
    }

    gestorArchivos.guardarCuentas(cuentas); // Guardamos el vector de cuentas en el archivo.

    gestorArchivos.guardarTransacciones(transacciones);

    gestorArchivos.guardarTarjetas(tarjetasBancarias);

    cout << "Datos guardados correctamente." << endl;
}

// Métodos
UsuarioRegistrado* Banco::iniciarSesion(const string& dni, const string& contrasena){ // const evita modificar los valores y & pasamos la referencia para evitar duplicar el objeto.
    for (UsuarioRegistrado& usuario : usuariosRegistrados){
        if (usuario.getDni() == dni && usuario.getContrasena() == contrasena){
            usuarioAutenticado = &usuario; // Guardamos el usuario autenticado en el atributo de la clase.
            return &usuario; // Devolvemos la referencia al usuario registrado que ha iniciado sesión en vez de una copia.
        }
    }
    usuarioAutenticado = nullptr; // Si no encontramos el usuario, el usuario autenticado es nulo.
    return nullptr; // Si no encontramos el usuario, devolvemos un puntero nulo para evitar errores y duplicaciones.
}

void Banco::registrarUsuario(UsuarioNoRegistrado& usuarioNoRegistrado){
    // Verificar primero si el DNI ya está registrado.
    for (UsuarioRegistrado& usuario : usuariosRegistrados){
        if (usuario.getDni() == usuarioNoRegistrado.getDni()){
            cout << "Error: Usuario ya registrado con este DNI: " << usuarioNoRegistrado.getDni() << endl; 
            return;
        }
    }
    // Si no está el DNI registrado, ya podemos registrar al usuario.
    usuariosRegistrados.emplace_back(usuariosRegistrados.size() + 1, usuarioNoRegistrado.getNombre(), usuarioNoRegistrado.getDni(), usuarioNoRegistrado.getContrasena());
    cout << "Usuario registrado con éxito" << endl;

}

UsuarioRegistrado* Banco::buscarUsuario(int id){ // Devuelve un puntero a un usuario registrado en vez de una copia del objeto.
    for (UsuarioRegistrado& usuario : usuariosRegistrados){
        if (usuario.getId() == id){
            return &usuario;
        }
    }
    cout << "Usuario no encontrado" << endl;
    return nullptr; // Usuario no encontrado y devolvemos un puntero nulo para evitar errores y duplicaciones.
}

string Banco::mostrarCuentasBancariasUsuario(UsuarioRegistrado* usuarioRegistrado){
    string resultado;
    for (CuentaBancaria* cuenta : usuarioRegistrado->getCuentasBancarias()){
        resultado += cuenta->getIBAN() + " ";
    }
    return resultado;
}

void Banco::crearCuentaBancaria(UsuarioRegistrado* usuario){
    if (!usuario){
        cout << "Error: Usuario no autenticado" << endl;
        return;
    }
    // Generamos un IBAN aleatorio de 24 caracteres pero con el código español.
    string IBAN;
    do{
        IBAN = "ES";
        for (int i = 0; i < 22; i++){
            IBAN += to_string(rand() % 10); // Añadimos un número aleatorio entre 0 y 9 al IBAN. El to_string convierte el número en un string.
        }
    }while (ibanExistente(IBAN)); // Comprobamos que el IBAN no exista en el vector de cuentas bancarias.
    
    double saldo = 0.0;
    TipoCuenta tipoCuenta;

    cout << "Introduce el tipo de cuenta (1. Ahorro, 2. Corriente, 3. Nomina, 4. Inversión, 5. Empresa): ";
    int opcion;
    cin >> opcion;

    switch(opcion){
        case 1:
            tipoCuenta = AHORRO;
            break;
        case 2:
            tipoCuenta = CORRIENTE;
            break;
        case 3:
            tipoCuenta = NOMINA;
            break;
        case 4:
            tipoCuenta = INVERSION;
            break;
        case 5:
            tipoCuenta = EMPRESA;
            break;
        default:
            cout << "Opción no válida" << endl;
            return;
    }

    CuentaBancaria* nuevaCuentaBancaria = new CuentaBancaria(IBAN, usuario->getId(), saldo, tipoCuenta, {}, {});
    usuario->agregarCuentaBancaria(*nuevaCuentaBancaria);
    cuentasBancarias.push_back(nuevaCuentaBancaria);
    
    cout << "Cuenta bancaria creada con éxito" << endl;
    cout << "IBAN: " << IBAN << endl;
}

bool Banco::ibanExistente(const string& IBAN){
    for (CuentaBancaria* cuenta : cuentasBancarias){
        if (cuenta->getIBAN() == IBAN){
            return true;
        }
    }
    return false;
}

string Banco::mostrarTarjetasBancarias(CuentaBancaria* cuentaBancaria){
    if (!cuentaBancaria){
        return "Error: Cuenta bancaria no válida.";
    }

    string resultado;
    for (TarjetaBancaria& tarjeta : cuentaBancaria->getTarjetasBancarias()){
        resultado += tarjeta.getNumeroTarjeta() + " ";
    }
    return resultado;
}

void Banco::retirar(CuentaBancaria* cuentaBancaria, double monto){
    if (monto > cuentaBancaria->getSaldo()){
        cout << "Error: Saldo insuficiente." << endl;
        return;
    }
    cuentaBancaria->retirar(monto);

    time_t now = time(0);
    string fecha = ctime(&now);
    Transaccion transaccion(transacciones.size() + 1, cuentaBancaria->getIBAN(), getNombre(), monto, "Retiro de efectivo", fecha);
    transacciones.push_back(transaccion); // Guardar la transacción en el historial de transacciones del banco.
    cuentaBancaria->agregarTransaccion(transaccion); // Guardar la transacción en el historial de la cuenta bancaria.
    
    cout << "Retiro realizado con éxito." << endl;
}

void Banco::depositar(CuentaBancaria* cuentaBancaria, double monto){
    if (monto <= 0){
        cout << "Error: Monto inválido." << endl;
        return;
    }
    cuentaBancaria->depositar(monto);

    time_t now = time(0);
    string fecha = ctime(&now);
    Transaccion transaccion(transacciones.size() + 1, getNombre(), cuentaBancaria->getIBAN(), monto, "Ingreso de efectivo", fecha);
    transacciones.push_back(transaccion);
    cuentaBancaria->agregarTransaccion(transaccion);

    cout << "Ingreso realizado con éxito." << endl;
}

void Banco::transferencia(const string& ibanOrigen, const string& ibanDestino, double monto){
    CuentaBancaria* cuentaOrigen = nullptr;
    CuentaBancaria* cuentaDestino = nullptr;

    for (CuentaBancaria* cuenta : cuentasBancarias){
        if (cuenta->getIBAN() == ibanOrigen){
            cuentaOrigen = cuenta;
        } else if (cuenta->getIBAN() == ibanDestino){
            cuentaDestino = cuenta;
        }
        if (cuentaOrigen && cuentaDestino){
            break;
        }
    }

    if (!cuentaOrigen || !cuentaDestino){
        cout << "Error: Cuenta bancaria no encontrada." << endl;
        return;
    }
    if (cuentaOrigen->getSaldo() < monto){
        cout << "Error: Saldo insuficiente." << endl;
        return;
    }

    string concepto;
    cout << "Introduce el concepto de la transferencia: ";
    cin.ignore(); // Limpiar el buffer de entrada.
    getline(cin, concepto); // Leer el concepto de la transferencia.

    cuentaOrigen->retirar(monto);
    cuentaDestino->depositar(monto);

    time_t now = time(0);
    string fecha = ctime(&now);

    Transaccion transaccion(transacciones.size() + 1, cuentaOrigen->getIBAN(), cuentaDestino->getIBAN(), monto, concepto, fecha);
    transacciones.push_back(transaccion); // Guardar la transacción en el historial de transacciones del banco.
    cuentaOrigen->agregarTransaccion(transaccion); // Guardar en el historial de la cuenta origen también.
    cuentaDestino->agregarTransaccion(transaccion); // Guardar en el historial de la cuenta destino también.



    cout << "Transferencia entre usuarios realizada con éxito." << endl;
}

void Banco::crearTarjetaBancaria(CuentaBancaria* cuentaBancaria) {
    if (!cuentaBancaria) {
        cout << "Error: Cuenta bancaria no válida." << endl;
        return;
    }

    // Generar un número de tarjeta único de 16 dígitos
    string numeroTarjeta;
    do {
        numeroTarjeta = "";
        for (int i = 0; i < 16; i++) {
            numeroTarjeta += to_string(rand() % 10); // Generar un dígito aleatorio
        }
    } while (numeroTarjetaExistente(numeroTarjeta)); // Asegurarse de que el número de tarjeta sea único

    // Generar un CVV de 3 dígitos
    string CVV = "";
    for (int i = 0; i < 3; i++) {
        CVV += to_string(rand() % 10);
    }

    // Generar la fecha de caducidad (5 años desde la fecha actual)
    time_t now = time(0);
    tm* fechaActual = localtime(&now);
    fechaActual->tm_year += 5; // Añadir 5 años
    ostringstream oss;
    oss << setw(2) << setfill('0') << (fechaActual->tm_mon + 1) << "/" << setw(2) << setfill('0') << (fechaActual->tm_year % 100);
    string fechaCaducidad = oss.str();

    // Solicitar el PIN de la tarjeta
    string PIN;
    cout << "Introduce el PIN de la tarjeta (6 dígitos): ";
    cin >> PIN;

    if (PIN.size() != 6 || !all_of(PIN.begin(), PIN.end(), ::isdigit)) {
        cout << "Error: El PIN debe tener exactamente 6 dígitos." << endl;
        return;
    }

    // Seleccionar el tipo de tarjeta
    TipoTarjeta tipoTarjeta;
    cout << "Seleccione el tipo de tarjeta (1. Crédito, 2. Débito): ";
    int opcion;
    cin >> opcion;

    switch (opcion) {
        case 1:
            tipoTarjeta = CREDITO;
            break;
        case 2:
            tipoTarjeta = DEBITO;
            break;
        default:
            cout << "Opción no válida. Operación cancelada." << endl;
            return;
    }

    // Crear la tarjeta bancaria
    TarjetaBancaria nuevaTarjeta(numeroTarjeta, cuentaBancaria->getIBAN(), fechaCaducidad, CVV, tipoTarjeta, ACTIVA, PIN);

    // Asociar la tarjeta a la cuenta bancaria
    cuentaBancaria->agregarTarjeta(nuevaTarjeta);

    // Asociar la tarjeta al banco
    tarjetasBancarias.push_back(nuevaTarjeta);

    cout << "Tarjeta creada con éxito." << endl;
    cout << "Número de tarjeta: " << numeroTarjeta << endl;
    cout << "Fecha de caducidad: " << fechaCaducidad << endl;
    cout << "CVV: " << CVV << endl;
    cout << "Tipo de tarjeta: " << (tipoTarjeta == CREDITO ? "Crédito" : "Débito") << endl;
}

bool Banco::numeroTarjetaExistente(const string& numeroTarjeta){
    for (TarjetaBancaria& tarjeta : tarjetasBancarias){
        if (tarjeta.getNumeroTarjeta() == numeroTarjeta){
            return true;
        }
    }
    return false;
}

void Banco::eliminarCuenta(CuentaBancaria* cuenta){
    auto it = find(cuentasBancarias.begin(), cuentasBancarias.end(), cuenta);
    if (it != cuentasBancarias.end()) {
        delete *it; // Liberar la memoria de la cuenta
        cuentasBancarias.erase(it); // Eliminar la cuenta del vector
    }
}

void Banco::eliminarUsuario(UsuarioRegistrado* usuario){
    if (!usuario) {
        cout << "Error: Usuario no válido." << endl;
        return;
    }

    // Un usuario tendrá cuentas bancarias asociadas, por lo tanto hace falta eliminar las cuentas bancarias además de el usuario.
    auto itCuenta = cuentasBancarias.begin();
    while (itCuenta != cuentasBancarias.end()){
        if ((*itCuenta)->getIdUsuario() == usuario->getId()){
            string ibanCuenta = (*itCuenta)->getIBAN();

            auto itTarjeta = tarjetasBancarias.begin();
            while (itTarjeta != tarjetasBancarias.end()){
                if (itTarjeta->getIBAN() == ibanCuenta){
                    itTarjeta = tarjetasBancarias.erase(itTarjeta); // Borramos la tarjeta de la lista de tarjetas del banco.
                } else {
                    ++itTarjeta;
                }
            }

            delete *itCuenta;
            itCuenta = cuentasBancarias.erase(itCuenta);
        } else {
            ++itCuenta;
        }
    }

    auto itUsuario = usuariosRegistrados.begin();
    while (itUsuario != usuariosRegistrados.end()){
        if (itUsuario->getId() == usuario->getId()){
            itUsuario = usuariosRegistrados.erase(itUsuario);
            break;
        } else {
            ++itUsuario;
        }
    }
    GestorArchivos gestorArchivos;
    gestorArchivos.guardarUsuarios(usuariosRegistrados); // Guardar los usuarios restantes en el archivo.
    gestorArchivos.guardarTarjetas(tarjetasBancarias); // Guardar las tarjetas restantes en el archivo.
    
    cout << "Usuario eliminado con éxito." << endl;
}