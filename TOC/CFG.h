#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <tuple>

using Alphabet = char;
using AlphabetSet = std::unordered_set<char>;
using Rule = std::string;
using Rules = std::vector<Rule>;
using ProductionRules = std::unordered_map<char, Rules>;

class CFG
{
public:
	AlphabetSet nonTerminals;
	AlphabetSet terminals;
	ProductionRules rules;
	Alphabet start;
	Alphabet epsilon;

	CFG(
		const AlphabetSet& nonTerminals,
		const AlphabetSet& terminals,
		const ProductionRules& rules,
		const Alphabet start,
		const Alphabet epsilon = '\0'
	);

	bool isTerminal(const Alphabet alpha) const;
	bool isNonTerminal(const Alphabet alpha) const;
	bool isEpsilon(const Alphabet alpha) const;
};

void logSet(const AlphabetSet& set);
void logMapSet(const std::unordered_map<char, AlphabetSet>& map);
void logNonTerminalsPositions(const std::unordered_map<Alphabet, std::vector<std::tuple<Alphabet, size_t, size_t>>>& map);