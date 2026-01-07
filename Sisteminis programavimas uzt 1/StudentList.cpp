#include "StudentList.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {

bool looksLikeStudentLine(const std::string& line) {
    std::istringstream iss(line);
    std::string a, b;
    if (!(iss >> a >> b)) return false;
    int x;
    return static_cast<bool>(iss >> x);
}

auto cmpFinalAvg = [](const StudentVector& a, const StudentVector& b) {
    if (a.finalAvg() != b.finalAvg()) return a.finalAvg() < b.finalAvg();
    if (a.surname() != b.surname()) return a.surname() < b.surname();
    return a.name() < b.name();
};

}

void StudentList::readFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) throw std::runtime_error("Nepavyko atidaryti failo: " + filename);

    students_.clear();

    std::streampos pos = fin.tellg();
    std::string firstLine;
    if (std::getline(fin, firstLine)) {
        if (looksLikeStudentLine(firstLine)) {
            fin.clear();
            fin.seekg(pos);
        }
    } else {
        return;
    }

    while (true) {
        StudentVector s;
        if (fin >> s) {
            s.computeFinals();
            students_.push_back(std::move(s));
            continue;
        }
        if (fin.eof()) break;
        fin.clear();
        std::string dummy;
        std::getline(fin, dummy);
    }
}

void StudentList::splitAndSortByFinalAvg(std::list<StudentVector>& lt5, std::list<StudentVector>& ge5) const {
    lt5.clear();
    ge5.clear();

    for (const auto& s : students_) {
        if (s.finalAvg() < 5.0) lt5.push_back(s);
        else ge5.push_back(s);
    }

    lt5.sort(cmpFinalAvg);
    ge5.sort(cmpFinalAvg);
}

void StudentList::writeResultsFile(const std::string& filename, const std::list<StudentVector>& v) const {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Nepavyko sukurti failo: " + filename);

    out << std::left
        << std::setw(18) << "Vardas"
        << std::setw(20) << "Pavarde"
        << std::right
        << std::setw(16) << "Galutinis(Vid.)"
        << std::setw(16) << "Galutinis(Med.)"
        << '\n';

    for (const auto& s : v) out << s << '\n';
}
