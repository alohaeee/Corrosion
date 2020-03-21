#ifndef CEXCEPTION_HPP
#define CEXCEPTION_HPP
#include "config.hpp"
#include <QString>
#include <exception>
namespace corrosion
{
/**
 * @brief The CException class with supported UTF-8 messaging.
 */
class CException : protected std::exception
{
    using error_type = int;

public:
    CException() = delete;
    CException(QString msg = QString(), error_type error = -1)
        : std::exception(), m_msg(msg), m_error(error){

                                        };
    virtual ~CException() = default;

    /**
     * @brief Return pair of message and error code.
     * @return Pair where First arg - QString message, Second arg - error code of error_type.
     */
    const std::pair<QString, error_type> All() const CORROSION_NOEXCEPT
    {
        return {m_msg, m_error};
    }

    /**
     * @brief Return message.
     * @return Message.
     */
    const QString &What() const CORROSION_NOEXCEPT
    {
        return m_msg;
    }

    /**
     * @brief Return error code.
     * error_type(-1) - undefined code.
     * @return Error code of error_type.
     */
    const error_type Error() const CORROSION_NOEXCEPT
    {
        return m_error;
    }
    /**
     * @brief Loging into console underlying message and error code.
     */
    void Log() const CORROSION_NOEXCEPT
    {
        qDebug(qUtf8Printable(m_msg + " - " + (m_error != -1 ? QString::number(m_error) : "NO ERROR CODE")));
    }

protected:
    QString m_msg{};
    error_type m_error{};
};
} // namespace corrosion

#endif // CEXCEPTION_HPP
