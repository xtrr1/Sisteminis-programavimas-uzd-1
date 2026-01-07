#include "Student.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

int main() {
    try {
        std::size_t m = 0;
        std::size_t n = 0;

        std::cout << "Iveskite studentu skaiciu (m): ";
        std::cin >> m;

        std::cout << "Iveskite namu darbu skaiciu (n): ";
        std::cin >> n;

        if (!std::cin || m == 0) {
            std::cerr << "Klaida: neteisingas m.\n";
            return 1;
        }

        std::vector<Student> grupe;
        grupe.reserve(m);

        std::cout << "\nIvedimas (formatas): Pavarde Vardas ND1..NDn Egz\n";
        std::cout << "Pvz: Sabonis Arvydas 10 9 8 10 9\n\n";

        for (std::size_t i = 0; i < m; ++i) {
            Student s(n);
            std::cout << "Studentas #" << (i + 1) << ": ";
            std::cin >> s;

            grupe.push_back(s);
        }

        std::cout << "\n"
            << std::left
            << std::setw(20) << "Pavarde"
            << std::setw(15) << "Vardas"
            << std::right
            << std::setw(10) << "Galutinis"
            << "\n-----------------------------------------------------\n";

        for (const auto& s : grupe) {
            std::cout << s << "\n";
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\nKlaida: " << e.what() << "\n";
        return 1;
    }
}