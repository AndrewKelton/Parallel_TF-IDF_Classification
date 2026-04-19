/**
 * @file tfidf.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the TFIDF_ class, which implements methods for processing trained and untrained data
 *        and calculating TF-IDF scores for text classification tasks.
 */

#ifndef _TFIDF_HPP
#define _TFIDF_HPP

#include "Corpus.hpp"

class tfidf {
    public:
        void fit(std::string path);

        void transform(std::string path);

        void classify();

    private:
        corpus::Corpus training_corpus;
        corpus::Corpus testing_corpus;
};

#endif // _TFIDF_HPP
