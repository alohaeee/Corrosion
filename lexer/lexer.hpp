#ifndef LEXER_HPP
#define LEXER_HPP

#include <QChar>
#include <QQueue>
#include <iostream>
#include <string>
#include <type_traits>

#include "../config.hpp"
#include "fwd.hpp"
namespace corrosion
{
/**
 * @brief Lexer is class for parsing lexems to tokens.
 * @tparam Type of token. For now supported only enumerators.
 */
template <typename Type, typename = std::enable_if_t<std::is_enum_v<Type>>>
class Lexer
{
    using underlying_type = std::underlying_type_t<Type>;
    struct Token
    {
        QString lexem;
        Type type;
    };

    std::pair<int, Token> LongestTerminate(int begin)
    {
        int result = 0;
        auto type = m_terminates[m_text.mid(begin, m_text.size() - begin - 1)];
        auto captured = m_terminates.RegExp().cap();
        if (type == m_terminates.Unknown())
        {
            result = 0;
        }
        else
        {
            result = captured.size();
        }
        return {result, {captured, type}};
    }

public:
    Lexer() = default;

    virtual ~Lexer() = default;

    /// @brief Ignore token value.
    static inline constexpr const Type IGNORE = Type(std::numeric_limits<underlying_type>::max());

    /**
     * @brief Separate text to tokens and store them into queue.
     * Greedy inside.
     * @param lexem Text to lex.
     */
    virtual void Lex(const QString &text)
    {
        RUSTC_ASSERT(m_terminates.Data().size() && m_packeds.Data().size());
        m_text = text;
        for (int begin = 0; begin < m_text.size(); begin++)
        {
            auto [result, token] = LongestTerminate(begin);
            if (result)
            {
                if (token.type != IGNORE)
                {
                    m_queue.push_back(token);
                }
                begin += result - 1;
            }
            else
            {
                int end = begin + 1;
                for (; end < m_text.size(); end++)
                {
                    auto [result, token] = LongestTerminate(end);
                    if (result)
                    {
                        auto lexem = m_text.mid(begin, end - begin);

                        auto [cap, type] = m_packeds.Exact(lexem);
                        if (type != IGNORE)
                        {
                            m_queue.push_back({lexem, type});
                        }
                        if (token.type != IGNORE)
                        {
                            m_queue.push_back(token);
                        }

                        break;
                    }
                }
                begin += result + end - begin;
            }
        }
        m_text.clear();
    }

    /**
     * @brief Check wheter the token's queue empty.
     * @return True if queue is empty, false otherwise.
     */
    inline bool isEmpty() const
    {
        return m_queue.isEmpty();
    }
    /**
     * @brief Dequeue first token from the queue.
     * @return First token in queue.
     */
    Token GetToken()
    {
        return m_queue.dequeue();
    }
    /**
     * @brief Look at first token in queue.
     * @return Reference to a first token in queue.
     */
    const Token &Look() const
    {
        return m_queue.head();
    }
    /**
     * @brief Look at second token in queue.
     * @return Reference to a second token in queue.
     */
    const Token &Next() const
    {
        return m_queue.begin() + 1;
    }

    /**
     * @brief Set terminate tokens.
     * @param tokenMap Map of tokens.
     */
    void SetTerminateTokens(const TokenMap &tokenMap)
    {
        m_terminates = tokenMap;
    }
    /**
     * @brief Set packed tokens.
     * @param tokenMap Map of tokens.
     */
    void SetPackedTokens(const TokenMap &tokenMap)
    {
        m_packeds = tokenMap;
    }

    /**
     * @brief Log into console "{lexem} - {type}" for each lexem.
     */
    void Log() const
    {
        foreach (auto &token, m_queue)
        {
            qDebug(qUtf8Printable(token.lexem + " - " + QString::number((int)token.type)));
        }
    }
    /**
     * @brief Clear the queue.
     */
    void Clear() const
    {
        m_queue.clear();
    }
    /**
     * @brief Return list of tokens and clear queue.
     */
    auto ToList()
    {
        QList<Token> list{m_queue};
        m_queue.clear();
        return list;
    }

protected:
    QString m_text{};
    QQueue<Token> m_queue{};
    TokenMap m_terminates{};
    TokenMap m_packeds{};
}; // namespace rustc

} // namespace rustc

#endif // LEXER_HPP
