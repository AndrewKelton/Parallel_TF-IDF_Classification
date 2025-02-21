#include "document.h"
#include "categories.h"

bool Document::is_term(string str) {
    return term_count.find(str) != term_count.end();
}

double Document::calculate_term_frequency(string term) {
    if (total_terms == 0) 
        return 0.0;

    return static_cast<double>(term_count[term]) / total_terms;
}

void Document::calculate_term_frequency_doc() {
    for (auto& [word, count] : term_count) 
        term_frequency[word] = calculate_term_frequency(word);
}

double Corpus::idf_corpus(int docs_with_term) {
    return log(static_cast<double>(num_of_docs) / static_cast<double>(docs_with_term));
}

int Corpus::num_doc_term(string str) {
    int count{0};

    for (auto& d : documents) 
        if (d.is_term(str))
            count++;

    return count;
}

// Try threading this one 
void Corpus::tfidf_documents() {
    vector<future<void>> futures;

    for (auto& document: documents) {
        futures.emplace_back(async(
            launch::async, &Corpus::emplace_tfidf_document, this, &document
        ));
    }

    for (auto& f : futures) 
        f.get();

    // vector<thread> threads;

    // for (int i = 0; i < num_of_docs; i+=10) {
    //     threads.emplace_back([this, i]() {
    //         for (int x = 0; x < 10; x++) {
    //             emplace_tfidf_document(&documents[x+i]);
    //         }
    //     });
    // }

    // for (auto& document : documents) 
    //     threads.emplace_back(thread(&Corpus::emplace_tfidf_document, this, &document));

    // for (auto& t : threads)
    //     t.join();
}

// using a thread insert tfidf into document. one thread per document
void Corpus::emplace_tfidf_document(Document * document) {
    for (const auto& [word, freq] : document->term_frequency)
        document->tf_idf[word] = freq * idf_corpus(num_doc_term(word));

    // cout << "Document ID: " << document->document_id << " finished." << endl;
}