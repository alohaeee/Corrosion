#ifndef LEXER_FWD_HPP
#define LEXER_FWD_HPP
#include "rxmap.hpp"
#include "tokenid.hpp"

namespace rustc
{
//! @brief Alias declaration for the rustc token map.
using TokenMap = RxMap<TokenID>;
} // namespace rustc
#endif // LEXER_FWD_HPP
