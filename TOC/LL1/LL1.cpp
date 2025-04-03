#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "../CFG.h"

using ParsingMap = std::unordered_map<char, AlphabetSet>;
using NonTerminalPosition = std::tuple<Alphabet, size_t, size_t>;
using NonTerminalsPositions = std::unordered_map<Alphabet, std::vector<NonTerminalPosition>>;

class LL1
{
	CFG grammar;
	ParsingMap firstMap;
	ParsingMap followMap;
	NonTerminalsPositions nonTerminalsPos;

public:
	LL1(const CFG& grammar)
		: grammar(grammar), firstMap({}), followMap({}), nonTerminalsPos({})
	{
		createNonTerminalPos();

		for (const Alphabet& nonTerminal : grammar.nonTerminals) {
			first(nonTerminal);
		}

		for (const Alphabet& nonTerminal : grammar.nonTerminals) {
			follow(nonTerminal);
		}
		//follow('B');

		//logNonTerminalsPositions(nonTerminalsPos);
		//logMapSet(firstMap);
		logMapSet(followMap);
	}

	AlphabetSet& first(Alphabet nonTerminal)
	{
		if (!firstMap[nonTerminal].empty()) {
			return firstMap[nonTerminal];
		}

		for (const Rule& rule : grammar.rules[nonTerminal]) {
			bool goToNextAlpha = true;
			int alphaIndex = 0;

			if (rule.empty()) {
				firstMap[nonTerminal].insert(grammar.epsilon);
				continue;
			}

			while (goToNextAlpha && alphaIndex < rule.size()) {
				goToNextAlpha = false;
				Alphabet alpha = rule[alphaIndex];

				// Terminal
				if (grammar.isTerminal(alpha)) {
					firstMap[nonTerminal].insert(alpha);
				}
				// Non-terminal
				else {
					AlphabetSet toAdd = first(alpha);
					if (toAdd.find(grammar.epsilon) != toAdd.end()) {
						toAdd.erase(grammar.epsilon);
						alphaIndex++;
						goToNextAlpha = true;
					}
					firstMap[nonTerminal].insert(toAdd.begin(), toAdd.end());
				}
			}
		}

		return firstMap[nonTerminal];
	}

	AlphabetSet& follow(Alphabet nonTerminal)
	{
		if (!followMap[nonTerminal].empty()) {
			return followMap[nonTerminal];
		}

		if (nonTerminal == grammar.start) {
			followMap[nonTerminal].insert('$');
		}
		if (nonTerminalsPos[nonTerminal].empty()) {
			followMap[nonTerminal].insert({});
		}

		for (const auto& pos: nonTerminalsPos[nonTerminal]) {
			auto& [ruleNonT, ruleIndex, alphaIndex] = pos;
			Rule rule = grammar.rules[ruleNonT][ruleIndex];
			size_t nextAlphaIndex = alphaIndex + 1;
			bool goToNextAlpha = true;

			while (goToNextAlpha) {
				goToNextAlpha = false;
				Alphabet nextAlpha = rule[nextAlphaIndex];

				if (grammar.isTerminal(nextAlpha) && nextAlpha != grammar.epsilon) {
					followMap[nonTerminal].insert(nextAlpha);
					break;
				}

				if (nextAlphaIndex >= rule.size()) {
					if (nonTerminal != ruleNonT) {
						AlphabetSet toAdd = follow(ruleNonT);
						followMap[nonTerminal].insert(toAdd.begin(), toAdd.end());
					}
				}
				else {
					AlphabetSet toAdd = first(rule[nextAlphaIndex]);

					if (toAdd.find(grammar.epsilon) != toAdd.end()) {
						toAdd.erase(grammar.epsilon);
						nextAlphaIndex++;
						goToNextAlpha = true;
					}

					followMap[nonTerminal].insert(toAdd.begin(), toAdd.end());
				}
			}
		}

		return followMap[nonTerminal];
	}

private:
	void createNonTerminalPos() {
		for (const Alphabet& nonT : grammar.nonTerminals) {
			Rules rules = grammar.rules[nonT];
			for (int i = 0; i < rules.size(); i++) {
				Rule rule = rules[i];
				for (int j = 0; j < rule.size(); j++) {
					Alphabet alpha = rule[j];
					if (grammar.isNonTerminal(alpha)) {
						nonTerminalsPos[alpha].emplace_back(nonT, i, j);
					}
				}
			}
		}
	}
};

static CFG creategrammar1()
{
	AlphabetSet nonTerminals = { 'S', 'A', 'B', 'C', 'D', 'E' };
	AlphabetSet terminals = { 'a', 'b', 'c', 'd', 'e', '\0' };
	ProductionRules rules = {
		{'S', {"ABCDE"}},
		{'A', {"a", "\0"} },
		{'B', {"b", "\0"} },
		{'C', {"c"} },
		{'D', {"d", "\0"} },
		{'E', {"e", "\0"} },
	};
	Alphabet start = 'S';
	return CFG(nonTerminals, terminals, rules, start);
}

static CFG creategrammar2()
{
	AlphabetSet nonTerminals = { 'P', 'Q', 'R', 'S' };
	AlphabetSet terminals = { 'x', 'y', 'z', 'w', '\0' };
	ProductionRules rules = {
		{'P', {"xQRS"}},
		{'Q', {"yz", "z"} },
		{'R', {"w", "\0"} },
		{'S', {"y"} },
	};
	Alphabet start = 'P';
	return CFG(nonTerminals, terminals, rules, start);
}

static CFG creategrammar3()
{
	AlphabetSet nonTerminals = { 'E', 'T', 'F', 'A', 'B'};
	AlphabetSet terminals = { 'i', '(', ')', '+', '*', '\0'};
	ProductionRules rules = {
		{'E', {"TA"}},
		{'A', {"+TA", "\0"} },
		{'T', {"FB"}},
		{'B', {"*FB", "\0"}},
		{'F', {"i", "(E)"}},
	};
	Alphabet start = 'E';
	return CFG(nonTerminals, terminals, rules, start);
}

static CFG creategrammar4()
{
	AlphabetSet nonTerminals = { 'S', 'B', 'C' };
	AlphabetSet terminals = { 'a', 'b', 'c', 'd', '\0' };
	ProductionRules rules = {
		{'S', {"Bb", "Cd"}},
		{'B', {"aB", "\0"} },
		{'C', {"cC", "\0"}},
	};
	Alphabet start = 'S';
	return CFG(nonTerminals, terminals, rules, start);
}

int main()
{
	LL1 ll1(creategrammar3());
}