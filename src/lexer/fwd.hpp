#ifndef LEXER_FWD_HPP
#define LEXER_FWD_HPP
#include "rxmap.hpp"
#include "tokenid.hpp"

namespace corrosion
{
//! @brief Alias declaration for the rustc token map.
using TokenMap = RxMap<TokenID>;
} // namespace corrosion
#endif // LEXER_FWD_HPP
