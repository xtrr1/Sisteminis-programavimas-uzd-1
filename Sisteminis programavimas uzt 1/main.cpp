#include "StudentVector.h"
#include "StudentDeque.h"
#include "StudentList.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::high_resolution_clock;

double seconds(Clock::duration d) {
    return std::chrono::duration<double>(d).count();
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin >> std::ws, line);
    return line;
}

std::size_t readCount(const std::string& prompt, std::size_t fallback) {
    std::string line = readLine(prompt);
    if (line.empty()) return fallback;
    std::size_t n = 0;
    try {
        n = static_cast<std::size_t>(std::stoull(line));
    } catch (...) {
        return fallback;
    }
    return n;
}

std::string inputFilename(std::size_t n) {
    return "students_" + std::to_string(n) + ".txt";
}

std::string outFilenameLT5(std::size_t n, const std::string& tag) {
    return "students_" + std::to_string(n) + "_maziau5" + tag + ".txt";
}

std::string outFilenameGE5(std::size_t n, const std::string& tag) {
    return "students_" + std::to_string(n) + "_daugiau5" + tag + ".txt";
}

void generateStudentsFile(const std::string& filename, std::size_t n, int hwCount) {
    std::ofstream out(filename);
    if (!out) throw std::runtime_error("Nepavyko sukurti failo: " + filename);

    out << "Vardas Pavarde";
    for (int i = 1; i <= hwCount; ++i) out << " ND" << i;
    out << " Egzaminas\n";

    std::mt19937 gen(static_cast<std::mt19937::result_type>(std::random_device{}()));
    std::uniform_int_distribution<int> dist(1, 10);

    for (std::size_t i = 1; i <= n; ++i) {
        out << "Vardas" << i << ' ' << "Pavarde" << i;
        for (int j = 0; j < hwCount; ++j) out << ' ' << dist(gen);
        out << ' ' << dist(gen) << '\n';
    }
}

bool looksLikeStudentLine(const std::string& line) {
    std::istringstream iss(line);
    std::string a, b;
    if (!(iss >> a >> b)) return false;
    int x;
    return static_cast<bool>(iss >> x);
}

std::vector<StudentVector> readStudentsFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) throw std::runtime_error("Nepavyko atidaryti failo: " + filename);

    std::vector<StudentVector> students;

    std::streampos pos = fin.tellg();
    std::string firstLine;
    if (std::getline(fin, firstLine)) {
        if (looksLikeStudentLine(firstLine)) {
            fin.clear();
            fin.seekg(pos);
        }
    } else {
        return students;
    }

    while (true) {
        StudentVector s;
        if (fin >> s) {
            s.computeFinals();
            students.push_back(std::move(s));
            continue;
        }
        if (fin.eof()) break;
        fin.clear();
        std::string dummy;
        std::getline(fin, dummy);
    }

    return students;
}

void splitAndSortByFinalAvg(const std::vector<StudentVector>& all, std::vector<StudentVector>& lt5, std::vector<StudentVector>& ge5) {
    lt5.clear();
    ge5.clear();
    lt5.reserve(all.size() / 2);
    ge5.reserve(all.size() / 2);

    for (const auto& s : all) {
        if (s.finalAvg() < 5.0) lt5.push_back(s);
        else ge5.push_back(s);
    }

    auto cmp = [](const StudentVector& a, const StudentVector& b) {
        if (a.finalAvg() != b.finalAvg()) return a.finalAvg() < b.finalAvg();
        if (a.surname() != b.surname()) return a.surname() < b.surname();
        return a.name() < b.name();
    };

    std::sort(lt5.begin(), lt5.end(), cmp);
    std::sort(ge5.begin(), ge5.end(), cmp);
}

