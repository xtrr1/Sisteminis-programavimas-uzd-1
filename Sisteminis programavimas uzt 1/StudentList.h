#pragma once

#include "StudentVector.h"

#include <cstddef>
#include <list>
#include <string>

class StudentList {
private:
    std::list<StudentVector> students_;

public:
    void readFromFile(const std::string& filename);
    void splitAndSortByFinalAvg(std::list<StudentVector>& lt5, std::list<StudentVector>& ge5) const;
    void writeResultsFile(const std::string& filename, const std::list<StudentVector>& v) const;
    std::size_t size() const { return students_.size(); }
};
