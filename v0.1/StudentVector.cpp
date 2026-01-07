#include "StudentVector.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
    bool isValidGrade(int x) { return x >= 0 && x <= 10; }
}

void StudentVector::setVardasPavarde(const std::string& vardas, const std::string& pavarde) {
    vardas_ = vardas;
    pavarde_ = pavarde;
}

void StudentVector::setNd(const std::vector<int>& nd) {
    for (int v : nd) {
        if (!isValidGrade(v)) throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
    }
    nd_ = nd;
}

void StudentVector::setEgz(int egz) {
    if (!isValidGrade(egz)) throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");
    egz_ = egz;
}

double StudentVector::ndVidurkis() const {
    if (nd_.empty()) return 0.0;
    long sum = 0;
    for (int v : nd_) sum += v;
    return static_cast<double>(sum) / static_cast<double>(nd_.size());
}

double StudentVector::ndMediana() const {
    if (nd_.empty()) return 0.0;
    auto tmp = nd_;
    std::sort(tmp.begin(), tmp.end());

    const std::size_t mid = tmp.size() / 2;
    if (tmp.size() % 2 == 1) return static_cast<double>(tmp[mid]);
    return (static_cast<double>(tmp[mid - 1]) + static_cast<double>(tmp[mid])) / 2.0;
}

void StudentVector::skaiciuotiGalutini(bool naudotiMediana) {
    const double baze = naudotiMediana ? ndMediana() : ndVidurkis();
    galutinis_ = 0.4 * baze + 0.6 * static_cast<double>(egz_);
}

std::istream& operator>>(std::istream& in, StudentVector& s) {
    // Format:
    // Pavarde Vardas ND1 ND2 ... NDk Egz (egz paskutinis)
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

    s.nd_ = vals;
    s.egz_ = egz;

    return in;
}

std::ostream& operator<<(std::ostream& out, const StudentVector& s) {
    out << std::left
        << std::setw(20) << s.pavarde_
        << std::setw(15) << s.vardas_
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(12) << s.galutinis_;
    return out;
}