void writeResultsFile(const std::string& filename, const std::vector<StudentVector>& v) {
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

struct RunTimes {
    double read = 0.0;
    double splitSort = 0.0;
    double write = 0.0;
    std::size_t lt = 0;
    std::size_t ge = 0;
};

RunTimes runVector(const std::string& inFile, const std::string& outLt5, const std::string& outGe5) {
    RunTimes r;

    const auto t0 = Clock::now();
    auto students = readStudentsFromFile(inFile);
    const auto t1 = Clock::now();

    std::vector<StudentVector> lt5, ge5;
    const auto t2 = Clock::now();
    splitAndSortByFinalAvg(students, lt5, ge5);
    const auto t3 = Clock::now();

    const auto t4 = Clock::now();
    writeResultsFile(outLt5, lt5);
    writeResultsFile(outGe5, ge5);
    const auto t5 = Clock::now();

    r.read = seconds(t1 - t0);
    r.splitSort = seconds(t3 - t2);
    r.write = seconds(t5 - t4);
    r.lt = lt5.size();
    r.ge = ge5.size();
    return r;
}

RunTimes runDeque(const std::string& inFile, const std::string& outLt5, const std::string& outGe5) {
    RunTimes r;
    StudentDeque sd;

    const auto t0 = Clock::now();
    sd.readFromFile(inFile);
    const auto t1 = Clock::now();

    std::deque<StudentVector> lt5, ge5;
    const auto t2 = Clock::now();
    sd.splitAndSortByFinalAvg(lt5, ge5);
    const auto t3 = Clock::now();

    const auto t4 = Clock::now();
    sd.writeResultsFile(outLt5, lt5);
    sd.writeResultsFile(outGe5, ge5);
    const auto t5 = Clock::now();

    r.read = seconds(t1 - t0);
    r.splitSort = seconds(t3 - t2);
    r.write = seconds(t5 - t4);
    r.lt = lt5.size();
    r.ge = ge5.size();
    return r;
}

RunTimes runList(const std::string& inFile, const std::string& outLt5, const std::string& outGe5) {
    RunTimes r;
    StudentList sl;

    const auto t0 = Clock::now();
    sl.readFromFile(inFile);
    const auto t1 = Clock::now();

    std::list<StudentVector> lt5, ge5;
    const auto t2 = Clock::now();
    sl.splitAndSortByFinalAvg(lt5, ge5);
    const auto t3 = Clock::now();

    const auto t4 = Clock::now();
    sl.writeResultsFile(outLt5, lt5);
    sl.writeResultsFile(outGe5, ge5);
    const auto t5 = Clock::now();

    r.read = seconds(t1 - t0);
    r.splitSort = seconds(t3 - t2);
    r.write = seconds(t5 - t4);
    r.lt = lt5.size();
    r.ge = ge5.size();
    return r;
}

}

int main(int argc, char* argv[]) {
    try {
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);

        std::size_t n = 0;
        if (argc >= 2) {
            try {
                n = static_cast<std::size_t>(std::stoull(argv[1]));
            } catch (...) {
                n = 0;
            }
        }
        if (n == 0) n = readCount("Iveskite generuojamu studentu skaiciu (pvz. 100000): ", 1000);
        if (n == 0) throw std::runtime_error("Studentu skaicius turi buti > 0");

        constexpr int hwCount = 5;

        const std::string inFile = inputFilename(n);
        const std::string outLt5Vec = outFilenameLT5(n, "vector");
        const std::string outGe5Vec = outFilenameGE5(n, "vector");
        const std::string outLt5Deq = outFilenameLT5(n, "deque");
        const std::string outGe5Deq = outFilenameGE5(n, "deque");
        const std::string outLt5List = outFilenameLT5(n, "list");
        const std::string outGe5List = outFilenameGE5(n, "list");

        const auto t0 = Clock::now();
        generateStudentsFile(inFile, n, hwCount);
        const auto t1 = Clock::now();

        auto vec = runVector(inFile, outLt5Vec, outGe5Vec);
        auto deq = runDeque(inFile, outLt5Deq, outGe5Deq);
        auto lst = runList(inFile, outLt5List, outGe5List);

        std::cout << "Sugeneruota: " << n << " studentu\n";
        std::cout << "Ivesta: " << inFile << "\n\n";

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Failo generavimas: " << seconds(t1 - t0) << " s\n\n";

        std::cout << std::left
                  << std::setw(10) << "Tipas"
                  << std::right
                  << std::setw(14) << "Read(s)"
                  << std::setw(18) << "Split+Sort(s)"
                  << std::setw(14) << "Write(s)"
                  << std::setw(10) << "<5"
                  << std::setw(10) << ">=5"
                  << "\n";
        std::cout << std::string(74, '-') << "\n";

        auto printRow = [](const std::string& tag, const RunTimes& r) {
            std::cout << std::left << std::setw(10) << tag
                      << std::right
                      << std::setw(14) << r.read
                      << std::setw(18) << r.splitSort
                      << std::setw(14) << r.write
                      << std::setw(10) << r.lt
                      << std::setw(10) << r.ge
                      << "\n";
        };

        printRow("vector", vec);
        printRow("deque", deq);
        printRow("list", lst);

        std::cout << "\nIsvedimo failai:\n";
        std::cout << "vector: " << outLt5Vec << ", " << outGe5Vec << "\n";
        std::cout << "deque:  " << outLt5Deq << ", " << outGe5Deq << "\n";
        std::cout << "list:   " << outLt5List << ", " << outGe5List << "\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Klaida: " << e.what() << "\n";
        return 1;
    }
}