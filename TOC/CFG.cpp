#include <iostream>
#include "CFG.h"

CFG::CFG(
	const AlphabetSet& nonTerminals, 
	const AlphabetSet& terminals, 
	const ProductionRules& rules, 
	const Alphabet start, 
	const Alphabet epsilon
)
	: nonTerminals(nonTerminals), terminals(terminals), rules(rules), start(start), epsilon(epsilon)
{
}

bool CFG::isTerminal(const Alphabet alpha) const
{
	return terminals.find(alpha) != terminals.end();
}

bool CFG::isNonTerminal(const Alphabet alpha) const
{
	return nonTerminals.find(alpha) != nonTerminals.end();
}

bool CFG::isEpsilon(const Alphabet alpha) const {
	return alpha == epsilon;
}

void logSet(const AlphabetSet& set) {
    std::cout << "{ ";

    bool first = true;
    for (const auto& item : set) {
        if (!first) {
            std::cout << ", ";
        }
        first = false;

        if (item == '\0') {
            std::cout << "\\0"; // Print null character as "\0"
        }
        else {
            std::cout << item;
        }
    }

    std::cout << " }" << std::endl;
}

void logMapSet(const std::unordered_map<char, AlphabetSet>& map) {
	std::cout << "{\n";

	for (const auto& pair : map) {
		std::cout << pair.first << " : ";
		logSet(pair.second);
	}

	std::cout << "}" << std::endl;
}

void logNonTerminalsPositions(const std::unordered_map<Alphabet, std::vector<std::tuple<Alphabet, size_t, size_t>>>& map) {
    std::cout << "{\n";

    for (const auto& [key, positions] : map) {
        std::cout << key << " : [ ";

        bool first = true;
        for (const auto& pos : positions) {
            if (!first) {
                std::cout << ", ";
            }
            first = false;

            std::cout << "(" << std::get<0>(pos) << ", "
                << std::get<1>(pos) << ", "
                << std::get<2>(pos) << ")";
        }

        std::cout << " ]\n";
    }

    std::cout << "}" << std::endl;
}
