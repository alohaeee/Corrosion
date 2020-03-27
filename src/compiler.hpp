#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "lexer/lexer.hpp"
#include "lexer/rustlexerspec.hpp"
#include "parser/parser.hpp"
#include "utility/cexception.hpp"
#include "utility/filesystem.hpp"

namespace corrosion
{
class Compiler
{
    void InitLexer()
    {
        m_lexer.SetPackedTokens({TokenID::UNKNOWN,
                                 {
                                     {"while", TokenID::KW_WHILE},
                                     {"for", TokenID::KW_FOR},
                                     {"continue", TokenID::KW_CONTINUE},
                                     {"break", TokenID::KW_BREAK},
                                     {"let", TokenID::KW_LET},
                                     {"mut", TokenID::KW_MUT},
                                     {"if", TokenID::KW_IF},
                                     {"return", TokenID::KW_RETURN},
                                     {regexp::number_literals::INTEGER_SUFFIX, TokenID::INTEGER_SUFFIX},
                                     {regexp::number_literals::FLOAT_SUFFIX, TokenID::FLOAT_SUFFIX},
                                     {regexp::number_literals::INTEGER_LITERAL, TokenID::INTEGER_LITERAL},
                                     {regexp::number_literals::FLOAT_LITERAL, TokenID::FLOAT_LITERAL},
                                     {regexp::number_literals::BOOLEAN_LITERAL, TokenID::BOOLEAN_LITERAL},
                                     {regexp::IDENTIFIER, TokenID::IDENTIFIER},
                                 }});
        m_lexer.SetTerminateTokens({TokenID::UNKNOWN,
                                    {{"\\(", TokenID::LPARENTHESIS},
                                     {"\\)", TokenID::RPARENTHESIS},
                                     {"\\{", TokenID::LBRACER},
                                     {"\\}", TokenID::RBRACER},
                                     {"\\[", TokenID::LBRACKET},
                                     {"\\]", TokenID::RBRACKET},
                                     {"=", TokenID::EQ},
                                     {";", TokenID::SEMI},
                                     {":", TokenID::COLON},
                                     {"\\+", TokenID::PLUS},
                                     {"-", TokenID::MINUS},
                                     {"\\*", TokenID::STAR},
                                     {"/", TokenID::SLASH},
                                     {"\\+=", TokenID::PLUSEQ},
                                     {"-=", TokenID::MINUSEQ},
                                     {"\\*=", TokenID::STAREQ},
                                     {"/=", TokenID::SLASHEQ},
                                     {"\\&=", TokenID::ANDEQ},
                                     {"\\|=", TokenID::OREQ},
                                     {"\\&", TokenID::AND},
                                     {"\\&\\&", TokenID::ANDAND},
                                     {"\\|", TokenID::OR},
                                     {"\\|\\|", TokenID::OROR},
                                     {"!", TokenID::NOT},
                                     {">>", TokenID::SHR},
                                     {"<<", TokenID::SHL},
                                     {">", TokenID::GT},
                                     {"<", TokenID::LT},
                                     {">=", TokenID::GT},
                                     {"<=", TokenID::LE},
                                     {"!=", TokenID::NE},
                                     {"\\.", TokenID::DOT},

                                     {regexp::number_literals::FLOAT_LITERAL, TokenID::FLOAT_LITERAL},
                                     {regexp::string_literals::CHAR_LITERAL, TokenID::CHAR_LITERAL},
                                     {regexp::string_literals::STRING_LITERAL, TokenID::STRING_LITERAL},

                                     // ignore multi line comments
                                     {regexp::comments::BLOCK_COMMENT, Lexer<TokenID>::IGNORE},
                                     // ignore line comment
                                     {regexp::comments::LINE_COMMENT, Lexer<TokenID>::IGNORE},
                                     // ignore white space
                                     {regexp::whitespaces::WHITESPACE_REG, Lexer<TokenID>::IGNORE}}});
    }

    enum class State
    {
        Lex,
        Parse,
        Test,
        Waiting
    };

public:
    Compiler()
    {
        InitLexer();
    }
    ~Compiler() = default;
    void Command(const int argc, char **argv)
    {
        QStringList list{};
        for (int i = 0; i < argc; i++)
        {
            list.append(argv[i]);
        }
        Command(list);
    }
    void Command(const QStringList &list)
    {
        if (list.size() == 0)
        {
            throw CException("Input is empty!");
        }

        if (list[0] == "lex")
        {
            if (list.size() == 1)
            {
                throw CException("lex [file]!\nLexer need target.");
            }
            auto text = FileSystem::ReadAllFile(list[1]);
            m_lexer.Lex(text);

            m_lexer.Log();
            m_lexer.Clear();
        }
        else if (list[0] == "tests")
        {
            if (list.size() == 1)
            {
                throw CException("test [file]!\ntest need target.");
            }
        }
        else
        {
            qDebug("unknown");
        }
    }

    /**
     * @brief Return lexer.
     * @return Compiler's lexer instance.
     */
    auto &GetLexer() CORROSION_NOEXCEPT
    {
        return m_lexer;
    }

    void Simulate()
    {
        QTextStream stream(stdin);
        forever
        {
            auto list = stream.readLine().split(QRegExp(" +"));
            if (stream.atEnd())
            {
                break;
            }
            if (!list.isEmpty() && list.front().isEmpty())
            {
                list.pop_front();
            }
            if (!list.isEmpty() && list.back().isEmpty())
            {
                list.pop_back();
            }
            if (!list.isEmpty())
            {
                Command(list);
            }
        }
    }

private:
    Lexer<TokenID> m_lexer{};
    State m_state = State::Waiting;
}; // namespace corrosion
} // namespace corrosion
#endif // COMPILER_HPP
