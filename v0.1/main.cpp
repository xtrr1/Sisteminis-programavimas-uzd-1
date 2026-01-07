#include "StudentVector.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace {

    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin >> std::ws, line);
        return line;
    }

    std::vector<StudentVector> readStudentsFromFile(const std::string& filename) {
        std::ifstream fin(filename);
        if (!fin) {
            throw std::runtime_error("Nepavyko atidaryti failo: " + filename);
        }

        std::vector<StudentVector> students;
        std::string line;

        while (std::getline(fin, line)) {
            bool onlySpaces = true;
            for (char c : line) {
                if (!std::isspace(static_cast<unsigned char>(c))) { onlySpaces = false; break; }
            }
            if (onlySpaces) continue;

            std::istringstream iss(line);
            StudentVector s;

            if (!(iss >> s)) continue;

            s.computeFinals();
            students.push_back(std::move(s));
        }

        return students;
    }

    void sortByNameSurname(std::vector<StudentVector>& v) {
        std::sort(v.begin(), v.end(),
            [](const StudentVector& a, const StudentVector& b) {
                if (a.name() != b.name()) return a.name() < b.name();
                return a.surname() < b.surname();
            });
    }

    void printTable(const std::vector<StudentVector>& v) {
        std::cout << "\n"
            << std::left
            << std::setw(18) << "Vardas"
            << std::setw(20) << "Pavarde"
            << std::right
            << std::setw(16) << "Galutinis(Vid.)"
            << std::setw(16) << "Galutinis(Med.)"
            << "\n--------------------------------------------------------------------------\n";

        for (const auto& s : v) {
            std::cout << s << "\n";
        }
    }

}

int main() {
    try {
        std::cout << "Duomenu nuskaitymas is failo (vector implementacija)\n";
        std::cout << "Eilutes formatas: Vardas Pavarde ND1 ND2 ... NDx Final\n";
        std::cout << "Pastaba: paskutinis skaicius interpretuojamas kaip EGZAMINO pazymys.\n";

        const std::string filename = readLine("\nIveskite failo pavadinima (pvz. kursiokai.txt): ");

        auto students = readStudentsFromFile(filename);
        if (students.empty()) {
            std::cout << "\nNerasta tinkamu irasu faile (arba visi buvo antrastes/neteisingi).\n";
            return 0;
        }

        sortByNameSurname(students);
        printTable(students);

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\nKlaida: " << e.what() << "\n";
        return 1;
    }
}