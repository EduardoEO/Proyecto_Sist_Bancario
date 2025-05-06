#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <string>

using namespace std;

class Transaccion{
    private:
        int id;
        string ordenante; // IBAN del ordenante
        string beneficiario; // IBAN del beneficiario
        double monto; // Monto de la transaccion
        string concepto;
        string fecha;

    public:
    Transaccion(int id, string ordenante, string beneficiario, double monto, string concepto, string fecha);
    
    int getId();
    string getOrdenante();
    string getBeneficiario();
    double getMonto();
    string getConcepto();
    string getFecha();

    void mostrarDetalles() const; // Muestra los detalles de la transacción.
    string mostrarDetallesStr() const; // Devuelve los detalles de la transacción como una cadena de texto.

};

#endif // TRANSACCION_HPP
