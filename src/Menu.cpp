#include "../include/Menu.hpp"
#include "../include/Banco.hpp"
#include "../include/UsuarioRegistrado.hpp"
#include "../include/CuentaBancaria.hpp"
#include "../include/Transaccion.hpp"
#include "../include/UsuarioNoRegistrado.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Para std::find_if
#include <ctime> // Para time_t y ctime
#include <sstream> // Para std::stringstream
#include <fstream> 
#include <filesystem>
#include <iomanip> // Para std::setw() y std::setfill()

using namespace std;

void Menu::mostrarMenuIniSesion(Banco& banco){
    cout << "------- " << banco.getNombre() << " -------" << endl;
    bool continuar = true;
    while(continuar){
        cout << "1. Iniciar sesión" << endl;
        cout << "2. Registrarse" << endl;
        cout << "3. Salir" << endl;
        cout << "Ingrese una opción: ";

        int opcion;
        cin >> opcion;

        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opción inválida. Por favor, ingrese un número." << endl;
            continue; // Volver a mostrar el menú si la entrada no es válida.
        }

        switch (opcion){
        case 1:
            iniciarSesion(banco); // Iniciar sesión de un usuario registrado.
            break;
        case 2:
            mostrarMenuRegistro(banco); // Menú de registro de un nuevo usuario.
            break;
        case 3: 
            continuar = false;
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
        }
    }
}

void Menu::iniciarSesion(Banco& banco){
    cout << "Ingrese su DNI: ";
    string dni;
    cin >> dni;
    cout << "Ingrese su contraseña: ";
    string contrasena;
    cin >> contrasena;
    
    UsuarioRegistrado* usuario = banco.iniciarSesion(dni, contrasena);
    if (usuario){
        cout << "Entrando..." << endl;
        cout << "Inicio de sesión exitoso. Bienvendio " << usuario->getNombre() << "." << endl;
        mostrarMenuUsuario(banco); // Menú de Usuario del usuario registrado.
    } else {
        cout << "Credenciales incorrectas." << endl;
    }
}

void Menu::mostrarMenuRegistro(Banco& banco){
    cout << "Ingrese su nombre completo: ";
    string nombre;
    cin.ignore(); // Limpiar el buffer de entrada.
    getline(cin, nombre); // Leer el nombre completo del usuario.
    cout << "Ingrese su DNI: ";
    string dni;
    cin >> dni;
    cout << "Ingrese su contraseña: ";
    string contrasena;
    cin >> contrasena;

    UsuarioNoRegistrado usuarioNoRegistrado(nombre, dni, contrasena);
    banco.registrarUsuario(usuarioNoRegistrado);
}

void Menu::mostrarMenuUsuario(Banco& banco){
    UsuarioRegistrado* usuario = banco.getUsuarioAutenticado(); // Obtenemos el usuario autenticado desde banco almacenado en la variable "auxiliar".
    if (!usuario){
        cout << "Error: No hay un usuario autenticado" << endl;
        return;
    }
    
    bool continuar = true;
    while (continuar){
        cout << "------- " << banco.getNombre() << " -------" << endl;
        cout << "Usuario: " << usuario->getNombre() << endl;
        cout << "Cuentas bancarias de su usuario: " << banco.mostrarCuentasBancariasUsuario(usuario) << endl;
        cout << "-----------------------------------------" << endl;
        cout << "1. Ver información de la cuenta" << endl;
        cout << "2. Cambiar la contraseña" << endl;
        cout << "3. Crear cuenta bancaria" << endl;
        cout << "4. Eliminar cuenta" << endl;
        cout << "5. Seleccionar cuenta bancaria" << endl;
        cout << "6. Cerrar sesión" << endl;
        cout << "Ingrese una opción: ";
        int opcion;
        cin >> opcion;

        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opción inválida. Por favor, ingrese un número." << endl;
            continue; // Volver a mostrar el menú si la entrada no es válida.
        }

        switch (opcion){
            case 1:
                mostrarInformacionUsuario(usuario);
                break;
            case 2:
                cambiarContrasena(usuario);
                break;
            case 3:
                banco.crearCuentaBancaria(usuario);
                break;
            case 4:
                cout << "¿Está seguro de eliminar su cuenta? (S/N): ";
                char respuesta;
                cin >> respuesta;
                if (respuesta == 'S' || respuesta == 's'){
                    cout << "Ingrese su contraseña: ";
                    string contrasena;
                    cin >> contrasena;
                    if (contrasena == usuario->getContrasena()){
                        banco.eliminarUsuario(usuario);
                        continuar = false;
                        break;
                    }
                }
                break;
            case 5:
                seleccionarCuentaBancaria(banco, usuario);
                break;
            case 6:
                continuar = false;
                break;
            default:
                cout << "Opción inválida" << endl;
                break;
        }
    }
    
}

