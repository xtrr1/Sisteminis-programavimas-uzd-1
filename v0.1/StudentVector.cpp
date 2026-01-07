#include "StudentVector.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {
    constexpr double HW_WEIGHT = 0.4;
    constexpr double EXAM_WEIGHT = 0.6;

    bool isValidGrade(int x) { return x >= 0 && x <= 10; }

    double computeFinal(double hwBase, int exam) {
        return HW_WEIGHT * hwBase + EXAM_WEIGHT * static_cast<double>(exam);
    }
}

void StudentVector::setNameSurname(const std::string& name, const std::string& surname) {
    name_ = name;
    surname_ = surname;
}

void StudentVector::setNd(const std::vector<int>& nd) {
    for (int v : nd) {
        if (!isValidGrade(v)) {
            throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
        }
    }
    nd_ = nd;
}

void StudentVector::setExam(int exam) {
    if (!isValidGrade(exam)) {
        throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");
    }
    exam_ = exam;
}

double StudentVector::ndAverage() const {
    if (nd_.empty()) return 0.0;
    long sum = 0;
    for (int v : nd_) sum += v;
    return static_cast<double>(sum) / static_cast<double>(nd_.size());
}

double StudentVector::ndMedian() const {
    if (nd_.empty()) return 0.0;

    auto tmp = nd_;
    std::sort(tmp.begin(), tmp.end());

    const std::size_t mid = tmp.size() / 2;
    if (tmp.size() % 2 == 1) return static_cast<double>(tmp[mid]);
    return (static_cast<double>(tmp[mid - 1]) + static_cast<double>(tmp[mid])) / 2.0;
}

void StudentVector::computeFinals() {
    finalAvg_ = computeFinal(ndAverage(), exam_);
    finalMed_ = computeFinal(ndMedian(), exam_);
}

std::istream& operator>>(std::istream& in, StudentVector& s) {

    std::string name, surname;
    if (!(in >> name >> surname)) return in;

    std::string rest;
    std::getline(in >> std::ws, rest);

    std::stringstream ss(rest);
    std::vector<int> nums;
    int x;
    while (ss >> x) nums.push_back(x);

    if (nums.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    const int exam = nums.back();
    nums.pop_back();

    for (int v : nums) {
        if (!isValidGrade(v)) throw std::runtime_error("Netinkamas ND pazymys (turi buti 0..10)");
    }
    if (!isValidGrade(exam)) throw std::runtime_error("Netinkamas egzamino pazymys (turi buti 0..10)");

    s.name_ = name;
    s.surname_ = surname;
    s.nd_ = std::move(nums);
    s.exam_ = exam;

    return in;
}

std::ostream& operator<<(std::ostream& out, const StudentVector& s) {
    out << std::left
        << std::setw(18) << s.name_
        << std::setw(20) << s.surname_
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(16) << s.finalAvg_
        << std::setw(16) << s.finalMed_;
    return out;
}