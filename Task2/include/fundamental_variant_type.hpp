#ifndef PRIMITIVE_VARIANT_TYPE_HPP
#define PRIMITIVE_VARIANT_TYPE_HPP

#include "variant_type.hpp"

using FundamentalVariantType = VariantType<bool, char, wchar_t, int, 
                                           short, long, float, double, 
                                           unsigned char, unsigned short,
                                           unsigned int, unsigned long>;

#endif