void Menu::mostrarInformacionUsuario(UsuarioRegistrado* usuario){
    cout << "Información del usuario: " << endl;
    usuario->mostrarInformacion();
}

void Menu::cambiarContrasena(UsuarioRegistrado* usuario){
    cout << "Contraseña actual: " << usuario->getContrasena() << endl;
    cout << "Introduce la nueva contraseña: ";
    string nuevaContrasena;
    cin >> nuevaContrasena;
    cout << "Introduce la nueva contraseña de nuevo: ";
    string comparadorContrasena;
    cin >> comparadorContrasena;
    if (nuevaContrasena == comparadorContrasena){
        usuario->setContrasena(nuevaContrasena);
        cout << "Contraseña cambiada con éxito." << endl;
    }else{
        cout << "Las contraseñas no coinciden." << endl;
    }
}

void Menu::seleccionarCuentaBancaria(Banco& banco, UsuarioRegistrado* usuario){
    cout << "Seleccione una de sus cuentas bancarias: " << endl;
    vector<CuentaBancaria*>& cuentas = usuario->getCuentasBancarias();
    for (int i = 0; i < cuentas.size(); i++){
        cout << i + 1 << ". " << cuentas[i]->getIBAN() << endl;
    }
    cout << "Ingrese una opción: ";
    int opcion;
    cin >> opcion;
    if (opcion >= 1 && opcion <= cuentas.size()){
        cout << "Cuenta seleccionada: " << cuentas[opcion - 1]->getIBAN() << endl;
        mostrarMenuPrincipal(banco, cuentas[opcion -1]);
    }else{
        cout << "Opción inválida." << endl;
    }
}

void Menu::mostrarMenuPrincipal(Banco& banco, CuentaBancaria* cuentaBancaria){
    bool continuar = true;
    while (continuar){
        cout << "------- " << banco.getNombre() << " -------" << endl;
        cout << "Cuenta bancaria actual: " << cuentaBancaria->getIBAN() << endl; // Mostrar el IBAN de dicha cuenta bancaria y todos los datos relacionados importantes.
        cout << "Saldo actual: " << cuentaBancaria->getSaldo() << endl;
        cout << "Tipo de cuenta: " << cuentaBancaria->getTipoCuentaStr() << endl;
        cout << "Tarjetas bancarias de su cuenta: " << banco.mostrarTarjetasBancarias(cuentaBancaria) << endl;
        cout << "-----------------------------------------" << endl;
        cout << "1. Retirar dinero" << endl;
        cout << "2. Ingresar dinero" << endl;
        cout << "3. Realizar transacción" << endl;
        cout << "4. Ver historial de transacciones" << endl;
        cout << "5. Solicitar tarjeta" << endl; // Crear un objeto de tipo TarjetaBancaria que según el tipoCuenta podrá o no. 
        cout << "6. Seleccionar tarjeta" << endl; // Seleccionar una tarjeta bancaria de la cuenta bancaria seleccionada y gestionar dicha tarjeta.
        cout << "7. Fusionar cuentas" << endl; // Fusionar cuentas bancarias de un usuario registrado. Un usuario registrado puede tener varias cuentas bancarias.
        cout << "8. Cerrar sesión" << endl;
        cout << "Ingrese una opción: ";
        int opcion = 0;
        cin >> opcion;

        if (cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opción inválida. Por favor, ingrese un número." << endl;
            continue; // Volver a mostrar el menú si la entrada no es válida.
        }

        switch (opcion){
            case 1: {
                double monto;
                cout << "Ingrese el monto a retirar: ";
                cin >> monto;
                banco.retirar(cuentaBancaria, monto);
                break;
            }
            case 2: {
                double monto;
                cout << "Ingrese el monto a ingresar: ";
                cin >> monto;
                banco.depositar(cuentaBancaria, monto);
                break;
            }
            case 3: {
                string ibanDestino;
                double monto;
                cout << "Ingrese el IBAN de la cuenta destino: ";
                cin >> ibanDestino;
                cout << "Ingrese el monto a transferir: ";
                cin >> monto;
                banco.transferencia(cuentaBancaria->getIBAN(), ibanDestino, monto);
                break;
            }
            case 4: 
                mostrarHistorialTransacciones(banco, cuentaBancaria);
                break;
            case 5:
                // mostrarMenuSolicitarTarjetaBancaria(banco, cuentaBancaria);
                banco.crearTarjetaBancaria(cuentaBancaria);
                break;
            case 6:
                seleccionarTarjetaBancaria(banco, cuentaBancaria);
                break;
            case 7:
                fusionarCuentasBancarias(banco, cuentaBancaria);
                break;
            case 8:
                continuar = false;
                break;
            default:
                cout << "Opción inválida." << endl;
                break;
        }
    }
}

