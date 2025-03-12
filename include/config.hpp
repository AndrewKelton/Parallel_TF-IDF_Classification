/**
 * @file config.hpp
 * @compiler g++ -std=c++17 
 * 
 * @author Andrew Kelton
 * @brief Contains default flag features for controlling the generation of output files.
 * 
 * @details This file defines default feature flags used to control the generation of `.txt` files
 * containing data generated from various sections of the code. 
 * 
 * By default, all flags are set to `0`, meaning output generation is **disabled** unless explicitly enabled
 * 
 * - `ENABLE_LENGTHY` (default: 0) → Enables detailed output in `/test-output/lengthy/`.  
 * - `ENABLE_TERMS_INFO` (default: 0) → Enables TF-IDF term information output.  
 * - `ENABLE_CATS_INFO` (default: 0) → Enables document category output.  
 * 
 * To enable a feature, modify the flag definition or use `-D<FLAG>=1` during compilation.
 * 
 * @version 0.1
 * @date 2025-03-12
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

/** @brief Enables detailed output in `/test-output/lengthy/`. Default: Disabled (`0`). */
#ifndef ENABLE_LENGTHY
#define ENABLE_LENGTHY 0
#endif

/** @brief Enables output of TF-IDF term information for the corpus. Default: Disabled (`0`). */
#ifndef ENABLE_TERMS_INFO
#define ENABLE_TERMS_INFO 0
#endif

/** @brief Enables output of document category information. Default: Disabled (`0`). */
#ifndef ENABLE_CATS_INFO
#define ENABLE_CATS_INFO 0
#endif

#endif // CONFIG_HPP