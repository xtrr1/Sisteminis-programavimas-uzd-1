#include "Student.h"
#include "StudentVector.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

namespace {

    int readInt(const std::string& prompt, int minVal, int maxVal) {
        while (true) {
            std::cout << prompt;
            int x;
            if (std::cin >> x && x >= minVal && x <= maxVal) return x;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Neteisinga ivestis. Bandykite dar karta.\n";
        }
    }

    bool readYesNo(const std::string& prompt) {
        while (true) {
            std::cout << prompt << " (t/n): ";
            std::string s;
            std::cin >> s;
            if (s == "t" || s == "T") return true;
            if (s == "n" || s == "N") return false;
            std::cout << "Iveskite 't' arba 'n'.\n";
        }
    }

    bool chooseMedian() {
        int m = readInt("Skaičiavimo metodas: 1 - Vidurkis, 2 - Mediana: ", 1, 2);
        return m == 2;
    }

    void printHeader(bool median) {
        std::cout << "\n"
            << std::left
            << std::setw(20) << "Pavarde"
            << std::setw(15) << "Vardas"
            << std::right
            << std::setw(12) << (median ? "Galutinis(Med.)" : "Galutinis(Vid.)")
            << "\n-------------------------------------------------------------\n";
    }

    void readNameSurname(int idx, std::string& pavarde, std::string& vardas) {
        std::cout << "Iveskite studento #" << idx << " pavarde ir varda: ";
        std::cin >> pavarde >> vardas;
    }
    // ==== C masyvo versija ====
    void runCArray() {
        bool median = chooseMedian();
        bool randomGen = readYesNo("Ar generuoti pazymius atsitiktinai?");
        int m = readInt("Iveskite studentu skaiciu: ", 1, 1000000);

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 10);

        std::vector<Student> grupe;
        grupe.reserve(static_cast<std::size_t>(m));

        if (!randomGen) {
            std::cout << "\nIvedimas vienoje eiluteje:\n"
                << "Pavarde Vardas ND1 ND2 ... NDk Egz\n"
                << "Pvz: Sabonis Arvydas 10 9 8 10 9\n"
                << "(Egzaminas visada paskutinis skaicius)\n\n";

            for (int i = 0; i < m; ++i) {
                Student s;
                std::cout << "Studentas #" << (i + 1) << ": ";
                std::cin >> s;
                s.skaiciuotiGalutini(median);
                grupe.push_back(s);
            }
        }
        else {
            int ndCount = readInt("Kiek ND generuoti kiekvienam studentui? ", 0, 1000);

            // SVARBU: vardas/pavarde įvedami ranka, pažymiai generuojami atsitiktinai
            for (int i = 0; i < m; ++i) {
                std::string pavarde, vardas;
                readNameSurname(i + 1, pavarde, vardas);

                Student s;
                s.setVardasPavarde(vardas, pavarde);

                std::vector<int> nd(static_cast<std::size_t>(ndCount));
                for (int& v : nd) v = dist(gen);

                if (!nd.empty()) s.setNd(nd.data(), nd.size());
                else s.setNd(nullptr, 0);

                s.setEgz(dist(gen));
                s.skaiciuotiGalutini(median);

                grupe.push_back(s);
            }
        }

        printHeader(median);
        for (const auto& s : grupe) std::cout << s << "\n";
    }
    // ==== std::vector versija ====
    void runVector() {
        bool median = chooseMedian();
        bool randomGen = readYesNo("Ar generuoti pazymius atsitiktinai?");
        int m = readInt("Iveskite studentu skaiciu: ", 1, 10000000);

        std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 10);

        std::vector<StudentVector> grupe;
        grupe.reserve(static_cast<std::size_t>(m));

        if (!randomGen) {
            std::cout << "\nIvedimas vienoje eiluteje:\n"
                << "Pavarde Vardas ND1 ND2 ... NDk Egz\n"
                << "Pvz: Sabonis Arvydas 10 9 8 10 9\n"
                << "(Egzaminas visada paskutinis skaicius)\n\n";

            for (int i = 0; i < m; ++i) {
                StudentVector s;
                std::cout << "Studentas #" << (i + 1) << ": ";
                std::cin >> s;
                s.skaiciuotiGalutini(median);
                grupe.push_back(s);
            }
        }
        else {
            int ndCount = readInt("Kiek ND generuoti kiekvienam studentui? ", 0, 1000);

            // SVARBU: vardas/pavarde įvedami ranka, pažymiai generuojami atsitiktinai
            for (int i = 0; i < m; ++i) {
                std::string pavarde, vardas;
                readNameSurname(i + 1, pavarde, vardas);

                StudentVector s;
                s.setVardasPavarde(vardas, pavarde);

                std::vector<int> nd(static_cast<std::size_t>(ndCount));
                for (int& v : nd) v = dist(gen);

                s.setNd(nd);
                s.setEgz(dist(gen));
                s.skaiciuotiGalutini(median);

                grupe.push_back(s);
            }
        }

        printHeader(median);
        for (const auto& s : grupe) std::cout << s << "\n";
    }

} // namespace

int main() {
    try {
        std::cout << "Pasirinkite realizacija:\n"
            << "1 - Dinaminis C masyvas\n"
            << "2 - std::vector\n";

        int impl = readInt("Jusu pasirinkimas: ", 1, 2);

        if (impl == 1) runCArray();
        else runVector();

        std::cout << "\nPastaba: tolesniuose etapuose dirbame tik su vektorine versija.\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\nKlaida: " << e.what() << "\n";
        return 1;
    }
}