void Menu::mostrarHistorialTransacciones(Banco& banco, CuentaBancaria* cuentaBancaria){
    cout << "Historial de transacciones: " << endl;
    vector<Transaccion>& transacciones = cuentaBancaria->getTransacciones();   
    if (transacciones.empty()){
        cout << "No hay transacciones registradas." << endl;
        return;
    }
    for (Transaccion& transaccion : transacciones){
        transaccion.mostrarDetalles();
        cout << "-----------------------------------------" << endl;
    }
    cout << "¿Desea tener una copia de su historial de transacciones? (S/N): ";
    char respuesta;
    cin >> respuesta;
    if (respuesta == 'S' || respuesta == 's'){
        cout << "Guardando historial de transacciones..." << endl;
        // Queremos obtener la ruta para guardar el archivo de texto en descargas (Usuarios de Windows)
        string rutaHome = getenv("USERPROFILE"); // Obtener la ruta del usuario actual.
        string rutaDescargas = rutaHome + "\\Downloads\\historialDeTransacciones.txt"; // Ruta de la carpeta de descargas.

        // Creamos el archivo de texto en la ruta especificada.
        ofstream archivoDeTexto(rutaDescargas);
        if (!archivoDeTexto.is_open()){
            cout << "Error al crear el archivo de texto." << endl;
            return;
        }

        archivoDeTexto << "Historial de transacciones: " << endl;
        for(const Transaccion& transaccion : transacciones){
            archivoDeTexto << transaccion.mostrarDetallesStr() << endl; // Guardamos el historial de transacciones en el archivo de texto.            
            }
        archivoDeTexto.close(); // Cerramos el archivo de texto.
        cout << "Historial de transacciones guardado en: " << rutaDescargas << endl;
    }else{
        cout << "Operación cancelada." << endl;
    }
}

void Menu::fusionarCuentasBancarias(Banco& banco, CuentaBancaria* cuentaBancariaActual){
    UsuarioRegistrado* usuario = banco.getUsuarioAutenticado();
    vector<CuentaBancaria*>& cuentas = usuario->getCuentasBancarias();

    // Verificar que haya cuentas bancarias disponibles para fusionar.
    vector<CuentaBancaria*> cuentasParaFusionar; // Para inicializar un vector de punteros, es necesario crear un vector vacío y después añadir los punteros a dicho vector.
    vector<CuentaBancaria*>& cuentasFusionables = cuentasParaFusionar;

    for (CuentaBancaria* cuenta : cuentas){
        if (cuenta != cuentaBancariaActual){
            cuentasFusionables.push_back(cuenta);
        }
    }

    if (cuentasFusionables.empty()){
        cout << "No hay cuentas bancarias disponibles para fusionar." << endl;
        return;
    }
    
    cout << "Cuenta bancaria actual: " << cuentaBancariaActual->getIBAN() << "(Saldo: " << cuentaBancariaActual->getSaldo() << ")" << endl;
    
    // Mostrar las cuentas bancarias disponibles para fusionar.
    
    cout << "Seleccione la cuenta bancaria que desea fusionar con la cuenta actual: " << endl;
    for (int i = 0; i < cuentas.size(); i++){
        if (cuentas[i] != cuentaBancariaActual){
            cout << i + 1 << ". " << cuentas[i]->getIBAN() << "(Saldo: " << cuentas[i]->getSaldo() << ")" << endl;
            
        }
    }

    cout << "Ingrese una opción: ";
    int opcion;
    cin >> opcion;

    if (cin.fail() ||opcion < 1 || opcion > cuentas.size()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opción inválida. Por favor, ingrese un número." << endl;
        return; // Volver a mostrar el menú si la entrada no es válida.
    }

    if (opcion >= 1 && opcion <= cuentas.size()){ 
        CuentaBancaria* cuentaSeleccionada = cuentas[opcion - 1];
        *cuentaBancariaActual += *cuentaSeleccionada;
        banco.eliminarCuenta(cuentaSeleccionada);
        cuentas.erase(cuentas.begin() + (opcion - 1)); // Borramos el puntero de la cuenta seleccionada del vector de cuentas del usuario. Por tanto, es como si no existiera porque no es alcanzable.


        cout << "Cuentas fusionadas con éxito." << endl;
        cout << "Saldo total: " << cuentaBancariaActual->getSaldo() << endl;
    }else{
        cout << "Opción inválida." << endl;
    }

}

