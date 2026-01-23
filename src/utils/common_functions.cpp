#include "utils/common_functions.hpp" 
namespace cc::utils{
bool isBarCodeDigit(std::string_view s){
        return !s.empty()&&std::ranges::all_of(s,[](unsigned char c){return std::isdigit(c);});
    }
}

