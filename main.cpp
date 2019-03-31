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
	/// �������C�ӂ�1�����ŕ���
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
	/// ��`����enum�̒l�����o��
	/// = ���L�q���Ă��Ȃ��ꍇ��,�������̒l���g�p����
	/// </summary>
	std::tuple<int32_t, std::string> FetchEnumValuePair(const std::string& source, const int32_t defaultValue)
	{
		const std::vector<std::string> values = ::Split(source, '=');
		if (values.size() == 1)
		{
			return { defaultValue, values.at(0) };
		}
		// int�ɕϊ��ł��Ȃ��p�^�[��(e.g. a = 1, b = a ���͖��Ή�)
		return { std::stoi(values.at(1)), values.at(0) };
	}

	/// <summary>
	/// ��`����enum class�̋L�q������,
	/// �񋓒萔 => �萔�Ɠ��������� �̘A�z�z����쐬
	/// </summary>
	template<class Type>
	std::unordered_map<Type, std::string> MakeEnumClassMap(const std::vector<std::string>& labels)
	{
		std::unordered_map<Type, std::string> result;
		int32_t defaultValue = 0;
		for (const std::string& label : labels)
		{
			const auto valueSet = ::FetchEnumValuePair(label, defaultValue);
			// �d���s��
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