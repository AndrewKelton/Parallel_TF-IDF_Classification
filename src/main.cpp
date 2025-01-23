#include "document.h"
#include "count_vectorization.h"
#include "preprocess.h"

int main() {

    Document doc;
    doc.text = "Hello, this is a test";
    doc.total_terms = 5;

    preprocess_text(&doc);
    count_words_doc(&doc);

    cout << doc.text << endl << endl;

    for (const auto& [word, count] : doc.term_count)
        cout << word << ": " << count << endl;


    // Corpus corp;
   

    return 0;
}