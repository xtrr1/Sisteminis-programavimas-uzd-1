#pragma once

#include <iosfwd>
#include <string>
#include <cstddef>

class Student {
private:
    std::string vardas_;
    std::string pavarde_;

    int* nd_ = nullptr;          // pilnai dinaminis C masyvas
    std::size_t ndCount_ = 0;

    int egz_ = 0;
    double galutinis_ = 0.0;

    void freeNd();
    void copyNdFrom(const Student& other);

public:
    // Constructors
    Student();
    Student(const Student& other);               // rule of three
    Student& operator=(const Student& other);    // rule of three
    ~Student();                                  // rule of three

    // Setters (naudinga atsitiktinei generacijai)
    void setVardasPavarde(const std::string& vardas, const std::string& pavarde);
    void setNd(const int* nd, std::size_t count);
    void setEgz(int egz);

    // Skaičiavimai
    double ndVidurkis() const;
    double ndMediana() const;
    void skaiciuotiGalutini(bool naudotiMediana);

    // Getters (output)
    const std::string& vardas() const { return vardas_; }
    const std::string& pavarde() const { return pavarde_; }
    int egz() const { return egz_; }
    std::size_t ndCount() const { return ndCount_; }
    double galutinis() const { return galutinis_; }

    // Įvedimas/išvedimas (nežinomas ND kiekis: ND... Egz – egz yra paskutinis skaičius)
    friend std::istream& operator>>(std::istream& in, Student& s);
    friend std::ostream& operator<<(std::ostream& out, const Student& s);
};