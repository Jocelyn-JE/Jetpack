//
// Created by roussierenoa on 3/28/25.
//

#include "lib.hpp"

std::vector<std::string> splitSentence(std::string sen, char delimiter)
{
	std::istringstream ss(sen);
	std::vector<std::string> words;
	std::string temp;
	while (std::getline(ss, temp, delimiter)) {
		words.push_back(temp);
	}
	return words;
}
