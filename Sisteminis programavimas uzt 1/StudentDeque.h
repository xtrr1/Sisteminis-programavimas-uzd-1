#pragma once

#include "StudentVector.h"

#include <cstddef>
#include <deque>
#include <string>

class StudentDeque {
private:
    std::deque<StudentVector> students_;

public:
    void readFromFile(const std::string& filename);
    void splitAndSortByFinalAvg(std::deque<StudentVector>& lt5, std::deque<StudentVector>& ge5) const;
    void writeResultsFile(const std::string& filename, const std::deque<StudentVector>& v) const;
    std::size_t size() const { return students_.size(); }
};
