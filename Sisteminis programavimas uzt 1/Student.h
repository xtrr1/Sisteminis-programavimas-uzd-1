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
    explicit Student(std::size_t ndCount);
    Student(const std::string& vardas, const std::string& pavarde,
        const int* nd, std::size_t ndCount, int egz);

    //Tryju taisykle
    Student(const Student& other);               
    Student& operator=(const Student& other);    
    ~Student();                                  

    void setNdCount(std::size_t ndCount);

    double ndVidurkis() const;
    void skaiciuotiGalutiniIsVidurkio();

    const std::string& vardas() const { return vardas_; }
    const std::string& pavarde() const { return pavarde_; }
    double galutinis() const { return galutinis_; }

    friend std::istream& operator>>(std::istream& in, Student& s);
    friend std::ostream& operator<<(std::ostream& out, const Student& s);
};