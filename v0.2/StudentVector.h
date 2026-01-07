#pragma once

#include <iosfwd>
#include <string>
#include <vector>
#include <cstddef>

class StudentVector {
private:
    std::string name_;
    std::string surname_;
    std::vector<int> nd_;
    int exam_ = 0;

    double finalAvg_ = 0.0;
    double finalMed_ = 0.0;

public:
    StudentVector() = default;

    void setNameSurname(const std::string& name, const std::string& surname);
    void setNd(const std::vector<int>& nd);
    void setExam(int exam);

    double ndAverage() const;
    double ndMedian() const;
    void computeFinals();

    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }
    int exam() const { return exam_; }
    std::size_t ndCount() const { return nd_.size(); }
    double finalAvg() const { return finalAvg_; }
    double finalMed() const { return finalMed_; }

    friend std::istream& operator>>(std::istream& in, StudentVector& s);
    friend std::ostream& operator<<(std::ostream& out, const StudentVector& s);
};