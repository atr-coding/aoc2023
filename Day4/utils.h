#ifndef _UTILS_H
#define _UTILS_H

#include <cstddef>
#include <string>
#include <algorithm>
#include <vector>
#include <string_view>
#include <ranges>
#include <fstream>

template<typename T>
using cref = const T&;

template<typename Type, std::size_t N, std::size_t M = N>
using mdarray = std::array<std::array<Type, N>, M>;

template<typename Itr>
inline [[nodiscard]] Itr find_first_non_digit(Itr begin, Itr end) noexcept {
	for (auto itr = begin; itr != end; ++itr) {
		if (!std::isdigit(*itr)) {
			return itr;
		}
	}
	return end;
}

inline void remove_all(std::string& str, unsigned char ch) {
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
}

inline std::vector<std::string> explode(std::string_view str, unsigned char ch) {
	return str |
		std::views::split(ch) |
		std::ranges::to<std::vector<std::string>>();
}

inline void trim(std::string& s) {
	auto not_space = [](unsigned char c) { return !std::isspace(c); };

	// erase the the spaces at the back first
	// so we don't have to do extra work
	s.erase(std::ranges::find_if(s | std::views::reverse, not_space).base(), s.end());

	// erase the spaces at the front
	s.erase(s.begin(), std::ranges::find_if(s, not_space));
}

inline [[nodiscard]] std::vector<std::string> read_file_into_vector(const std::string& filename) {
	if (std::ifstream file(filename); file) {
		std::vector<std::string> lines;
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		return lines;
	}
	return {};
}

template<typename VecType, typename Func>
inline void operator|(std::vector<VecType>& vec, Func func) {
	for (auto& str : vec) {
		func(str);
	}
}

template<typename Type>
void erase_first_element(std::vector<Type>& vec) {
	vec.erase(vec.begin());
}

template<typename Type, typename Func>
void erase_elements_if(std::vector<Type>& vec, Func func) {
	vec.erase(std::ranges::remove_if(vec, func));
}

#endif