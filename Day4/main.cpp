#include <iostream>
#include <map>
#include <array>
#include <print>

#include "utils.h"

std::array<std::size_t, 300> instance_count{ 0 };

void calculate_copies(const std::map<std::size_t, std::size_t>& card_win_counts, std::size_t card_num) {
	if (card_num <= card_win_counts.size()) {
		std::size_t wins = card_win_counts.at(card_num);
		instance_count[card_num]++;
		for (auto i{ 1 }; i <= wins; ++i) {
			//instance_count[card_num + i]++;
			calculate_copies(card_win_counts, card_num + i);
		}
	}
}

int main() {
	std::size_t total{ 0 };
	std::map<std::size_t, std::size_t> card_win_counts;

	auto file = read_file_into_vector("data.txt");
	for(const auto& line : file) {

		auto colon_split = explode(line, ':');
		std::string num = colon_split.at(0).substr(4, colon_split.at(0).back());
		trim(num);
		std::size_t card_num = std::stoi(num);
		erase_first_element(colon_split); // Removes "Card #:"

		for (const auto& e : colon_split) {
			auto pipe_split = explode(e, '|');
			auto win_nums = explode(pipe_split.at(0), ' ');
			auto my_nums = explode(pipe_split.at(1), ' ');

			// Remove empty strings
			win_nums.erase(std::remove_if(win_nums.begin(), win_nums.end(), [](const std::string& elem) { return elem.empty(); }), win_nums.end());
			my_nums.erase(std::remove_if(my_nums.begin(), my_nums.end(), [](const std::string& elem) { return elem.empty(); }), my_nums.end());

			std::ranges::sort(win_nums);
			std::ranges::sort(my_nums);

			std::vector<std::string> inter{};
			std::ranges::set_intersection(win_nums, my_nums, std::back_inserter(inter));

			/*total += (inter.size() == 0 ? 0 : static_cast<std::size_t>(std::pow(2, inter.size() - 1)));*/
			card_win_counts[card_num] = inter.size();
			};

		};

	//total = calculate_copies(card_win_counts, 1);
	for (const auto& [card_num, win_count] : card_win_counts) {
		calculate_copies(card_win_counts, card_num);
	}

	//std::map<std::size_t, std::size_t> instance_count;

	//for (const auto& [card_num, win_count] : card_win_counts) {
	//	std::println("Card Number: {} | Win Count: {}", card_num, win_count);
	//	/*for (auto i{ 1 }; i < win_count; ++i) {
	//		instance_count[card_num + i]++;
	//	}*/
	//}

	auto cnum{ 1 };
	for (const auto& y : instance_count | std::views::drop(1)) {
		std::println("Card Number: {} | Instance Count: {}", cnum, y);
		cnum++;
		total += y;
	}

	std::println("Total: {}", total);
}