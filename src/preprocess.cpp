#include "preprocess.h"

// Helper for static_assert to trigger an error for unsupported types
template <typename T>
struct always_false : std::false_type {};

template <typename InputType>

// covert string to wstring and vice versa
static auto convert_string_wstring(const InputType& input) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

    if constexpr (is_same<InputType, wstring>::value) 
        return converter.to_bytes(input); // convert wstring to string
    else if constexpr (is_same<InputType, string>::value) 
        return converter.from_bytes(input); // cnvert string to wstring
    else 
        exit(ERR_CODES::BAD_CAST);
        // static_assert(always_false<InputType>::value, utils_error_msg("Bad Cast!", ERR_CODES::BAD_CAST));
}

// convert uppercase text to lower case text
static string preprocess_to_lower_text(string str) {
    transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c){ return tolower(c); });

    return str;
}

// remove punctuation and numbers from text
static string preprocess_remove_punc_text(string str) {
    string processed = "";

    for (char c : str)
        if (!ispunct(c) && !isdigit(c))
            processed += c;

    return processed;
}

/* prune text, such as: "enjoying" -> "enjoy" 
 * uses the OleanderStemmingLibrary by Blake Madden
 * URL: https://github.com/Blake-Madden/OleanderStemmingLibrary.git
 */
extern string preprocess_prune_term(string str) {
    wstring to_prune = convert_string_wstring(str);

    stemming::english_stem<> stemmer;
    stemmer(to_prune);

    return convert_string_wstring(to_prune);
}

// preprocess all text in document
void preprocess_text(Document * doc) {
    doc->text = preprocess_to_lower_text(preprocess_remove_punc_text(doc->text));
}