# 🏦 Sistema Bancario - Maze Bank

Sistema bancario modular interactivo desarrollado en **C++** (estándar C++17) con arquitectura de **Programación Orientada a Objetos (POO)**, interfaz de consola interactiva (CLI) para Windows, sistema de persistencia en ficheros de texto plano estructurados y capa criptográfica de seguridad avanzada.

---

## 👥 Información del Proyecto

* **Asignatura:** Proyectos de Programación
* **Curso:** 2º Grado en Ingeniería Informática
* **Integrantes del Grupo:**
  * Eduardo Estefanía
  * Rodrigo González
  * Víctor Hernández
* **Repositorio:** [EduardoEO/Proyecto_Sist_Bancario](https://github.com/EduardoEO/Proyecto_Sist_Bancario)

---

## 🚀 Características Principales

### 👤 Gestión de Usuarios y Autenticación
* **Registro seguro:** Validación de DNI único y guardado inmediato a disco.
* **Inicio de sesión protegido:** Verificación mediante huellas hash SHA-256.
* **Gestión de perfil:** Modificación de contraseña y visualización de detalles.
* **Eliminación en cascada:** Al borrar un usuario, se eliminan automáticamente todas sus cuentas y tarjetas vinculadas para mantener la integridad referencial.

### 💳 Gestión de Cuentas Bancarias
* **5 Tipos de Cuenta:** Ahorro, Corriente, Nómina, Inversión y Empresa.
* **Generación de IBAN:** Creación automática de identificador español único de 24 caracteres (`ES...`).
* **Operaciones monetarias:** Ingreso de efectivo, retirada con control de fondos y transferencias inmediatas entre cuentas del banco.
* **Fusión de Cuentas (`operator+=`):** Sobrecarga de operador que permite unificar dos cuentas de un mismo titular, sumando fondos, uniendo historiales y transfiriendo tarjetas compatibles.

### 💳 Gestión de Tarjetas (Débito y Crédito)
* **Generación completa:** Número de 16 dígitos único, fecha de caducidad a 5 años vista (`MM/AA`), CVV de 3 dígitos y PIN configurable de 6 dígitos.
* **Control de compatibilidad:** Restricciones según el tipo de cuenta (las cuentas de Inversión no admiten tarjetas; las de Ahorro no admiten crédito).
* **Detección dinámica de caducidad:** Comprobación automática contra la fecha real del reloj del sistema (`time(0)`).
* **Renovación de tarjetas:** Generación de nueva fecha a +5 años, nuevo código CVV y reactivación inmediata.
* **Operativa de tarjeta:** Activación, bloqueo voluntario y cambio de PIN de acceso.

### 📜 Historial de Movimientos y Exportación
* Registro pormenorizado de depósitos, retiradas y transferencias con fecha/hora real.
* **Exportación a archivo:** Permite generar una copia física del extracto bancario directamente en la carpeta de descargas del usuario de Windows (`%USERPROFILE%\Downloads\historialDeTransacciones.txt`).

---

## 🔒 Sistema de Seguridad Criptográfica

Para evitar el almacenamiento de contraseñas, saldos, números de tarjeta, movimientos y datos personales en texto plano, el proyecto implementa un módulo de seguridad criptográfico desacoplado (`Seguridad`):

1. **Hash Criptográfico SHA-256 con Salt para Contraseñas:**
   * Algoritmo nativo **SHA-256** (estándar criptográfico FIPS 180-4) implementado en C++ sin librerías externas.
   * Incorpora un *Salt* interno (`MazeBank_Salt_Seguridad2025`) para mitigar ataques de diccionario y tablas arcoíris.
   * Las contraseñas se almacenan en `usuariosRegistrados.txt` exclusivamente como huellas digitales unidireccionales de 64 caracteres hexadecimales.

2. **Cifrado Simétrico Reversible para Todos los Archivos de Datos (`data/`):**
   * Toda la información sensible contenida en los ficheros de la base de datos se almacena cifrada en representación hexadecimal:
     * **`usuariosRegistrados.txt`:** ID (numérico) | Nombre (hex cifrado) | DNI (hex cifrado) | Hash SHA-256 (64 hex).
     * **`cuentasBancarias.txt`:** IBAN (hex cifrado) | ID Usuario (hex cifrado) | Saldo (hex cifrado) | Tipo Cuenta (hex cifrado).
     * **`tarjetasBancarias.txt`:** Nº Tarjeta (hex cifrado) | IBAN (hex cifrado) | Fecha Caducidad (hex cifrado) | CVV (hex cifrado) | Tipo Tarjeta (hex cifrado) | Estado (hex cifrado) | PIN (hex cifrado).
     * **`transacciones.txt`:** ID Transacción (numérico) | Cuenta Ordenante (hex cifrado) | Cuenta Beneficiaria (hex cifrado) | Monto (hex cifrado) | Concepto (hex cifrado) | Fecha (hex cifrado).
   * Al abrir cualquiera de los archivos `.txt` en disco, ningún tercero podrá ver números de cuenta, saldos, números de tarjeta, CVVs, códigos PIN ni transferencias bancarias.

3. **Carga y Descifrado Dinámico en Memoria:**
   * Al arrancar el banco, los métodos de `GestorArchivos` descifran en tiempo real los campos protegidos utilizando la clave interna de cifrado (`MazeBankKey2025`), manteniendo en memoria los objetos C++ plenamente operativos.

4. **Migración Automática y Transparente:**
   * Si se añade o modifica manualmente un archivo con registros en texto plano, los métodos de carga detectan automáticamente el formato previo, lo leen sin error y lo migran en el acto al nuevo formato cifrado.

---

## 🔑 Tablas de Referencia para Pruebas (Cheat Sheet)

Como todos los archivos de la carpeta `data/` están cifrados para proteger la privacidad, a continuación se detallan las tablas completas de datos en texto plano para poder realizar pruebas de cada funcionalidad:

> [!TIP]
> **Contraseña universal para todas las cuentas de prueba:** `password123`

### 1. Usuarios y Credenciales de Acceso
| ID | Nombre Titular | DNI *(Usuario)* | Contraseña | Cuentas Vinculadas | Tarjetas y Casuística de Prueba |
|:--:|:---|:---:|:---:|:---|:---|
| **1** | **Juan Perez** | `12345678A` | `password123` | • `ES1740948824551711527614` *(Corriente, 2.468,00 €)* | • Débito `1740948824551711` (PIN: `123456`) - **Activa** |
| **2** | **Maria Lopez** | `23456789B` | `password123` | • `ES2322168576189279543123` *(Ahorro, 10.000,00 €)* | • Débito `7931986502860248` (PIN: `456789`) - **Activa** |
| **3** | **Paco Fernandez** | `34567890C` | `password123` | • `ES3456789012345678901233` *(Nómina, 3.250,50 €)* | • Crédito `3456789012345678` (PIN: `111111`) - **Activa** |
| **4** | **Carlos Gomez** | `45678901D` | `password123` | • `ES4567890123456789012344` *(Empresa, 45.200,00 €)*<br>• `ES4567890123456789012345` *(Corriente, 1.500,00 €)* | • Crédito `4567890123456789` (PIN: `222222`)<br>🎯 *2 cuentas: probar **Fusionar cuentas*** |
| **5** | **Laura Martinez** | `56789012E` | `password123` | • `ES5678901234567890123455` *(Inversión, 18.750,00 €)* | 🎯 *Cuenta Inversión (rechaza tarjetas)* |
| **6** | **David Rodriguez** | `67890123F` | `password123` | • `ES6789012345678901234566` *(Corriente, 890,25 €)* | • Débito `6789012345678901` (PIN: `333333`)<br>🎯 ***Tarjeta Caducada** (fecha 01/24): probar **Renovación*** |
| **7** | **Elena Sanchez** | `78901234G` | `password123` | • `ES7890123456789012345677` *(Nómina, 2.150,00 €)* | • Débito `7890123456789012` (PIN: `444444`)<br>🎯 ***Tarjeta Bloqueada**: probar **Activar tarjeta*** |
| **8** | **Javier Ruiz** | `89012345H` | `password123` | • `ES8901234567890123456788` *(Ahorro, 5.400,00 €)* | • Débito `8901234567890123` (PIN: `555555`) - **Activa** |
| **9** | **Carmen Morales** | `90123456J` | `password123` | • `ES9012345678901234567899` *(Empresa, 78.300,00 €)* | • Crédito `9012345678901234` (PIN: `666666`) - **Activa** |
| **10** | **Eduardo Estefania** | `01234567K` | `password123` | • `ES0123456789012345678900` *(Corriente, 15.000,00 €)*<br>• `ES0123456789012345678901` *(Ahorro, 25.000,00 €)* | • Débito `0123456789012345` (PIN: `777777`)<br>• Crédito `0123456789012346` (PIN: `888888`) |

---

### 2. Cuentas Bancarias (`data/cuentasBancarias.txt`)
> **Valores del enumerador TipoCuenta:** `0` = Ahorro, `1` = Corriente, `2` = Nómina, `3` = Inversión, `4` = Empresa.

| IBAN | ID Titular | Titular | Tipo de Cuenta | Saldo Inicial |
|:---|:---:|:---|:---|:---:|
| `ES1740948824551711527614` | 1 | Juan Perez | Corriente (`1`) | 2.468,00 € |
| `ES2322168576189279543123` | 2 | Maria Lopez | Ahorro (`0`) | 10.000,00 € |
| `ES3456789012345678901233` | 3 | Paco Fernandez | Nómina (`2`) | 3.250,50 € |
| `ES4567890123456789012344` | 4 | Carlos Gomez | Empresa (`4`) | 45.200,00 € |
| `ES4567890123456789012345` | 4 | Carlos Gomez | Corriente (`1`) | 1.500,00 € |
| `ES5678901234567890123455` | 5 | Laura Martinez | Inversión (`3`) | 18.750,00 € |
| `ES6789012345678901234566` | 6 | David Rodriguez | Corriente (`1`) | 890,25 € |
| `ES7890123456789012345677` | 7 | Elena Sanchez | Nómina (`2`) | 2.150,00 € |
| `ES8901234567890123456788` | 8 | Javier Ruiz | Ahorro (`0`) | 5.400,00 € |
| `ES9012345678901234567899` | 9 | Carmen Morales | Empresa (`4`) | 78.300,00 € |
| `ES0123456789012345678900` | 10 | Eduardo Estefania | Corriente (`1`) | 15.000,00 € |
| `ES0123456789012345678901` | 10 | Eduardo Estefania | Ahorro (`0`) | 25.000,00 € |

---

### 3. Tarjetas Bancarias (`data/tarjetasBancarias.txt`)
> **Valores de enumeradores:**
> * **TipoTarjeta:** `0` = Crédito, `1` = Débito.
> * **TipoEstado:** `0` = Activa, `1` = Bloqueada, `2` = Caducada.

| Número de Tarjeta | IBAN Vinculado | Titular | Caducidad | CVV | Tipo | Estado | PIN | Notas de Prueba |
|:---|:---|:---|:---:|:---:|:---|:---|:---:|:---|
| `1740948824551711` | `ES1740948824551711527614` | Juan Perez | 04/30 | 527 | Débito (`1`) | Activa (`0`) | `123456` | Estado operativo normal |
| `7931986502860248` | `ES2322168576189279543123` | Maria Lopez | 04/30 | 650 | Débito (`1`) | Activa (`0`) | `456789` | Estado operativo normal |
| `3456789012345678` | `ES3456789012345678901233` | Paco Fernandez | 05/30 | 333 | Crédito (`0`) | Activa (`0`) | `111111` | Estado operativo normal |
| `4567890123456789` | `ES4567890123456789012344` | Carlos Gomez | 06/30 | 444 | Crédito (`0`) | Activa (`0`) | `222222` | Transferible al fusionar |
| `6789012345678901` | `ES6789012345678901234566` | David Rodriguez | **01/24** | 666 | Débito (`1`) | **Caducada** (`2`) | `333333` | 🎯 Probar renovación |
| `7890123456789012` | `ES7890123456789012345677` | Elena Sanchez | 08/30 | 777 | Débito (`1`) | **Bloqueada** (`1`) | `444444` | 🎯 Probar reactivación |
| `8901234567890123` | `ES8901234567890123456788` | Javier Ruiz | 09/30 | 888 | Débito (`1`) | Activa (`0`) | `555555` | Estado operativo normal |
| `9012345678901234` | `ES9012345678901234567899` | Carmen Morales | 10/30 | 999 | Crédito (`0`) | Activa (`0`) | `666666` | Estado operativo normal |
| `0123456789012345` | `ES0123456789012345678900` | Eduardo Estefania | 11/30 | 123 | Débito (`1`) | Activa (`0`) | `777777` | Tarjeta 1 de Eduardo |
| `0123456789012346` | `ES0123456789012345678900` | Eduardo Estefania | 11/30 | 456 | Crédito (`0`) | Activa (`0`) | `888888` | Tarjeta 2 de Eduardo |

---

### 4. Historial de Transacciones (`data/transacciones.txt`)
| ID | Ordenante *(Origen)* | Beneficiario *(Destino)* | Monto | Concepto | Fecha y Hora |
|:--:|:---|:---|:---:|:---|:---|
| **1** | `Maze Bank` | `ES2322168576189279543123` *(Maria Lopez)* | 10.000,00 € | Ingreso inicial | Wed Apr 30 16:02:51 2025 |
| **2** | `Maze Bank` | `ES1740948824551711527614` *(Juan Perez)* | 2.468,00 € | Ingreso de nómina | Wed Apr 30 16:48:23 2025 |
| **3** | `ES1740948824551711527614` *(Juan Perez)* | `ES2322168576189279543123` *(Maria Lopez)* | 150,00 € | Pago cena compartida | Wed Apr 30 18:20:10 2025 |
| **4** | `Maze Bank` | `ES4567890123456789012344` *(Carlos Gomez)* | 45.200,00 € | Aportacion capital | Thu May 01 09:30:00 2025 |
| **5** | `Maze Bank` | `ES0123456789012345678900` *(Eduardo Estefania)* | 15.000,00 € | Ingreso cuenta personal | Thu May 01 10:15:00 2025 |
| **6** | `ES0123456789012345678900` *(Eduardo Estefania)* | `ES0123456789012345678901` *(Eduardo Estefania)* | 5.000,00 € | Traspaso a cuenta ahorro | Thu May 01 11:00:00 2025 |

---

## 🛠️ Estructura del Repositorio

```text
Proyecto Sist Bancario/
├── include/                   # Archivos de cabecera (.hpp)
│   ├── Banco.hpp              # Núcleo y lógica de negocio
│   ├── CuentaBancaria.hpp     # Entidad de cuenta financiera
│   ├── GestorArchivos.hpp     # Capa de persistencia E/S
│   ├── Menu.hpp               # Menús y capa de presentación CLI
│   ├── Seguridad.hpp          # Módulo criptográfico (SHA-256 y cifrado)
│   ├── TarjetaBancaria.hpp    # Entidad de tarjetas bancarias
│   ├── TipoCuenta.hpp         # Enum de tipos de cuenta
│   ├── TipoEstado.hpp         # Enum de estados de tarjeta
│   ├── TipoTarjeta.hpp        # Enum de crédito / débito
│   ├── Transaccion.hpp        # Entidad de movimiento bancario
│   ├── Usuario.hpp            # Clase base abstracta
│   ├── UsuarioNoRegistrado.hpp
│   └── UsuarioRegistrado.hpp
├── src/                       # Implementaciones en C++ (.cpp)
│   ├── Banco.cpp
│   ├── CuentaBancaria.cpp
│   ├── GestorArchivos.cpp
│   ├── Menu.cpp
│   ├── Seguridad.cpp
│   ├── TarjetaBancaria.cpp
│   ├── Transaccion.cpp
│   ├── Usuario.cpp
│   ├── UsuarioNoRegistrado.cpp
│   ├── UsuarioRegistrado.cpp
│   └── main.cpp
├── build/                     # Carpeta de compilación y salida binaria
│   ├── obj/                   # Archivos intermedios objeto (.obj)
│   ├── output.pdb             # Archivo de símbolos de depuración
│   └── output.exe             # Ejecutable final de Windows
├── data/                      # Base de datos persistente cifrada
│   ├── usuariosRegistrados.txt    # Clientes (cifrados con hash SHA-256)
│   ├── cuentasBancarias.txt       # Cuentas bancarias (cifradas)
│   ├── tarjetasBancarias.txt      # Tarjetas asociadas (cifradas)
│   └── transacciones.txt          # Historial de transacciones (cifrado)
├── Documentación/             # Documentos de entrega e informes en PDF
├── .vscode/                   # Configuración y tareas de compilación
│   └── tasks.json
├── .gitignore                 # Filtro de exclusión de Git
└── README.md                  # Este documento
```

---

## 💻 Compilación y Ejecución

### Opción 1: Developer Command Prompt for VS (`cl.exe`) - Recomendado
Abre la consola **Developer Command Prompt for VS 2022** (o Developer PowerShell) y ejecuta desde la raíz del proyecto:
```cmd
cl.exe /std:c++17 /EHsc /Fo.\build\obj\ /Fd.\build\output.pdb /Fe.\build\output.exe src\*.cpp
```
* **`/std:c++17`**: Utiliza el estándar ISO C++17.
* **`/EHsc`**: Habilita el manejo estructurado de excepciones de C++.
* **`/Fo.\build\obj\`**: Envía todos los archivos intermedios `.obj` dentro de la subcarpeta `build/obj/` para mantener limpio el proyecto.
* **`/Fe.\build\output.exe`**: Genera el archivo ejecutable en `build/output.exe`.

---

### Opción 2: Compilar en Visual Studio Code
El repositorio incluye la tarea configurada en `.vscode/tasks.json`.
1. Abre la carpeta del proyecto en **Visual Studio Code**.
2. Presiona el atajo **`Ctrl + Shift + B`**.
3. El proyecto se compilará automáticamente con `cl.exe`, ubicando los `.obj` en `build/obj/` y el ejecutable en `build/output.exe`.
4. *(Opcional)* Presiona **`F5`** si deseas iniciar la sesión de depuración integrada.

---

### ▶️ Ejecución del Programa

> [!IMPORTANT]
> **Ejecuta siempre el programa desde la raíz del proyecto**, no desde dentro de la subcarpeta `build/`. De este modo, el sistema localiza correctamente la carpeta `data/` de la base de datos persistente.

En la consola (PowerShell o CMD) desde la raíz del proyecto:
```powershell
.\build\output.exe
```
