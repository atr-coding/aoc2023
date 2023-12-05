#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <array>
#include <memory>
#include <stacktrace>

template<typename T>
using cref = const T&;

template<typename Type, std::size_t N, std::size_t M = N>
using mdarray = std::array<std::array<Type, N>, M>;

template<typename Itr>
[[nodiscard]] Itr find_first_non_digit(Itr begin, Itr end) noexcept {
	for (auto itr = begin; itr != end; ++itr) {
		if (!std::isdigit(*itr)) {
			return itr;
		}
	}
	return end;
}

[[nodiscard]]
bool is_valid_char(unsigned char c) noexcept {
	return std::ispunct(c) && c != '.';
}

struct Element {
	int number{ -1 };
	char character{ -1 };
};

auto get_surrounding_digits(cref<mdarray<std::shared_ptr<Element>, 140>> grid, uint32_t x, uint32_t y) {
	std::vector<std::shared_ptr<Element>> results;
	for (int64_t t{ static_cast<int64_t>(y - 1) }; t <= static_cast<int64_t>(y + 1); ++t) {
		for (int64_t p{ static_cast<int64_t>(x - 1) }; p <= static_cast<int64_t>(x + 1); ++p) {
			if (!(t == y && p == x)) {
				if (t >= 0 && p >= 0 && grid[t][p] != nullptr) {
					//std::cout << "\t x(p): " << p << " y(t): " << t << '\n';
					if (std::find(results.begin(), results.end(), grid[t][p]) == results.end() && grid[t][p]->character == -1) {
						results.push_back(grid[t][p]);
					}
				}
			}
		}
	}
	return results;
}

int main() {
	if (std::ifstream file("data.txt"); file) {
		std::vector<std::string> lines;

		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}

		std::size_t total{ 0 };

		mdarray<std::shared_ptr<Element>, 140> grid;

		{
			std::size_t x{ 0 }, y{ 0 };
			for (auto itr = lines.begin(); itr != lines.end(); ++itr) {
				const auto& line = (*itr);
				for (auto c = line.begin(); c != line.end();) {
					if (std::isdigit(*c)) {
						const auto nondigit = find_first_non_digit(c, line.end());
						std::string number{ c, nondigit };

						auto elem = std::make_shared<Element>(Element{ std::stoi(number), -1 });

						for (auto i{ 0 }; i < number.size(); ++i) {
							grid[y][x + i] = elem;
						}

						x += number.size();
						c = nondigit;

					} else {
						if (std::ispunct(*c) && *c != '.') {
							if (grid[y][x] == nullptr) {
								grid[y][x] = std::make_shared<Element>(Element{ -1, *c });
							}

						}
						++x;
						++c;
					}
				}
				x = 0;
				++y;
			}
		}

		uint32_t gx{ 0 }, gy{ 0 };
		for (const auto& y : grid) {
			for (const auto& x : y) {
				if (x != nullptr && x->character != -1 && x->character == '*') {
					auto digits = get_surrounding_digits(grid, gx, gy);
					if (digits.size() == 2) {
						std::size_t local_total{ 1 };
						for (const auto& digit : digits) {
							local_total *= digit->number;
						}
						total += local_total;
					}
				}
				++gx;
			}
			gx = 0;
			++gy;
		}

		std::cout << total << '\n';

		/*
		* // part 1:
		for (auto itr = lines.begin(); itr != lines.end(); ++itr) {
			const auto& line = (*itr);
			std::size_t line_total{ 0 };
			for (auto c = line.begin(); c != line.end();) {
				if (std::isdigit(*c)) {
					const auto nondigit = find_first_non_digit(c, line.end());
					std::string number{ c, nondigit };
					std::cout << number;

					const std::size_t size = number.size();
					std::size_t x{ 0 };
					if (c != line.begin()) {
						x = std::distance(line.begin(), c) - 1;
					}

					// Check top row for characters.
					if (itr != lines.begin()) { // Since we're not at the top row, we need to check the row above us.
						const std::string& line_above = *(itr - 1);
						for (auto i = x; i <= x + size + (x > 0 ? 1 : 0); ++i) {
							auto lai = (line_above.begin() + i);
							if (lai != line_above.end()) {
								if (is_valid_char(*lai)) {
									line_total += std::stoi(number);
									continue;
								}
							}
						}
					}

					// Check sides for characters.
					auto start = (c != line.begin() ? c - 1 : c);
					for (auto i = 0; i <= size + 1; ++i) {
						auto c2 = start + i;
						if (c2 != line.end()) {
							if (is_valid_char(*c2)) {
								line_total += std::stoi(number);
								continue;
							}
						}
					}

					// Check bottom row for characters.
					if (itr + 1 != lines.end()) { // Since we're not at the bottom row, we need to check the row below us.
						const std::string& line_below = *(itr + 1);
						for (auto i = x; i <= x + size + (x > 0 ? 1 : 0); ++i) {
							auto lbi = (line_below.begin() + i);
							if (lbi != line_below.end()) {
								if (is_valid_char(*lbi)) {
									line_total += std::stoi(number);
									continue;
								}
							}
						}
					}

					c = nondigit;
				} else if (std::ispunct(*c) && *c != '.') {
					std::cout << *c;
					++c;
				} else {
					std::cout << '.';
					++c;
				}

			}
			std::cout << "\t:\t" << line_total;
			total += line_total;
			std::cout << '\n';
		}
		*/
		//std::cout << total << '\n';
	}
}