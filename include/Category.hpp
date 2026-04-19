/**
 * @file Category.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the Category class and its related functions.
 */

#ifndef _CATEGORY_HPP
#define _CATEGORY_HPP

#include <string>
#include <vector>

class Category {

    public:

        Category(std::string category_type, std::vector<double> centroid) {
            this->category_type = category_type;
            this->centroid = centroid;
        }

        std::string get_type();

        double similarity(const std::vector<double>& doc_row);

        std::vector<double> get_centroid();

        void print_all_info();

    private:
        std::string category_type;
        std::vector<double> centroid;
};

#endif