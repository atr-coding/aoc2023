#include <iostream>
#include <string>
#include <string_view>
#include <fstream>
#include <map>
#include <ranges>
#include <vector>
#include <utility>

void remove_all(std::string& str, unsigned char ch) {
	str.erase(std::remove(str.begin(), str.end(), ch), str.end());
}

std::vector<std::string> explode(std::string_view str, unsigned char ch) {
	return str |
		std::views::split(ch) |
		std::ranges::to<std::vector<std::string>>();
}

void trim(std::string& s) {
	auto not_space = [](unsigned char c) { return !std::isspace(c); };

	// erase the the spaces at the back first
	// so we don't have to do extra work
	s.erase(std::ranges::find_if(s | std::views::reverse, not_space).base(), s.end());

	// erase the spaces at the front
	s.erase(s.begin(), std::ranges::find_if(s, not_space));
}

struct Game {
	std::size_t id{ 0 };
	std::map<std::string, std::size_t> cubes = {
		{"red", 0},
		{"green", 0},
		{"blue", 0},
	};
};

int main() {
	if (std::ifstream file("data.txt"); file) {
		std::string line;
		std::vector<Game> games;
		while (std::getline(file, line)) {
			Game game;
			game.id = std::stoi(explode(explode(line, ':')[0], ' ')[1]);
			for (const auto& g : line | std::views::split(':') | std::views::drop(1)) {
				for (const auto& subset : g | std::views::split(';')) {
					for (const auto& cubes : subset | std::views::split(',')) {
						std::string cube;
						std::ranges::copy(cubes, std::back_inserter(cube));
						trim(cube);
						auto pull = explode(cube, ' ');
						auto n = std::stoi(pull[0]);
						auto& type = pull[1];
						if (n > game.cubes[type]) {
							game.cubes[type] = n;
						}
					}
				}
			}
			games.push_back(game);
		}

		std::size_t p1_sum{ 0 };
		std::size_t p2_sum{ 0 };

		const auto max_red = 12;
		const auto max_green = 13;
		const auto max_blue = 14;

		for (const auto& game : games) {
			const auto& red_cubes = game.cubes.at("red");
			const auto& green_cubes = game.cubes.at("green");
			const auto& blue_cubes = game.cubes.at("blue");
			std::cout << '[' << game.id << ':' << red_cubes << ", " << green_cubes << ", " << blue_cubes << "]\n";
			if (red_cubes <= max_red && green_cubes <= max_green && blue_cubes <= max_blue) {
				p1_sum += game.id;
			}

			p2_sum += red_cubes * green_cubes * blue_cubes;
		}

		std::cout << p2_sum << '\n';
	}
}