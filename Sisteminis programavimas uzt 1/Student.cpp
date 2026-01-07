#include "Student.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
    bool isValidGrade(int x) { return x >= 0 && x <= 10; }
}

Student::Student() = default;

void Student::freeNd() {
    delete[] nd_;
    nd_ = nullptr;
    ndCount_ = 0;
}

void Student::copyNdFrom(const Student& other) {
    if (other.ndCount_ == 0) {
        freeNd();
        return;
    }
    int* tmp = new int[other.ndCount_];
    for (std::size_t i = 0; i < other.ndCount_; ++i) tmp[i] = other.nd_[i];

    delete[] nd_;
    nd_ = tmp;
    ndCount_ = other.ndCount_;
}

Student::Student(const Student& other)
    : vardas_(other.vardas_),
    pavarde_(other.pavarde_),
    egz_(other.egz_),
    galutinis_(other.galutinis_) {
    copyNdFrom(other);
}

Student& Student::operator=(const Student& other) {
    if (this == &other) return *this;

    vardas_ = other.vardas_;
    pavarde_ = other.pavarde_;
    egz_ = other.egz_;
    galutinis_ = other.galutinis_;
    copyNdFrom(other);

    return *this;
}

Student::~Student() {
    freeNd();
}

void Student::setVardasPavarde(const std::string& vardas, const std::string& pavarde) {
    vardas_ = vardas;
    pavarde_ = pavarde;
}

void Student::setNd(const int* nd, std::size_t count) {
    freeNd();
    ndCount_ = count;
    if (ndCount_ == 0) return;

    nd_ = new int[ndCount_];
    for (std::size_t i = 0; i < ndCount_; ++i) {
        if (!isValidGrade(nd[i])) throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
        nd_[i] = nd[i];
    }
}

void Student::setEgz(int egz) {
    if (!isValidGrade(egz)) throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");
    egz_ = egz;
}

double Student::ndVidurkis() const {
    if (ndCount_ == 0) return 0.0;
    long sum = 0;
    for (std::size_t i = 0; i < ndCount_; ++i) sum += nd_[i];
    return static_cast<double>(sum) / static_cast<double>(ndCount_);
}

double Student::ndMediana() const {
    if (ndCount_ == 0) return 0.0;
    std::vector<int> tmp(nd_, nd_ + ndCount_);
    std::sort(tmp.begin(), tmp.end());

    const std::size_t mid = tmp.size() / 2;
    if (tmp.size() % 2 == 1) return static_cast<double>(tmp[mid]);
    return (static_cast<double>(tmp[mid - 1]) + static_cast<double>(tmp[mid])) / 2.0;
}

void Student::skaiciuotiGalutini(bool naudotiMediana) {
    const double baze = naudotiMediana ? ndMediana() : ndVidurkis();
    galutinis_ = 0.4 * baze + 0.6 * static_cast<double>(egz_);
}

std::istream& operator>>(std::istream& in, Student& s) {

    if (!(in >> s.pavarde_ >> s.vardas_)) return in;

    std::string line;
    std::getline(in >> std::ws, line);

    std::stringstream ss(line);
    std::vector<int> vals;
    int x;
    while (ss >> x) vals.push_back(x);

    if (vals.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    const int egz = vals.back();
    vals.pop_back();

    for (int v : vals) {
        if (!isValidGrade(v)) throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
    }
    if (!isValidGrade(egz)) throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");

    if (!vals.empty()) s.setNd(vals.data(), vals.size());
    else s.setNd(nullptr, 0);

    s.setEgz(egz);

    return in;
}

std::ostream& operator<<(std::ostream& out, const Student& s) {
    out << std::left
        << std::setw(20) << s.pavarde_
        << std::setw(15) << s.vardas_
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(12) << s.galutinis_;
    return out;
}