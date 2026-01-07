#pragma once

#include <iosfwd>
#include <string>
#include <cstddef>

class Student {
private:
    std::string vardas_;
    std::string pavarde_;

    int* nd_ = nullptr;
    std::size_t ndCount_ = 0;

    int egz_ = 0;
    double galutinis_ = 0.0;

    void freeNd();
    void copyNdFrom(const Student& other);

public:
    Student();
    Student(const Student& other);
    Student& operator=(const Student& other);
    ~Student();

    void setVardasPavarde(const std::string& vardas, const std::string& pavarde);
    void setNd(const int* nd, std::size_t count);
    void setEgz(int egz);

    double ndVidurkis() const;
    double ndMediana() const;
    void skaiciuotiGalutini(bool naudotiMediana);

    const std::string& vardas() const { return vardas_; }
    const std::string& pavarde() const { return pavarde_; }
    int egz() const { return egz_; }
    std::size_t ndCount() const { return ndCount_; }
    double galutinis() const { return galutinis_; }

    friend std::istream& operator>>(std::istream& in, Student& s);
    friend std::ostream& operator<<(std::ostream& out, const Student& s);
};