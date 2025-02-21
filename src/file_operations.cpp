#include "file_operations.h"

// return pair of first split from csv 'category, text'
static pair<string, string> split_string_comma(const string& str) {
  size_t comma_pos = str.find(',');
  if (comma_pos == string::npos)
    return {str, ""};

  return {str.substr(0, comma_pos), str.substr(comma_pos + 1)};
}

// // return category enum from category string
// static TEXT_CATEGORY_TYPES get_category(string category) {
//     return categories_text.find(category)->second;
// }

// return a new document
static Document create_document(string text, TEXT_CATEGORY_TYPES category) {
    Document new_document;
    new_document.text = text;
    new_document.category = category;

    return new_document;
}

// read in csv to corpus/documents
extern void read_csv_to_corpus(Corpus& corpus, const string& file_name) {
    ifstream file(file_name);

    if (!file.is_open()) {
        exit(EXIT_FAILURE);
    }

    string line;
    int i = 0;
    while (getline(file, line)) {
        
        // ignore header
        if (line == "category,text")
            continue;

        pair<string, string> split = split_string_comma(line);
        TEXT_CATEGORY_TYPES category = get_category(split.first);
        
        corpus.documents.push_back(create_document(split.second, category));
    }   

    file.close();
}    

extern void write_to_csv_tfidf_corpus(Corpus corpus, const string& file_name) {
    
}