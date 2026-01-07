#include "Student.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace {
    bool isValidGrade(int x) { return x >= 0 && x <= 10; }
}

Student::Student() = default;

Student::Student(std::size_t ndCount) {
    setNdCount(ndCount);
}

Student::Student(const std::string& vardas, const std::string& pavarde,
    const int* nd, std::size_t ndCount, int egz)
    : vardas_(vardas), pavarde_(pavarde), egz_(egz) {
    setNdCount(ndCount);
    for (std::size_t i = 0; i < ndCount_; ++i) nd_[i] = nd[i];
    if (!isValidGrade(egz_)) throw std::invalid_argument("Netinkamas egzamino pazymys");
    skaiciuotiGalutiniIsVidurkio();
}

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

void Student::setNdCount(std::size_t ndCount) {
    freeNd();
    ndCount_ = ndCount;
    if (ndCount_ > 0) nd_ = new int[ndCount_];
}

double Student::ndVidurkis() const {
    if (ndCount_ == 0) return 0.0;
    long sum = 0;
    for (std::size_t i = 0; i < ndCount_; ++i) sum += nd_[i];
    return static_cast<double>(sum) / static_cast<double>(ndCount_);
}

void Student::skaiciuotiGalutiniIsVidurkio() {
    galutinis_ = 0.4 * ndVidurkis() + 0.6 * static_cast<double>(egz_);
}

std::istream& operator>>(std::istream& in, Student& s) {
    in >> s.pavarde_ >> s.vardas_;

    for (std::size_t i = 0; i < s.ndCount_; ++i) {
        int x;
        in >> x;
        if (!in) return in;
        if (!isValidGrade(x)) throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
        s.nd_[i] = x;
    }

    in >> s.egz_;
    if (!in) return in;
    if (!isValidGrade(s.egz_)) throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");

    s.skaiciuotiGalutiniIsVidurkio();
    return in;
}

std::ostream& operator<<(std::ostream& out, const Student& s) {
    out << std::left
        << std::setw(20) << s.pavarde_
        << std::setw(15) << s.vardas_
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(10) << s.galutinis_;
    return out;
}