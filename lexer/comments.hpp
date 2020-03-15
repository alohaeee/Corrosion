#ifndef COMMENTS_HPP
#define COMMENTS_HPP

//#include <QString>
// class Comments
//{
//    using size_type = int;
//    using check_info = std::tuple<bool, int>;

// public:
//    enum CommentType
//    {
//        LINE,
//        BLOCK,
//        NONE
//    };
//    using check_result = std::tuple<CommentType, size_type>;

//    static check_result Check(const QString &text, size_type start_pos)
//    {
//        if (text[start_pos] == '/')
//        {
//            if (text[start_pos + 1] == '/')
//            {
//                return {CommentType::LINE, text.indexOf('\n', start_pos)};
//            }
//            else if (text[start_pos + 1] == '*')
//            {
//                return {CommentType::BLOCK, text.indexOf("*/", start_pos) + 2};
//            }
//        }
//        return {CommentType::NONE, -1};
//    }
//};

#endif // COMMENTS_HPP
