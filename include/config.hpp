/* config.hpp
 * 
 * Contains default feature flags that can be overridden
 * by the Makefile. These features allow for all data 
 * generated to be outputted into .txt files.
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