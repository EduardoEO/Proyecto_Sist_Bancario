#include "../include/Seguridad.hpp"

#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cctype>

using namespace std;

// Constantes de redondeo K para SHA-256 (FIPS 180-4)
static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

uint32_t Seguridad::rotr(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t Seguridad::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t Seguridad::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t Seguridad::sig0(uint32_t x) {
    return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint32_t Seguridad::sig1(uint32_t x) {
    return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

uint32_t Seguridad::theta0(uint32_t x) {
    return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t Seguridad::theta1(uint32_t x) {
    return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}

string Seguridad::sha256(const string& input) {
    // Valores de inicialización H para SHA-256
    uint32_t H[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Pre-procesamiento y relleno (Padding)
    vector<uint8_t> msg(input.begin(), input.end());
    uint64_t bitLength = static_cast<uint64_t>(msg.size()) * 8;

    msg.push_back(0x80); // Añadir bit '1'
    while ((msg.size() % 64) != 56) {
        msg.push_back(0x00);
    }

    // Añadir longitud en 64 bits big-endian
    for (int i = 7; i >= 0; --i) {
        msg.push_back(static_cast<uint8_t>((bitLength >> (i * 8)) & 0xFF));
    }

    // Procesar en bloques de 512 bits (64 bytes)
    for (size_t chunk = 0; chunk < msg.size(); chunk += 64) {
        uint32_t W[64];
        for (int t = 0; t < 16; ++t) {
            W[t] = (static_cast<uint32_t>(msg[chunk + t * 4]) << 24) |
                   (static_cast<uint32_t>(msg[chunk + t * 4 + 1]) << 16) |
                   (static_cast<uint32_t>(msg[chunk + t * 4 + 2]) << 8) |
                   (static_cast<uint32_t>(msg[chunk + t * 4 + 3]));
        }
        for (int t = 16; t < 64; ++t) {
            W[t] = theta1(W[t - 2]) + W[t - 7] + theta0(W[t - 15]) + W[t - 16];
        }

        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];
        uint32_t f = H[5];
        uint32_t g = H[6];
        uint32_t h = H[7];

        for (int t = 0; t < 64; ++t) {
            uint32_t T1 = h + sig1(e) + choose(e, f, g) + K[t] + W[t];
            uint32_t T2 = sig0(a) + majority(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
        H[5] += f;
        H[6] += g;
        H[7] += h;
    }

    // Generar cadena hexadecimal de 64 caracteres
    ostringstream oss;
    for (int i = 0; i < 8; ++i) {
        oss << hex << setw(8) << setfill('0') << H[i];
    }
    return oss.str();
}

string Seguridad::hashContrasena(const string& contrasena) {
    // Aplicamos salt criptográfico para prevenir ataques de tablas arcoíris
    string salt = "MazeBank_Salt_Seguridad2025";
    return sha256(contrasena + salt);
}

bool Seguridad::verificarContrasena(const string& contrasenaPlana, const string& hashAlmacenado) {
    // Comprobar contra el hash nuevo
    if (hashContrasena(contrasenaPlana) == hashAlmacenado) {
        return true;
    }
    // Compatibilidad por si en memoria o archivo previo había una contraseña plana
    if (contrasenaPlana == hashAlmacenado) {
        return true;
    }
    return false;
}

string Seguridad::cifrarTexto(const string& textoPlano, const string& clave) {
    if (textoPlano.empty()) return "";
    string claveUsada = clave.empty() ? "MazeBankKey2025" : clave;
    ostringstream oss;
    for (size_t i = 0; i < textoPlano.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(textoPlano[i]);
        unsigned char k = static_cast<unsigned char>(claveUsada[i % claveUsada.size()]);
        unsigned char enc = static_cast<unsigned char>((c ^ k) + ((i * 7 + 13) % 256));
        oss << hex << setw(2) << setfill('0') << static_cast<int>(enc);
    }
    return oss.str();
}

string Seguridad::descifrarTexto(const string& textoCifrado, const string& clave) {
    if (textoCifrado.empty() || (textoCifrado.size() % 2 != 0)) {
        return textoCifrado; // Si no tiene longitud par, devolver texto tal cual
    }
    if (!esCadenaHexadecimal(textoCifrado)) {
        return textoCifrado; // Si no es hexadecimal, es texto plano previo
    }

    string claveUsada = clave.empty() ? "MazeBankKey2025" : clave;
    string resultado;
    resultado.reserve(textoCifrado.size() / 2);

    for (size_t i = 0; i < textoCifrado.size(); i += 2) {
        string byteStr = textoCifrado.substr(i, 2);
        try {
            int byteVal = stoi(byteStr, nullptr, 16);
            size_t pos = i / 2;
            unsigned char k = static_cast<unsigned char>(claveUsada[pos % claveUsada.size()]);
            unsigned char enc = static_cast<unsigned char>(byteVal);
            unsigned char dec = static_cast<unsigned char>((enc - ((pos * 7 + 13) % 256)) ^ k);
            resultado.push_back(static_cast<char>(dec));
        } catch (...) {
            return textoCifrado;
        }
    }
    return resultado;
}

bool Seguridad::esHashSha256(const string& str) {
    if (str.size() != 64) return false;
    for (char c : str) {
        if (!isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool Seguridad::esCadenaHexadecimal(const string& str) {
    if (str.empty() || (str.size() % 2 != 0)) return false;
    for (char c : str) {
        if (!isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}
