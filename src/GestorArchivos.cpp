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
#include <filesystem>

using namespace std;

// Configuración de rutas para la base de datos (carpeta data/)
static const string CARPETA_DATOS = "data/";
static const string ARCHIVO_USUARIOS = CARPETA_DATOS + "usuariosRegistrados.txt";
static const string ARCHIVO_CUENTAS = CARPETA_DATOS + "cuentasBancarias.txt";
static const string ARCHIVO_TRANSACCIONES = CARPETA_DATOS + "transacciones.txt";
static const string ARCHIVO_TARJETAS = CARPETA_DATOS + "tarjetasBancarias.txt";

// Obtener ruta con fallback por si existiese archivo antiguo en la raíz
static string obtenerRutaArchivo(const string& rutaPrincipal, const string& nombreRaiz) {
    if (filesystem::exists(rutaPrincipal)) {
        return rutaPrincipal;
    }
    if (filesystem::exists(nombreRaiz)) {
        return nombreRaiz;
    }
    return rutaPrincipal;
}

vector<UsuarioRegistrado>GestorArchivos::cargarUsuarios(){
    vector<UsuarioRegistrado> usuarios;
    string ruta = obtenerRutaArchivo(ARCHIVO_USUARIOS, "usuariosRegistrados.txt");
    ifstream archivo(ruta);
    
    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de usuarios: " << ruta << endl; // cerr sirve para mostrar errores
        return usuarios; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    bool requiereGuardar = false;
    string linea;
    while (getline(archivo, linea)){ // Con esto lees la línea completa del archivo
        while (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')){
            linea.pop_back(); // Eliminar retornos de carro de Windows CRLF
        }
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string idStr, nombreCampo, dniCampo, contrasenaCampo;
        
        getline(ss, idStr, '|'); // Con esto lees únicamente hasta el delimitador '|'
        getline(ss, nombreCampo, '|');
        getline(ss, dniCampo, '|');
        getline(ss, contrasenaCampo); // Leer hasta el final de la línea

        while (!contrasenaCampo.empty() && (contrasenaCampo.back() == '\r' || contrasenaCampo.back() == '\n')){
            contrasenaCampo.pop_back();
        }

        // Ignorar encabezados si existiesen
        if (idStr == "idUsuarioRegistrado" || idStr == "id"){
            continue;
        }

        if (idStr.empty() || nombreCampo.empty() || dniCampo.empty() || contrasenaCampo.empty()){
            cerr << "Error: Línea incompleta en el archivo de usuarios." << endl;
            continue;
        }

        try  {
            int id = stoi(idStr); // Convertir el ID de string a int

            // Descifrar nombre y DNI si estaban cifrados; si eran texto plano, descifrarTexto los devuelve intactos
            string nombre = Seguridad::descifrarTexto(nombreCampo);
            string dni = Seguridad::descifrarTexto(dniCampo);

            // Si la contraseña no es un hash SHA-256 (64 hex), significa que estaba en plano
            string contrasenaHash = contrasenaCampo;
            if (!Seguridad::esHashSha256(contrasenaCampo)){
                contrasenaHash = Seguridad::hashContrasena(contrasenaCampo);
                requiereGuardar = true; // Se requiere migrar a formato seguro
            }

            // Comprobar si los campos del archivo estaban en plano para forzar actualización segura
            if (nombreCampo == nombre || dniCampo == dni){
                requiereGuardar = true;
            }

            UsuarioRegistrado usuario(id, nombre, dni, contrasenaHash); // Crear el objeto UsuarioRegistrado con los datos protegidos
            usuarios.push_back(usuario); // Agregar el usuario al vector
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
            continue;
        }
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Usuarios cargados correctamente." << endl;

    // Si había datos en texto plano, los migramos inmediatamente al formato seguro
    if (requiereGuardar && !usuarios.empty()){
        guardarUsuarios(usuarios);
        cout << "Migración de seguridad completada: usuarios cifrados y contraseñas hasheadas en archivo." << endl;
    }

    return usuarios; // Retornar el vector de usuarios
}

vector<CuentaBancaria>GestorArchivos::cargarCuentasBancarias(){
    vector<CuentaBancaria> cuentas;
    string ruta = obtenerRutaArchivo(ARCHIVO_CUENTAS, "cuentasBancarias.txt");
    ifstream archivo(ruta);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de cuentas: " << ruta << endl;
        return cuentas; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    bool requiereGuardar = false;
    string linea;
    while (getline(archivo, linea)){
        while (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')){
            linea.pop_back(); // Eliminar retornos de carro de Windows CRLF
        }
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string ibanCampo, idUsuarioCampo, saldoCampo, tipoCuentaCampo;

        getline(ss, ibanCampo, '|');
        getline(ss, idUsuarioCampo, '|');
        getline(ss, saldoCampo, '|');
        getline(ss, tipoCuentaCampo);

        while (!tipoCuentaCampo.empty() && (tipoCuentaCampo.back() == '\r' || tipoCuentaCampo.back() == '\n')){
            tipoCuentaCampo.pop_back();
        }

        // Ignorar encabezados si existiesen
        if (ibanCampo == "iban" || ibanCampo == "IBAN"){
            continue;
        }

        if (ibanCampo.empty() || idUsuarioCampo.empty() || saldoCampo.empty() || tipoCuentaCampo.empty()){
            cerr << "Error: Línea con campos vacíos: " << linea << endl;
            continue;
        }

        try {
            string iban;
            int idUsuario;
            double saldo;
            int tipoCuenta;

            // Detección: si el IBAN empieza por "ES", el archivo estaba en texto plano
            if (ibanCampo.rfind("ES", 0) == 0) {
                iban = ibanCampo;
                idUsuario = stoi(idUsuarioCampo);
                saldo = stod(saldoCampo);
                tipoCuenta = stoi(tipoCuentaCampo);
                requiereGuardar = true; // Migrar a formato seguro cifrado
            } else {
                iban = Seguridad::descifrarTexto(ibanCampo);
                idUsuario = stoi(Seguridad::descifrarTexto(idUsuarioCampo));
                saldo = stod(Seguridad::descifrarTexto(saldoCampo));
                tipoCuenta = stoi(Seguridad::descifrarTexto(tipoCuentaCampo));
            }

            TipoCuenta tipoCuentaEnum = static_cast<TipoCuenta>(tipoCuenta);
            CuentaBancaria cuenta(iban, idUsuario, saldo, tipoCuentaEnum);
            cuentas.push_back(cuenta);
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
            continue;
        }
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Cuentas cargadas correctamente." << endl;

    if (requiereGuardar && !cuentas.empty()){
        guardarCuentas(cuentas);
        cout << "Migración de seguridad completada: cuentas bancarias cifradas en archivo." << endl;
    }

    return cuentas; // Retornar el vector de cuentas
}

vector<Transaccion>GestorArchivos::cargarTransacciones(){
    vector<Transaccion> transacciones;
    string ruta = obtenerRutaArchivo(ARCHIVO_TRANSACCIONES, "transacciones.txt");
    ifstream archivo(ruta);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de transacciones: " << ruta << endl;
        return transacciones; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    bool requiereGuardar = false;
    string linea;
    while (getline(archivo, linea)){
        while (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')){
            linea.pop_back(); // Eliminar retornos de carro de Windows CRLF
        }
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string idTransaccionStr, ibanOrigenCampo, ibanDestinoCampo, montoCampo, conceptoCampo, fechaCampo;

        getline(ss, idTransaccionStr, '|');
        getline(ss, ibanOrigenCampo, '|');
        getline(ss, ibanDestinoCampo, '|');
        getline(ss, montoCampo, '|');
        getline(ss, conceptoCampo, '|');
        getline(ss, fechaCampo);

        while (!fechaCampo.empty() && (fechaCampo.back() == '\r' || fechaCampo.back() == '\n')){
            fechaCampo.pop_back();
        }

        if (idTransaccionStr == "idTransaccion" || idTransaccionStr == "id"){
            continue;
        }

        if (idTransaccionStr.empty() || ibanOrigenCampo.empty() || ibanDestinoCampo.empty() || montoCampo.empty() || conceptoCampo.empty() || fechaCampo.empty()){
            cerr << "Error: Línea con campos vacíos: " << linea << endl;
            continue;
        }

        try {
            int idTransaccion = stoi(idTransaccionStr);
            string ordenante, beneficiario, concepto, fecha;
            double monto;

            // Detección: si la fecha contiene espacios o no es hexadecimal, estaba en texto plano
            bool esCifrado = (fechaCampo.find(' ') == string::npos && Seguridad::esCadenaHexadecimal(fechaCampo));

            if (!esCifrado) {
                ordenante = ibanOrigenCampo;
                beneficiario = ibanDestinoCampo;
                monto = stod(montoCampo);
                concepto = conceptoCampo;
                fecha = fechaCampo;
                requiereGuardar = true; // Migrar a cifrado
            } else {
                ordenante = Seguridad::descifrarTexto(ibanOrigenCampo);
                beneficiario = Seguridad::descifrarTexto(ibanDestinoCampo);
                monto = stod(Seguridad::descifrarTexto(montoCampo));
                concepto = Seguridad::descifrarTexto(conceptoCampo);
                fecha = Seguridad::descifrarTexto(fechaCampo);
            }

            Transaccion transaccion(idTransaccion, ordenante, beneficiario, monto, concepto, fecha);
            transacciones.push_back(transaccion);
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
        }  
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Transacciones cargadas correctamente." << endl;

    if (requiereGuardar && !transacciones.empty()){
        guardarTransacciones(transacciones);
        cout << "Migración de seguridad completada: transacciones cifradas en archivo." << endl;
    }

    return transacciones; // Retornar el vector de transacciones
}

vector<TarjetaBancaria>GestorArchivos::cargarTarjetasBancarias(){
    vector<TarjetaBancaria> tarjetas;
    string ruta = obtenerRutaArchivo(ARCHIVO_TARJETAS, "tarjetasBancarias.txt");
    ifstream archivo(ruta);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de tarjetas: " << ruta << endl;
        return tarjetas; // Retorna un vector vacío si no se puede abrir el archivo.
    }

    bool requiereGuardar = false;
    string linea;
    while (getline(archivo, linea)){
        while (!linea.empty() && (linea.back() == '\r' || linea.back() == '\n')){
            linea.pop_back(); // Eliminar retornos de carro de Windows CRLF
        }
        if (linea.empty()) continue; // Ignorar líneas vacías
        stringstream ss(linea);
        string numeroTarjetaCampo, ibanCampo, fechaCaducidadCampo, cvvCampo, tipoTarjetaCampo, estadoCampo, pinCampo;

        getline(ss, numeroTarjetaCampo, '|');
        getline(ss, ibanCampo, '|');
        getline(ss, fechaCaducidadCampo, '|');
        getline(ss, cvvCampo, '|');
        getline(ss, tipoTarjetaCampo, '|');
        getline(ss, estadoCampo, '|');
        getline(ss, pinCampo);

        while (!pinCampo.empty() && (pinCampo.back() == '\r' || pinCampo.back() == '\n')){
            pinCampo.pop_back();
        }

        if (numeroTarjetaCampo == "nºtarjeta" || numeroTarjetaCampo == "numeroTarjeta"){
            continue;
        }

        if (numeroTarjetaCampo.empty() || ibanCampo.empty() || fechaCaducidadCampo.empty() || cvvCampo.empty() || tipoTarjetaCampo.empty() || estadoCampo.empty() || pinCampo.empty()){
            cerr << "Error: línea con campos vacíos: " << linea << endl;
            continue;
        }

        try {
            string numeroTarjeta, iban, fechaCaducidad, cvv, pin;
            int tipoTarjeta, estado;

            // Detección: si fechaCaducidad contiene '/' o el IBAN empieza por "ES", el registro está en plano
            bool esCifrado = (fechaCaducidadCampo.find('/') == string::npos && 
                              ibanCampo.rfind("ES", 0) != 0 && 
                              Seguridad::esCadenaHexadecimal(numeroTarjetaCampo) &&
                              numeroTarjetaCampo.size() > 16);

            if (!esCifrado) {
                numeroTarjeta = numeroTarjetaCampo;
                iban = ibanCampo;
                fechaCaducidad = fechaCaducidadCampo;
                cvv = cvvCampo;
                tipoTarjeta = stoi(tipoTarjetaCampo);
                estado = stoi(estadoCampo);
                pin = pinCampo;
                requiereGuardar = true; // Migrar a cifrado
            } else {
                numeroTarjeta = Seguridad::descifrarTexto(numeroTarjetaCampo);
                iban = Seguridad::descifrarTexto(ibanCampo);
                fechaCaducidad = Seguridad::descifrarTexto(fechaCaducidadCampo);
                cvv = Seguridad::descifrarTexto(cvvCampo);
                tipoTarjeta = stoi(Seguridad::descifrarTexto(tipoTarjetaCampo));
                estado = stoi(Seguridad::descifrarTexto(estadoCampo));
                pin = Seguridad::descifrarTexto(pinCampo);
            }

            TipoTarjeta tipoTarjetaEnum = static_cast<TipoTarjeta>(tipoTarjeta);
            TipoEstado estadoEnum = static_cast<TipoEstado>(estado);

            TarjetaBancaria tarjeta(numeroTarjeta, iban, fechaCaducidad, cvv, tipoTarjetaEnum, estadoEnum, pin);
            tarjetas.push_back(tarjeta);
        } catch (const exception& e){
            cerr << "Error al procesar la línea: " << linea << ". Excepción: " << e.what() << endl;
            continue;
        }
        
    }
    archivo.close(); // Cerrar el archivo después de leerlo
    cout << "Tarjetas cargadas correctamente." << endl;

    if (requiereGuardar && !tarjetas.empty()){
        guardarTarjetas(tarjetas);
        cout << "Migración de seguridad completada: tarjetas bancarias cifradas en archivo." << endl;
    }

    return tarjetas; // Retornar el vector de tarjetas
}

void GestorArchivos::guardarUsuarios(vector<UsuarioRegistrado>& usuarios){
    filesystem::create_directories(CARPETA_DATOS);
    ofstream archivo(ARCHIVO_USUARIOS, ios::trunc);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de usuarios para guardar: " << ARCHIVO_USUARIOS << endl;
        return;
    }

    for (UsuarioRegistrado& usuario : usuarios){
        string nombreCifrado = Seguridad::cifrarTexto(usuario.getNombre());
        string dniCifrado = Seguridad::cifrarTexto(usuario.getDni());
        string contrasenaHash = usuario.getContrasena();
        if (!Seguridad::esHashSha256(contrasenaHash)){
            contrasenaHash = Seguridad::hashContrasena(contrasenaHash);
        }

        archivo << usuario.getId() << "|"
                << nombreCifrado << "|"
                << dniCifrado << "|"
                << contrasenaHash << "\n"; // Guardar los datos del usuario protegidos contra accesos no autorizados
    }
    archivo.close();
    cout << "Usuarios guardados correctamente de forma segura en: " << ARCHIVO_USUARIOS << endl;
}

void GestorArchivos::guardarCuentas(vector<CuentaBancaria>& cuentas){
    filesystem::create_directories(CARPETA_DATOS);
    ofstream archivo(ARCHIVO_CUENTAS, ios::trunc);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de cuentas para guardar: " << ARCHIVO_CUENTAS << endl;
        return;
    }

    for (CuentaBancaria& cuenta : cuentas){
        string ibanCifrado = Seguridad::cifrarTexto(cuenta.getIBAN());
        string idUsuarioCifrado = Seguridad::cifrarTexto(to_string(cuenta.getIdUsuario()));
        ostringstream ssSaldo;
        ssSaldo << cuenta.getSaldo();
        string saldoCifrado = Seguridad::cifrarTexto(ssSaldo.str());
        string tipoCuentaCifrado = Seguridad::cifrarTexto(to_string(static_cast<int>(cuenta.getTipoCuenta())));

        archivo << ibanCifrado << "|"
                << idUsuarioCifrado << "|"
                << saldoCifrado << "|"
                << tipoCuentaCifrado << "\n";
    }
    archivo.close();
    cout << "Cuentas guardadas correctamente de forma segura en: " << ARCHIVO_CUENTAS << endl;
}

