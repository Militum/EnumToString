#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace
{
#define DEFINE_ENUM_CLASS(Type,...)\
    enum class Type\
    {\
        __VA_ARGS__\
    };\
    static std::string ToString(Type value)\
    {\
        static std::unordered_map<Type, std::string> nameMap;\
        if(nameMap.size() == 0)\
        {\
            const std::vector<std::string> names = ::Split(#__VA_ARGS__, ',');\
            nameMap = ::MakeEnumClassMap<Type>(names);\
        }\
        if(nameMap.count(value) == 0)\
        {\
            return "unknown value";\
        }\
        return nameMap.at(value);\
    }

#define ENUM_TO_STRING(value) \
	ToString(value)

	/// <summary>
	/// 文字列を任意の1文字で分割
	/// </summary>
	/// <param name="src"></param>
	/// <param name="delimiter"></param>
	/// <returns></returns>
	std::vector<std::string> Split(const std::string& src, const char delimiter)
	{
		std::vector<std::string> result;
		size_t currentIndex = 0;
		size_t endIndex = 0;
		const size_t size = src.size();
		do
		{
			endIndex = src.find_first_of(delimiter, currentIndex);
			if (endIndex == std::string::npos)
			{
				endIndex = size;
			}
			result.emplace_back(src.substr(currentIndex, endIndex - currentIndex));
			currentIndex = endIndex + 1;
		} while (currentIndex < size);
		return result;
	}

	/// <summary>
	/// 定義したenumの値を取り出す
	/// = を記述していない場合は,第二引数の値を使用する
	/// </summary>
	std::tuple<int32_t, std::string> FetchEnumValuePair(const std::string& source, const int32_t defaultValue)
	{
		const std::vector<std::string> values = ::Split(source, '=');
		if (values.size() == 1)
		{
			return { defaultValue, values.at(0) };
		}
		// intに変換できないパターン(e.g. a = 1, b = a 等は未対応)
		return { std::stoi(values.at(1)), values.at(0) };
	}

	/// <summary>
	/// 定義したenum classの記述を元に,
	/// 列挙定数 => 定数と同じ文字列 の連想配列を作成
	/// </summary>
	template<class Type>
	std::unordered_map<Type, std::string> MakeEnumClassMap(const std::vector<std::string>& labels)
	{
		std::unordered_map<Type, std::string> result;
		int32_t defaultValue = 0;
		for (const std::string& label : labels)
		{
			const auto valueSet = ::FetchEnumValuePair(label, defaultValue);
			// 重複不可
			result[static_cast<Type>(std::get<0>(valueSet))] = std::get<1>(valueSet);
			defaultValue = std::get<0>(valueSet) + 1;
		}
		return result;
	}

	DEFINE_ENUM_CLASS(
		State,
		None = 8,
		Init = 2,
		Three  // 3
	);
}

// example
int main(void)
{
	std::cout << ENUM_TO_STRING(::State::None) << "\n";  // None
	std::cout << ENUM_TO_STRING(::State::Init) << "\n";  // Init
	std::cout << ENUM_TO_STRING(::State::Three) << "\n"; // Three
	std::cout << ENUM_TO_STRING(static_cast<::State>(100)) << "\n"; // unknown value
	return 0;
}