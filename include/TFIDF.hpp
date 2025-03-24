#ifndef _TFIDF_HPP
#define _TFIDF_HPP

#include "count_vectorization.hpp"
#include "file_operations.hpp"
#include "flag_handler.hpp"


namespace TFIDF { // namespace TFIDF

    class TFIDF_ {

        public:
            corpus::Corpus trained_corpus;
            std::vector<cats::Category> trained_cat_vect;
            corpus::Corpus un_trained_corpus;
            std::vector<std::string> un_trained_cats_correct;

            struct Timer {
                std::chrono::_V2::system_clock::time_point start;
                std::chrono::_V2::system_clock::time_point end;
                double duration;

                void start_timer() {
                    start = std::chrono::high_resolution_clock::now();
                }

                void end_timer() {
                    end = std::chrono::high_resolution_clock::now();
                    duration = elapsed_time_ms(start, end);
                }
            };
            Timer timer;

            /* Contructors */
            TFIDF_(bool is_parallel=true, 
                   std::string trained_input_file=DEFAULT_TRAINED_INPUT_FILE, 
                   std::string un_trained_input_file=DEFAULT_UN_TRAINED_INPUT_FILE, 
                   std::string un_trained_correct_classification_file=DEFAULT_UN_TRAINED_CORRECT_INPUT_FILE, 
                   std::string processed_data_csv_file=DEFAULT_PROCESSED_DATA_OUTPUT_CSV_FILE,
                   bool complete_all_tasks=true, 
                   bool classify_unknown=true,
                   bool record_performance=true,
                   bool output_performance=true,
                   bool output_classification=true,
                   bool convert_output_to_csv=true,
                   bool is_base_lvl_logging=true,
                   int num_threads=-1 /* leave as '-1' for dynamic # of threads ** NOT USED CURRENTLY, FUTURE FEATURE ** */
                  ) 
                : task_settings{is_parallel, 
                              (un_trained_input_file.empty() || un_trained_input_file == "") ? false : complete_all_tasks,
                              classify_unknown, record_performance, (record_performance) ? output_performance : false,
                              output_classification, (output_classification && output_performance) ? convert_output_to_csv : false, is_base_lvl_logging,
                              (is_parallel == false) ? 1 : num_threads
                             },
                input_files{trained_input_file, 
                              un_trained_input_file, 
                              un_trained_correct_classification_file, 
                              processed_data_csv_file
                             }
            {}


            void process_trained_data();

            void process_un_trained_data();

            void process_all_data();

        private:
            struct TaskSettings {
                bool is_parallel; // executes parallel implementation
                bool complete_all_tasks; // trained/untrained ops
                bool classify_unknown; 
                bool record_performance;
                bool output_performance; // time output
                bool output_classification;
                bool convert_output_to_csv;
                bool is_base_lvl_logging; // logs errors with std::cerr
                int num_threads; // if -1 then dynamic # threads

                enum _TaskType {
                    _START_PERFORMANCE=0x02,
                    _END_PERFORMANCE=0x03,
                    _OUTPUT_PERFORMANCE=0x04,
                    _BASE_LVL_LOGGING=0x05
                };
            };
            TaskSettings task_settings;

            struct InputFiles {
                std::string trained_input_file;
                std::string un_trained_input_file;
                std::string un_trained_correct_classification_file;
                std::string processed_data_csv_file;
            };
            InputFiles input_files;

            void handle_err(std::string to_cerr); 
    };
}


#endif // _TFIDF_HPP