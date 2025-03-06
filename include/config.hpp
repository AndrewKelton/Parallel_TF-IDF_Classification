/* config.hpp
 * 
 * Contains default feature flags to generate .txt
 * files containing data generated from sections of
 * the code. By default ENABLE_LENGTHY is set to '1'
 * to output all data included to the directory:
 * "/test-output/lengthy".
 */


#ifndef CONFIG_HPP
#define CONFIG_HPP

// outputs all data
#ifndef ENABLE_LENGTHY
#define ENABLE_LENGTHY 1
#endif

// outputs all TF-IDF info of corpus 
#ifndef ENABLE_TERMS_INFO
#define ENABLE_TERMS_INFO 0
#endif

// outputs all categories info
#ifndef ENABLE_CATS_INFO
#define ENABLE_CATS_INFO 0
#endif

#endif 