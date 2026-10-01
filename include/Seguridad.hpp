#ifndef SEGURIDAD_HPP
#define SEGURIDAD_HPP

#include <string>

class Seguridad {
private:
    // Funciones auxiliares para SHA-256
    static uint32_t rotr(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sig0(uint32_t x);
    static uint32_t sig1(uint32_t x);
    static uint32_t theta0(uint32_t x);
    static uint32_t theta1(uint32_t x);

public:
    // Hashing unidireccional SHA-256 para contraseñas
    static std::string sha256(const std::string& input);
    static std::string hashContrasena(const std::string& contrasena);
    static bool verificarContrasena(const std::string& contrasenaPlana, const std::string& hashAlmacenado);

    // Cifrado y descifrado simétrico para datos de usuarios (Nombre, DNI)
    static std::string cifrarTexto(const std::string& textoPlano, const std::string& clave = "MazeBankKey2025");
    static std::string descifrarTexto(const std::string& textoCifrado, const std::string& clave = "MazeBankKey2025");

    // Métodos de comprobación de formato
    static bool esHashSha256(const std::string& str);
    static bool esCadenaHexadecimal(const std::string& str);
};

#endif // SEGURIDAD_HPP
