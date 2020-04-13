#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

class Node {
public:
	Node* parent;

	std::map<char, Node*> next_vert;

	Node() : next_vert(), leaf(0), symbol('\0'),			//2 constructors
		parent(nullptr) {}
	Node(char symbol, Node* parent) : next_vert(), leaf(0),
		symbol(symbol),	parent(parent) {}

	char get_symbol() {
		return symbol;
	}

	void make_leaf() {
		leaf = true;
	}

	bool is_leaf() {
		return leaf;
	}

	void IncVal() {
		++value;
	}

	int GetVal() {
		return value;
	}

private:
	char symbol;
	bool leaf;
	int value;
};

class Trie {
public:
	Trie(std::vector<std::string>& patterns);
	std::string FindHint(const std::string& str);
private:
	void AddPattern(std::string str);
	Node* root;
	Node* current_node;
	std::string current_word;
};

Trie::Trie(std::vector<std::string>& patterns) {
	root = new Node();
	for (std::string pattern : patterns) {
		AddPattern(pattern);
	}
	current_node = root;
}

void Trie::AddPattern(std::string str) {
	Node* node = root;
	for (int i = 0; i < str.length(); ++i) {
		char sym = str[i];
		if (node->next_vert.find(sym) == node->next_vert.end()) {
			Node* new_node = new Node(sym, node);
			node->next_vert.insert(std::make_pair(sym, new_node));
		}
		node->IncVal();
		node = node->next_vert.at(sym);
	}
	node->make_leaf();
}

std::string Trie::FindHint(const std::string& str) {
	for (int i = 0; i < str.length(); ++i) {
		char sym = str[i];
		if (current_node->next_vert.find(sym) == current_node->next_vert.end()) {
			return current_word;
		}
		if (!current_node->is_leaf()) {
			current_node = current_node->next_vert.at(sym);
		}
		current_word += sym;
	}

	while (!current_node->is_leaf()) {
		int val = current_node->GetVal();
		bool found = 0;
		for (auto next : current_node->next_vert) {
			if (next.second->GetVal() == val){
				current_node = next.second;
				found = 1;
				continue;
			}
		}
		if (!found) {
			return current_word;
		}
	}

	return current_word;
}

class Commander {
public:
	Commander(Trie* trie) : trie(trie) {}
	std::string ShowHint(const std::string& query);
private:
	Trie* trie;
};

std::string Commander::ShowHint(const std::string& query) {
	return trie->FindHint(query);
}

void Parse(std::string text, std::vector<std::string>& patterns) {
	size_t last_space_pos = 0;
	for (size_t i = 0; i < text.length(); ++i) {
		if (text[i] == ' ') {
			patterns.push_back(text.substr(last_space_pos, i - last_space_pos));
		}
		last_space_pos = i;
	}
	patterns.push_back(text.substr(last_space_pos, text.length() - last_space_pos));
}

int main() {
	std::cout << "Add text: ";
	std::string text;
	std::getline(std::cin, text);
	std::vector<std::string> patterns;

	Parse(text, patterns);
	Trie trie(patterns);
	Commander comm(&trie);
	std::string command;
	std::string query;
	while (true) {
		std::cin >> command;
		if (command == "Request") {
			std::string hint;
			std::cin >> query;
			hint = comm.ShowHint(query);
			std::cout << hint << std::endl;
		}
	}
}