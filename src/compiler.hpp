#ifndef COMPILER_HPP
#define COMPILER_HPP

#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "utility/cexception.hpp"
#include "utility/filesystem.hpp"

namespace corrosion
{
class Compiler
{
    using constans = const QString;
    void InitLexer()
    {
        m_lexer.SetPackedTokens({TokenID::UNKNOWN,
                                 {
                                     {"while", TokenID::KW_WHILE},
                                     {"if", TokenID::KW_IF},
                                     {"return", TokenID::KW_RETURN},
                                     //{"[0-9]+", TokenID::INTEGER_LITERAL},
                                     {"[+-]?([0-9]*[.])?[0-9]+", TokenID::FLOAT_LITERAL},
                                     {"\"([^\\\\]|\\\\.)*\"", TokenID::STRING_LITERAL},
                                     {"[a-zA-Z_][a-zA-Z0-9_]*", TokenID::IDENTIFIER},
                                 }});
        m_lexer.SetTerminateTokens({TokenID::UNKNOWN,
                                    {{"\\(", TokenID::LPARENTHESIS},
                                     {"\\)", TokenID::RPARENTHESIS},
                                     {"\\{", TokenID::LBRACER},
                                     {"\\}", TokenID::RBRACER},
                                     {"=", TokenID::EQ},
                                     {";", TokenID::SEMI},
                                     {"\\+", TokenID::PLUS},
                                     {"-", TokenID::MINUS},
                                     {"\\*", TokenID::STAR},
                                     {"/", TokenID::SLASH},
                                     {"\\+=", TokenID::PLUS_EQ},
                                     {"-=", TokenID::MINUS_EQ},
                                     {"\\*=", TokenID::STAR_EQ},
                                     {"/=", TokenID::SLASH_EQ},
                                     {"\\++", TokenID::PLUS_PLUS},
                                     {"--", TokenID::MINUS_MINUS},
                                     // ignore multi line comments
                                     {"(/\\*.*\\*/)", Lexer<TokenID>::IGNORE},
                                     // ignore line comment
                                     {"//([^\n]*)(\n|$)", Lexer<TokenID>::IGNORE},
                                     // ignore white space
                                     {"[ \\n\\t\\r]", Lexer<TokenID>::IGNORE}}});
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
    auto &GetLexer() const CORROSION_NOEXCEPT
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
