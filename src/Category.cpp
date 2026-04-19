/* Category.cpp
 * source file for Category.hpp
 */

#include "Category.hpp"

std::string Category::get_type() {
    return category_type;
}

std::vector<double> Category::get_centroid() {
    return centroid;
}