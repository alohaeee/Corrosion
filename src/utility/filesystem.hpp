#ifndef FILEREADER_HPP
#define FILEREADER_HPP
#include "cexception.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>
namespace corrosion
{
/**
 * @brief The FileSystem is class that eases working with files.
 */
class FileSystem
{
public:
    static QString ReadAllFile(const QString &path)
    {
        QFile file(path);
        file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text);
        if (!file.isOpen())
        {
            throw CException("Can't open file: " + path);
        }
        QTextStream stream(&file);
        return stream.readAll();
    };
    static QString CurrentDirectory()
    {

        return m_dir.currentPath();
    }
    static QStringList LookAtDirectory()
    {

        return m_dir.entryList();
    }

private:
    inline static QDir m_dir{};
};
} // namespace corrosion

#endif // FILEREADER_HPP
