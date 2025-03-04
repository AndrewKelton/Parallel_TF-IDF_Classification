#ifndef PRINTABLE_H
#define PRINTABLE_H

#include <iostream>

class Corpus;
class Document;
class Category;

class Printable {
    public:
        virtual ~Printable() = default;
        virtual void print_all_info() const = 0;
};

#endif