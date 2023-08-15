/***********************************
 * File:     Version.h
 *
 * Author:   caipeng
 *
 * Email:    iiicp@outlook.com
 *
 * Date:     2023/2/4
 *
 * Sign:     enjoy life
 ***********************************/
#ifndef LCC_VERSION_H
#define LCC_VERSION_H

#include "clang/Basic/Version.inc"
#include <string>

namespace clang {
std::string getClangVersion();
} // namespace clang

#endif // LCC_VERSION_H
