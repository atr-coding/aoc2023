#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <map>

int main() {
	std::size_t total{ 0 };
	if (std::ifstream file("data.txt"); file) {
		std::string line;
		while (std::getline(file, line)) {
			std::string numbers;
			for (auto c = line.begin(); c != line.end(); ++c) {
				if (std::isdigit(*c)) {
					numbers += *c;
				} else {
					for (const auto& [key, value] : std::map<std::string, std::string>{
							{"one", "1"},
							{"two", "2"},
							{"three", "3"},
							{"four", "4"},
							{"five", "5"},
							{"six", "6"},
							{"seven", "7"},
							{"eight", "8"},
							{"nine", "9"}
						}) {
						for (auto i = 0; i < key.size(); ++i) {
							if ((c + i) == line.end() || key[i] != *(c + i)) {
								break;
							}

							if (i == key.size() - 1) {
								numbers += value;
							}
						}
					}
				}
			}
			total += std::stoi(std::string{ numbers.front(), (numbers.size() > 1 ? numbers.back() : numbers.front()) });
		}
	}
	std::cout << total << '\n';
}