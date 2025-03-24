/**
 * @file TFIDF.hpp
 * @ingroup HeaderFiles
 * 
 * @author Andrew Kelton
 * @brief Defines the TFIDF_ class, which implements methods for processing trained and untrained data
 *        and calculating TF-IDF scores for text classification tasks.
 * 
 * @version 1.0
 * @date 2025-03-24
 * 
 * @par Changelog:
 * - Initial version with basic functionality for TF-IDF processing.
 */

#ifndef _TFIDF_HPP
#define _TFIDF_HPP

#include "count_vectorization.hpp"
#include "file_operations.hpp"
#include "flag_handler.hpp"


namespace TFIDF { // namespace TFIDF

    /**
     * @class TFIDF_
     * @brief The main class for handling TF-IDF calculations, training data processing, 
     *        and untrained data classification.
     * 
     * @details This class implements the logic for processing both trained and untrained data
     *          related to TF-IDF computations. It includes functionality for performing the 
     *          classification of untrained documents, calculating TF-IDF scores, and 
     *          managing various tasks such as performance monitoring and logging.
     * 
     * It contains methods for:
     * - `process_trained_data()`: Processes the trained corpus.
     * - `process_un_trained_data()`: Processes the untrained corpus.
     * - `process_all_data()`: Processes both trained and untrained data.
     * 
     * @note This class can be configured to run in parallel or sequentially based on the user settings.
     */
    class TFIDF_ {

        public:
            corpus::Corpus trained_corpus;
            std::vector<cats::Category> trained_cat_vect;
            corpus::Corpus un_trained_corpus;
            std::vector<std::string> un_trained_cats_correct;

            /**
             * @struct Timer
             * @brief A structure used for measuring performance during the TF-IDF computation.
             * 
             * @details The `Timer` struct helps in timing how long certain operations take during 
             *          the TF-IDF computation, allowing performance metrics to be collected.
             */
            struct Timer {
                std::chrono::_V2::system_clock::time_point start;
                std::chrono::_V2::system_clock::time_point end;
                double duration;

                /**
                 * @brief Starts the timer.
                 */
                void start_timer() {
                    start = std::chrono::high_resolution_clock::now();
                }
                
                /**
                 * @brief Ends the timer and calculates the duration.
                 */
                void end_timer() {
                    end = std::chrono::high_resolution_clock::now();
                    duration = elapsed_time_ms(start, end);
                }
            };
            Timer timer;

            /**
             * @brief Constructs a TFIDF_ object with user-defined configuration settings.
             * 
             * @param is_parallel Whether to run the computations in parallel (default: true).
             * @param trained_input_file The input file for trained data.
             * @param un_trained_input_file The input file for untrained data.
             * @param un_trained_correct_classification_file The correct classifications for untrained data.
             * @param processed_data_csv_file Output CSV file for processed data.
             * @param complete_all_tasks Whether to process all tasks (default: true).
             * @param classify_unknown Whether to classify unknown documents (default: true).
             * @param record_performance Whether to record performance data (default: true).
             * @param output_performance Whether to output performance data (default: true).
             * @param output_classification Whether to output classifications (default: true).
             * @param convert_output_to_csv Whether to convert output to CSV (default: true).
             * @param is_base_lvl_logging Whether to log errors to stderr (default: true).
             * @param num_threads Number of threads for parallel processing (default: -1 for dynamic threads).
             */
            TFIDF_(bool is_parallel=true, 
                   std::string trained_input_file=DEFAULT_TRAINED_INPUT_FILE, 
                   std::string un_trained_input_file=DEFAULT_UN_TRAINED_INPUT_FILE, 
                   std::string un_trained_correct_classification_file=DEFAULT_UN_TRAINED_CORRECT_INPUT_FILE, 
                   std::string output_results_file=DEFAULT_OUTPUT_RESULTS_TXT_FILE,
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
                              output_results_file,
                              processed_data_csv_file
                             }
            {}

            /**
             * @brief Processes the trained data and classifies documents to their respective Category.
             */
            void process_trained_data();

            /**
             * @brief Processes the untrained data and classifies it with the trained data.
             */
            void process_un_trained_data();

            /**
             * @brief Processes both trained and untrained data.
             */
            void process_all_data();

        private:

            /**
             * @struct TaskSettings
             * @brief Configuration settings related to the task, such as parallel processing and output options.
             */
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

            /**
             * @struct InputFiles
             * @brief Holds the file paths for input and output files.
             */
            struct InputFiles {
                std::string trained_input_file;
                std::string un_trained_input_file;
                std::string un_trained_correct_classification_file;
                std::string output_results_file;
                std::string processed_data_csv_file;
            };
            InputFiles input_files;

            /**
             * @brief Handles errors by logging to stderr.
             * @param to_cerr The error message to log.
             */
            void handle_err(std::string to_cerr); 
    };
}

#endif // _TFIDF_HPP