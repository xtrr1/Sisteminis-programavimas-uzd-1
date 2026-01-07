#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <cstddef>

class StudentVector {
private:
    std::string vardas_;
    std::string pavarde_;
    std::vector<int> nd_;
    int egz_ = 0;
    double galutinis_ = 0.0;

public:
    StudentVector() = default;

    void setVardasPavarde(const std::string& vardas, const std::string& pavarde);
    void setNd(const std::vector<int>& nd);
    void setEgz(int egz);

    double ndVidurkis() const;
    double ndMediana() const;
    void skaiciuotiGalutini(bool naudotiMediana);

    const std::string& vardas() const { return vardas_; }
    const std::string& pavarde() const { return pavarde_; }
    int egz() const { return egz_; }
    std::size_t ndCount() const { return nd_.size(); }
    double galutinis() const { return galutinis_; }

    friend std::istream& operator>>(std::istream& in, StudentVector& s);
    friend std::ostream& operator<<(std::ostream& out, const StudentVector& s);
};