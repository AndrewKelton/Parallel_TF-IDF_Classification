#include "TFIDF.hpp"

void TFIDF::TFIDF_::process_trained_data() {

    /* Read in trained data from CSV file */
    try {
        read_csv_to_corpus(std::ref(trained_corpus), input_files.trained_input_file);
    } catch (std::runtime_error e) {
        handle_err("Error reading: " + input_files.trained_input_file + " " + std::string(e.what()));
        return;
    }

    /* -- Vectorize Documents Section -- */
    timer.start_timer();

    if (task_settings.is_parallel) {
        try {
            vectorize_corpus_threaded(&trained_corpus);
        } catch (std::exception e) {
            handle_err("Error in vectorize_corpus_parallel: " + std::string(e.what()));
            return;
        }
    } else {
        try {
            vectorize_corpus_sequential(&trained_corpus);
        } catch (std::exception &e) {
            handle_err("Error in vectorize_corpus_sequential: " + std::string(e.what()));
            return;
        }
    }

    timer.end_timer();
    if (task_settings.output_performance)
        print_duration_code(timer.duration, vectorization_);
    /* -- Vectorize Documents Section END -- */


    /* -- Calculate TF-IDF Section -- */
    timer.start_timer();

    if (task_settings.is_parallel) {
        try {
            trained_corpus.tfidf_documents();
        } catch (std::exception &e) {
            handle_err("Error in tfidf_documents: " + std::string(e.what()));
            return;
        }
    } else {
        try {
            trained_corpus.tfidf_documents_seq();
        } catch (std::exception &e) {
            handle_err("Error in tfidf_documents_seq: " + std::string(e.what()));
            return;
        }
    }
    
    timer.end_timer();
    if (task_settings.output_performance)
        print_duration_code(timer.duration, tfidf_);
    /* -- Calculate TF-IDF Section END -- */


    /* -- Category Section -- */
    timer.start_timer();

    if (task_settings.is_parallel) {
        try {
            trained_cat_vect = cats::par::get_all_cat_par(trained_corpus);
        } catch (std::exception &e) {
            handle_err("Error in get_all_cat_par: " + std::string(e.what()));
            return;
        }
    } else {
        try {
            trained_cat_vect = cats::seq::get_all_cat_seq(trained_corpus);
        } catch (std::exception &e) {
            handle_err("Error in get_all_cat_seq: " + std::string(e.what()));
            return;
        }
    }

    timer.end_timer();
    if (task_settings.output_performance)
        print_duration_code(timer.duration, categories_);
    /* -- Category Section END -- */
}

void TFIDF::TFIDF_::process_un_trained_data() {

    /* Read in the untrained/unknown text */
    try {
        read_unknown_text(std::ref(un_trained_corpus), input_files.un_trained_input_file);
    } catch (std::runtime_error &e) {
        handle_err("Error in read_unknown_text: " + std::string(e.what()));
        return;
    }

    timer.start_timer();

    if (task_settings.is_parallel) {
        try {
            vectorize_corpus_threaded(&un_trained_corpus);
        } catch (std::exception &e) {
            handle_err("Error in vectorize_corpus_threaded: " + std::string(e.what()));
            return;
        }
    } else {
        try {
            vectorize_corpus_sequential(&un_trained_corpus); // sequential vectorization
        } catch (std::exception &e) {
            handle_err("Error in vectorize_corpus_sequential: " + std::string(e.what()));
            return;
        }
    }

    if (task_settings.is_parallel) {
        try {
            un_trained_corpus.tfidf_documents();
        } catch (std::exception &e) {
            handle_err("Error in tfidf_documents: " + std::string(e.what()));
            return;
        }
    } else {
        try {
            un_trained_corpus.tfidf_documents_seq();
        } catch (std::exception &e) {
            handle_err("Error in tfidf_documents: " + std::string(e.what()));
            return;
        }
    }

    if (task_settings.classify_unknown) {
        try {
            un_trained_cats_correct = read_unknown_cats();
        } catch (std::runtime_error &e) {
            handle_err("Error in read_unknown_cats: " + std::string(e.what()));
            return;
        }

        if (task_settings.is_parallel) {
            try {
                cats::par::init_classification_par(std::ref(un_trained_corpus), std::ref(trained_cat_vect), un_trained_cats_correct);
            } catch (std::exception &e) {
                handle_err("Error in init_classification_par: " + std::string(e.what()));
                return;
            }
        } else {
            try {
                cats::seq::init_classification_seq(std::ref(un_trained_corpus), trained_cat_vect, std::ref(un_trained_cats_correct));
            } catch (std::exception &e) {
                handle_err("Error in init_classification_par: " + std::string(e.what()));
                return;
            }
        }

        timer.end_timer();

        if (task_settings.output_performance)
            print_duration_code(timer.duration, unknown_);

        if (task_settings.output_classification)
            cats::print_classifications();

        if (task_settings.convert_output_to_csv) {
            try {
                convert_results_txt_to_csv(0, true);
            } catch (std::runtime_error &e) {
                handle_err("Error in convert_results_txt_to_csv: " + std::string(e.what()));
                return;
            }
        }

    } else {
        timer.end_timer();
        if (task_settings.output_performance)
            print_duration_code(timer.duration, unknown_);
    }
}   

void TFIDF::TFIDF_::process_all_data() {
    process_trained_data();
    process_un_trained_data();
}

void TFIDF::TFIDF_::handle_err(std::string to_cerr) {
    if (task_settings.is_base_lvl_logging) 
        std::cerr << to_cerr << std::endl;
    return;
}