void GestorArchivos::guardarTransacciones(vector<Transaccion>& transacciones){
    filesystem::create_directories(CARPETA_DATOS);
    ofstream archivo(ARCHIVO_TRANSACCIONES, ios::trunc);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de transacciones para guardar: " << ARCHIVO_TRANSACCIONES << endl;
        return;
    }

    for (Transaccion& transaccion : transacciones){
        string idStr = to_string(transaccion.getId());
        string ordenanteCifrado = Seguridad::cifrarTexto(transaccion.getOrdenante());
        string beneficiarioCifrado = Seguridad::cifrarTexto(transaccion.getBeneficiario());
        ostringstream ssMonto;
        ssMonto << transaccion.getMonto();
        string montoCifrado = Seguridad::cifrarTexto(ssMonto.str());
        string conceptoCifrado = Seguridad::cifrarTexto(transaccion.getConcepto());
        string fechaCifrada = Seguridad::cifrarTexto(transaccion.getFecha());

        archivo << idStr << "|"
                << ordenanteCifrado << "|"
                << beneficiarioCifrado << "|"
                << montoCifrado << "|"
                << conceptoCifrado << "|"
                << fechaCifrada << "\n";
    }

    archivo.close();
    cout << "Transacciones guardadas correctamente de forma segura en: " << ARCHIVO_TRANSACCIONES << endl;
}