void Menu::seleccionarTarjetaBancaria(Banco& banco, CuentaBancaria* cuentaBancaria){
    cout << "Seleccione una de sus tarjetas bancarias: " << endl;
    vector<TarjetaBancaria>& tarjetas = cuentaBancaria->getTarjetasBancarias();

    for (int i = 0; i < tarjetas.size(); i++){
        cout << i + 1 << ". " << tarjetas[i].getNumeroTarjeta() << endl;
    }

    cout << "Ingrese una opción: ";
    int opcion;
    cin >> opcion;

    if (cin.fail() || opcion < 1 || opcion > tarjetas.size()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Opción inválida. Por favor, ingrese un número." << endl;
        return; // Volver a mostrar el menú si la entrada no es válida.
    }

    if (opcion >= 1 && opcion <= tarjetas.size()){
        TarjetaBancaria& tarjetaSeleccionada = tarjetas[opcion - 1];
        cout << "Tarjeta seleccionada: " << tarjetaSeleccionada.getNumeroTarjeta() << endl;
        cout << "Ingrese el PIN de la tarjeta seleccionada: ";
        string PINIngresado;
        cin >> PINIngresado;

        if (tarjetaSeleccionada.getPIN() == PINIngresado){
            cout << "PIN correcto. Accediendo..." << endl;
            mostrarMenuTarjetaBancaria(banco, tarjetaSeleccionada);
        } else {
            cout << "PIN incorrecto. No se puede acceder a la tarjeta." << endl;
            return;
        }
    }else{
        cout << "Opción inválida." << endl;
    }
}

