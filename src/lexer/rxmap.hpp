#ifndef RXMAP_HPP
#define RXMAP_HPP

#include <QList>
#include <QRegExp>
#include <QString>
#include <QVector>
#include <tuple>
#include <utility>

#include "../config.hpp"

namespace corrosion
{

/**
 * @brief The RxMap is the class for the mapping QRexExp with some value.
 * @tparam T Type of mapped values.
 */
template <typename Type>
class RxMap
{
    using size_type = int;
    using container_type = QVector<Type>;
    using init_type = QList<std::pair<QString, Type>>;
    Type UNKNOWN{};

public:
    //! @brief Unknown value for unmatched result.
    const Type &Unknown() const CORROSION_NOEXCEPT
    {
        return UNKNOWN;
    }
    /**
     * @brief Return first matched experssion on first position in text or Unknown() value on failure.
     * @param Text Checked text.
     * @return Mapped value.
     */
    Type operator[](const QString &text)
    {
        if (m_regExp.indexIn(text) == 0)
        {
            auto captured = m_regExp.capturedTexts();
            for (size_type i = 1; i < captured.size(); i++)
            {
                if (!captured[i].isEmpty())
                {
                    return m_values[i - 1];
                }
            }
        }

        return UNKNOWN;
    }

    /**
     * @brief Return list of exact matched experssions in text or Unknown() value on failure.
     * @param Text Checked text.
     * @return List of pairs where First arg - mathched QRegExp, Second arg - mapped value.
     */
    QVector<std::pair<QString, Type>> Find(const QString &text)
    {
        if (m_regExp.exactMatch(text))
        {
            QVector<std::pair<QString, Type>> list;
            auto captured = m_regExp.capturedTexts();
            for (size_type i = 0; i < captured.size(); i++)
            {
                if (!captured[i].isEmpty())
                {
                    list.append({captured[i], m_values[i]});
                }
            }
            return list;
        }
        return {{"", UNKNOWN}};
    }
    /**
     * @brief Return first matched experssion index in text or Unknown() value on failure.
     * @param Text Checked text.
     * @return Pair where First arg - matched position, Second arg - mapped value.
     */
    std::pair<int, Type> Index(const QString &text)
    {
        auto index = m_regExp.indexIn(text);
        if (index != -1)
        {
            auto captured = m_regExp.capturedTexts();
            for (size_type i = 0; i < captured.size(); i++)
            {
                if (!captured[i].isEmpty())
                {
                    return {index, m_values[i]};
                }
            }
        }

        return {-1, UNKNOWN};
    }

    /**
     * @brief Return first exact matched experssion in text or Unknown() value on failure.
     * @param Text Checked text.
     * @return Pair where First arg - matched text, Second arg - mapped value.
     */
    std::pair<QString, Type> Exact(const QString &text)
    {
        if (m_regExp.exactMatch(text))
        {
            std::pair<QString, Type> result;
            auto captured = m_regExp.capturedTexts();
            for (size_type i = 1; i < captured.size(); i++)
            {
                if (!captured[i].isEmpty())
                {
                    result = std::make_pair(captured[i], m_values[i - 1]);
                }
            }
            return result;
        }
        return {m_regExp.cap(), UNKNOWN};
    }

    RxMap() = default;
    virtual ~RxMap() = default;

    RxMap &operator=(const RxMap &other)
    {
        CORROSION_ASSERT(other.m_regExp.capturedTexts().size() - 1 == other.m_values.size());
        m_values.clear();
        for (auto value : other.m_values)
        {
            this->m_values.append(value);
        }
        UNKNOWN = other.UNKNOWN;
        m_regExp = other.m_regExp;

        return *this;
    }
    RxMap &operator=(const RxMap &&other)
    {
        CORROSION_ASSERT(other.m_regExp.capturedTexts().size() - 1 == other.m_values.size());
        m_values.clear();
        for (auto &value : other.m_values)
        {
            this->m_values.append(value);
        }
        UNKNOWN = other.UNKNOWN;
        m_regExp = other.m_regExp;

        return *this;
    }
    /**
     * @brief RxMap Alias construct with QString instead of QRegExp.
     * @param unknown Valid unknown value.
     * @param list Vector of pairs future QRegExp pattern and mapped values.
     */
    RxMap(Type unknown, init_type &&list) : UNKNOWN(unknown)
    {
        QString pattern{};

        for (auto &&[key, value] : list)
        {
            QRegExp expr(key);
            for (int i = 0; i < expr.capturedTexts().size(); i++)
            {
                m_values.append(value);
            }
            pattern.append('(' + key + ')' + '|');
        }
        m_regExp.setPattern(pattern.chopped(1));
        CORROSION_ASSERT(m_regExp.capturedTexts().size() - 1 == m_values.size());
    }
    /**
     * @brief Returns reference to underlying container of values.
     * @return Underlying container.
     */
    const container_type &Data() const CORROSION_NOEXCEPT
    {
        return m_values;
    }

    /**
     * @brief Returns reference to underlying regular expression.
     * @return Underlying regular expression.
     */
    const QRegExp &RegExp() const CORROSION_NOEXCEPT
    {
        return m_regExp;
    }

protected:
    container_type m_values{};
    QRegExp m_regExp{};
}; // namespace rustc
} // namespace corrosion

#endif // RXMAP_HPP