void GestorArchivos::guardarTarjetas(vector<TarjetaBancaria>& tarjetas){
    filesystem::create_directories(CARPETA_DATOS);
    ofstream archivo(ARCHIVO_TARJETAS, ios::trunc);

    if (!archivo.is_open()){
        cerr << "Error al abrir el archivo de tarjetas para guardar: " << ARCHIVO_TARJETAS << endl;
        return;
    }

    for (TarjetaBancaria& tarjeta : tarjetas){
        if (tarjeta.getNumeroTarjeta().empty() || tarjeta.getIBAN().empty()){
            cerr << "Error: tarjeta con campos vacíos." << endl;
            continue; // Si hay un error en la tarjeta, se muestra un mensaje de error y se continúa con la siguiente tarjeta.
        }
        
        string numTarjetaCifrado = Seguridad::cifrarTexto(tarjeta.getNumeroTarjeta());
        string ibanCifrado = Seguridad::cifrarTexto(tarjeta.getIBAN());
        string fechaCifrada = Seguridad::cifrarTexto(tarjeta.getFechaCaducidad());
        string cvvCifrado = Seguridad::cifrarTexto(tarjeta.getCVV());
        string tipoCifrado = Seguridad::cifrarTexto(to_string(static_cast<int>(tarjeta.getTipoTarjeta())));
        string estadoCifrado = Seguridad::cifrarTexto(to_string(static_cast<int>(tarjeta.getEstado())));
        string pinCifrado = Seguridad::cifrarTexto(tarjeta.getPIN());

        archivo << numTarjetaCifrado << "|"
                << ibanCifrado << "|"
                << fechaCifrada << "|"
                << cvvCifrado << "|"
                << tipoCifrado << "|"
                << estadoCifrado << "|"
                << pinCifrado << "\n";
    }

    archivo.close();
    cout << "Tarjetas guardadas correctamente de forma segura en: " << ARCHIVO_TARJETAS << endl;
}