void Menu::mostrarMenuTarjetaBancaria(Banco& banco, TarjetaBancaria& tarjetaBancaria){
    time_t now = time(0);
    tm* fechaActual = localtime(&now);
    int mesActual = fechaActual->tm_mon + 1; // Mes actual (1-12)
    int anioActual = (fechaActual->tm_year + 1900) % 100; // Año actual (AA)

    // Extraer mes y año de caducidad de la tarjeta. Teniendo en cuenta que tiene un formato MM AA.
    int mesCaducidad, anioCaducidad;
    char separador;
    istringstream iss(tarjetaBancaria.getFechaCaducidad());
    iss >> mesCaducidad >> separador >> anioCaducidad;

    // Comparar la fecha de caducidad con la fecha actual.
    if (anioCaducidad < anioActual || (anioCaducidad == anioActual && mesCaducidad < mesActual)){
        tarjetaBancaria.setEstado(CADUCADA); // Cambiar el estado de la tarjeta a CADUCADA.
        cout << "La tarjeta ha caducado." << endl;
    }
    bool continuar = true;
    while (continuar){
        cout << "------- " << banco.getNombre() << " -------" << endl;
        cout << "Tarjeta bancaria actual: " << tarjetaBancaria.getNumeroTarjeta() << endl;
        cout << "CVV: " << tarjetaBancaria.getCVV() << endl;
        cout << "Fecha de caducidad: " << tarjetaBancaria.getFechaCaducidad() << endl;
        cout << "Estado: " << tarjetaBancaria.getEstadoStr(tarjetaBancaria.getEstado()) << endl;
        cout << "-----------------------------------------" << endl;
        if (tarjetaBancaria.getEstado() == CADUCADA){
            cout << "1. Renovar tarjeta" << endl;
            cout << "2. Cerrar sesión" << endl;
            cout << "Ingrese una opción: ";
            int opcion;
            cin >> opcion;

            if (cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opción inválida. Por favor, ingrese un número." << endl;
                continue; // Volver a mostrar el menú si la entrada no es válida.
            }
            
            switch (opcion){
                case 1: 
                    renovarTarjetaBancaria(banco, tarjetaBancaria);
                    break;
                case 2:
                    continuar = false;
                    break;
                default:
                    cout << "Opción inválida." << endl;
                    break;
            }

        }else{
            cout << "1. Activar tarjeta" << endl;
            cout << "2. Bloquear tarjeta" << endl;
            cout << "3. Cambiar PIN" << endl;
            cout << "4. Mostrar detalles" << endl;
            cout << "5. Cerrar sesión" << endl;
            cout << "Ingrese una opción: ";
            int opcion;
            cin >> opcion;

            if (cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Opción inválida. Por favor, ingrese un número." << endl;
                continue; // Volver a mostrar el menú si la entrada no es válida.
            }

            switch (opcion){
                case 1:
                    tarjetaBancaria.setEstado(ACTIVA);
                    cout << "Tarjeta activada." << endl;
                    break;
                case 2:
                    tarjetaBancaria.setEstado(BLOQUEADA);
                    cout << "Tarjeta bloqueada." << endl;
                    break;
                case 3:
                    cambiarPIN(tarjetaBancaria);
                    break;
                case 4:
                    tarjetaBancaria.mostrarDetalles();
                    break;
                case 5:
                    continuar = false;
                    break;
                default:
                    cout << "Opción inválida." << endl;
                    break;
            }
        }
    }
}

void Menu::cambiarPIN(TarjetaBancaria& tarjetaBancaria){
    cout << "PIN actual: " << tarjetaBancaria.getPIN() << endl;
    cout << "Introduce el nuevo PIN: ";
    string nuevoPIN;
    cin >> nuevoPIN;
    cout << "Introduce el nuevo PIN de nuevo: ";
    string comparadorPIN;
    cin >> comparadorPIN;
    if (nuevoPIN == comparadorPIN){
        tarjetaBancaria.setPIN(nuevoPIN);
        cout << "PIN cambiado con éxito." << endl;
    }else{
        cout << "Los PIN no coinciden." << endl;
    }
}

void Menu::renovarTarjetaBancaria(Banco& banco, TarjetaBancaria& tarjetaBancaria){
    time_t now = time(0);
    tm* fechaActual = localtime(&now);
    fechaActual->tm_year += 5; // Añadimos 5 años a la fecha actual.

    ostringstream oss;
    oss << setw(2) << setfill('0') << (fechaActual->tm_mon + 1) << "/" << setw(2) << setfill('0') << (fechaActual->tm_year % 100); // Le sumas un mes al mes actual y le sumas 5 años al año actual. Ejemplo: 02/23 -> (Hoy es: 04/25) -> 04/30
    // Formato MM/AA. El %100 es para obtener solo los dos últimos dígitos del año. El setw(2) y setfill('0') son para que el mes y el año tengan siempre dos dígitos.
    string nuevaFechaCaducidad = oss.str();

    string nuevoCVV = "";
    for (int i = 0; i < 3; i++){
        nuevoCVV += to_string(rand() % 10);
    }

    tarjetaBancaria.setFechaCaducidad(nuevaFechaCaducidad);
    tarjetaBancaria.setCVV(nuevoCVV);
    tarjetaBancaria.setEstado(ACTIVA);

    cout << "La tarjeta ha sido renovada con éxito." << endl;
    cout << "Nueva fecha de caducidad: " << tarjetaBancaria.getFechaCaducidad() << endl;
    cout << "Nuevo CVV: " << tarjetaBancaria.getCVV() << endl;
    cout << "Estado: " << tarjetaBancaria.getEstadoStr(tarjetaBancaria.getEstado()) << endl;
}

