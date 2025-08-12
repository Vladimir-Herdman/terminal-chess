#ifndef TERMINALCHESS_TYPES_CLASSES_H
#define TERMINALCHESS_TYPES_CLASSES_H

#include <cstdint>

template <typename CLASS>
using VoidMemberFuncPtr = void (CLASS::*)();

template <typename CLASS>
using U64MemberFuncPtrConst = uint64_t (CLASS::*)() const;

template <typename CLASS>
using U64MemberFuncIntPtrConst = uint64_t (CLASS::*)(const int) const;

template <typename CLASS>
using U64MemberVarPtr = uint64_t CLASS::*;

#endif //TERMINALCHESS_TYPES_CLASSES_H
