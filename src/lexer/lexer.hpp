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

    std::pair<int, Token> LongestTerminate(const QString &text, int begin)
    {
        int result = 0;

        auto type = m_terminates[text.mid(begin, text.size() - begin)];
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
        CORROSION_ASSERT(m_terminates.Data().size() && m_packeds.Data().size());

        for (int end = 0, begin = 0; end < text.size(); end++)
        {
            auto [result, token] = LongestTerminate(text, end);
            if (result)
            {
                if (token.type != IGNORE)
                {
                    m_queue.push_back(token);
                }

                if (end != 0)
                {
                    auto lexem = text.mid(begin, end - begin);

                    if (!lexem.isEmpty())
                    {
                        auto [cap, type] = m_packeds.Exact(lexem);
                        if (type != IGNORE)
                        {
                            m_queue.push_back({lexem, type});
                        }
                    }
                }

                end += result - 1;
                begin = end + 1;
            }
            else if (end == text.size() - 1)
            {
                auto lexem = text.mid(begin, end - begin + 1);

                if (!lexem.isEmpty())
                {
                    auto [cap, type] = m_packeds.Exact(lexem);
                    if (type != IGNORE)
                    {
                        m_queue.push_back({lexem, type});
                    }
                }
            }
        }
    }

    /**
     * @brief Check wheter the token's queue empty.
     * @return True if queue is empty, false otherwise.
     */
    inline bool isEmpty() const CORROSION_NOEXCEPT
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
    const Token &Look() const CORROSION_NOEXCEPT
    {
        return m_queue.head();
    }
    /**
     * @brief Look at second token in queue.
     * @return Reference to a second token in queue.
     */
    const Token &Next() const CORROSION_NOEXCEPT
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
    void Log() const CORROSION_NOEXCEPT
    {
        for (auto &token : m_queue)
        {
            qDebug(qUtf8Printable(token.lexem + " - " + QString::number((int)token.type)));
        }
    }

    /**
     * @brief Log into stdcerr "{lexem} - {type}" for each lexem.
     */
    void LogErr() const CORROSION_NOEXCEPT
    {
        for (auto &token : m_queue)
        {
            std::cerr << QString(token.lexem + " - " + QString::number((int)token.type)).toStdString() << std::endl;
        }
    }
    /**
     * @brief Clear the queue.
     */
    void Clear() CORROSION_NOEXCEPT
    {
        m_queue.clear();
    }
    /**
     * @brief Return list of tokens and clear queue.
     */
    auto ToList() CORROSION_NOEXCEPT
    {
        QList<Token> list{m_queue};
        m_queue.clear();
        return list;
    }

protected:
    QQueue<Token> m_queue{};
    RxMap<Type> m_terminates{};
    RxMap<Type> m_packeds{};
}; // namespace rustc

} // namespace corrosion

#endif // LEXER_HPP
