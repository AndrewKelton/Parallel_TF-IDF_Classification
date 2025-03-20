/* preprocess.cpp
 * source file for preprocess.hpp
 */

#include "preprocess.hpp"
#include "english_stem.h"
#include "utils.hpp"
#include <locale>
#include <codecvt>


/* Helper for static_assert to trigger an error 
 * for unsupported types. See comment below for 
 * use of chatGPT.
 */
template <typename T>
struct always_false : std::false_type {};
template <typename InputType>

/* Convert std::string to wstring and wstring to std::string.
 * Used when stemming words to their base forms.
 * 
 * NOTE: I received help from chatGPT to write this function.
 *      The parameters given were "In c++, how can you create
 *      a function that can return different dataTypes and can
 *      accept different dataTypes, without relying on a tuple, 
 *      map, or set?"
 * 
 * Result: chatGPT outputted the following code segments:
 *      template <typename T>
 *      template <typename InputType> // allows any input (shown above this multiline-comment)
 *      auto convert_string_wstring(const InputType& input) {...} // auto allows the return of any value
 */
static auto convert_string_wstring(const InputType& input) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    if constexpr (std::is_same<InputType, std::wstring>::value) 
        return converter.to_bytes(input); // convert wstring to std::string
    else if constexpr (std::is_same<InputType, std::string>::value) 
        return converter.from_bytes(input); // convert std::string to std::wstring
    else 
        throw std::runtime_error("Bad Cast in convert_string_wstring");
}

// convert uppercase text to lower case text
static std::string preprocess_to_lower_text(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return tolower(c); });

    return str;
}

// remove punctuation and numbers from text
static std::string preprocess_remove_punc_text(std::string str) {
    std::string processed = "";

    for (char c : str)
        if (!ispunct(c) && !isdigit(c))
            processed += c;

    return processed;
}

extern std::string preprocess_prune_term(std::string str) {
    std::wstring to_prune;

    try {
        to_prune = convert_string_wstring(str);
    } catch(const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return str;
    }

    // prune it
    stemming::english_stem<> stemmer;
    stemmer(to_prune);

    return convert_string_wstring(to_prune);
}

// preprocess all text in document
extern void preprocess_text(docs::Document * doc) {
    doc->text = preprocess_to_lower_text(preprocess_remove_punc_text(doc->text));
}