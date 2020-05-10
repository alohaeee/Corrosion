//
// Created by Vladislav on 05.05.2020.
//

#ifndef CORROSION_SRC_UTILITY_FILE_READER_HPP_
#define CORROSION_SRC_UTILITY_FILE_READER_HPP_

namespace corrosion
{
	class FileLoader
	{
	 public:
		static inline bool exist(const std::filesystem::path& path)
		{
			return std::filesystem::exists(path);
		}
		static inline void create(const std::filesystem::path& path)
		{
			if(!exist(path.parent_path()))
			{
				std::filesystem::create_directory(path.parent_path());
			}
			std::ofstream stream(path.string());
			if(!stream.is_open())
			{
				throw std::invalid_argument("Can't open file: " + path.string());
			}
			stream.close();
		}
		template<typename Char = char>
		static std::string readFile(const std::filesystem::path& path)
		{
			std::ifstream stream(path);

			if (stream.is_open())
			{
				std::string str((std::istreambuf_iterator<Char>(stream)),
					std::istreambuf_iterator<Char>());

				stream.close();
				return str;
			}
			throw std::invalid_argument("Can't open file: " + path.string());
		}
	};
}

#endif //CORROSION_SRC_UTILITY_FILE_READER_HPP_
