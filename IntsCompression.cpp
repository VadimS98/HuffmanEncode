#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <memory>
#include <random>

#include "HuffNode.h"

using namespace std;

std::mutex mlock;

struct comp
{
	bool operator()(const HuffmanNode* l, const HuffmanNode* r) const
	{
		return l->freq > r->freq;
	}
};

bool isLeaf(HuffmanNode* root) {
	return root->left == nullptr && root->right == nullptr;
}

void encode(HuffmanNode* root, string str, unordered_map<uint8_t, string>& huffmanCode)
{
	if (root == nullptr) {
		return;
	}

	if (isLeaf(root)) {
		mlock.lock();
		huffmanCode[root->value] = (str != "") ? str : "1";
		mlock.unlock();
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void decode(HuffmanNode* root, int& index, string& str, vector<int>* result)
{
	if (root == nullptr) {
		return;
	}

	if (isLeaf(root))
	{
		result->push_back(root->value);
		return;
	}

	index++;

	if (str[index] == '0') {
		decode(root->left, index, str, result);
	}
	else {
		decode(root->right, index, str, result);
	}
}

void encodeChunk(HuffmanNode* root, const vector<uint8_t>& chunk, unordered_map<uint8_t, string>& huffmanCode)
{
	for (uint8_t value : chunk) {
		encode(root, "", huffmanCode);
	}
}

void buildHuffmanTree(vector<uint8_t> given) {

	unordered_map<uint8_t, int> freq;
	for (uint8_t i : given) {
		freq[i]++;
	}


	priority_queue<HuffmanNode*, vector<HuffmanNode*>, comp> pq;

	for (auto pair : freq) {
		pq.push(new HuffmanNode(pair.first, pair.second));
	}

	while (pq.size() != 1) {
		HuffmanNode* l = pq.top();
		pq.pop();
		HuffmanNode* r = pq.top();
		pq.pop();

		int sum = l->freq + r->freq;
		pq.push(new HuffmanNode(0, sum, l, r));
	}

	HuffmanNode* root = pq.top();
	unordered_map<uint8_t, string> huffmanCode;
	vector<thread> threads;
	const int numThreads = thread::hardware_concurrency();
	const int chunkSize = given.size() / numThreads;

	for (int i = 0; i < numThreads; i++) {
		int start = i * chunkSize;
		int end = (i == numThreads - 1) ? given.size() : (i + 1) * chunkSize;
		vector<uint8_t> chunk(given.begin() + start, given.begin() + end);
		threads.emplace_back(encodeChunk, root, vector<uint8_t>(given.begin() + start, given.begin() + end), std::ref(huffmanCode));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	string str;
	vector<string> coded;
	for (char ch : given) {
		str += huffmanCode[ch];
		coded.push_back(huffmanCode[ch]);
	}
	//cout << "\nThe encoded string is:\n" << str << endl;

	vector<int> decodedResult;
	decodedResult.reserve(1000000);

	threads.clear();

	if (isLeaf(root))
	{
		while (root->freq--) {
			cout << root->value;
		}
	}
	else {
		int index = -1;
		while (index < (int)str.size() - 1) {
			decode(root, index, str, &decodedResult);
		}
	}

	std::ofstream outputFile("out.txt");

	if (!outputFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
	}

	int decodedSize = decodedResult.size();
	for (int i = 0; i < decodedSize; ++i) {
		outputFile << decodedResult.at(i);
		
		if(i < decodedSize - 1) {
			outputFile << ",";
		}

		if ((i + 1) % 50 == 0) {
			outputFile << std::endl;
		}
	}

	outputFile.close();
}

vector<uint8_t> getInts(string line) {
	vector<uint8_t> ret;;
	string one;
	for (auto ch : line) {
		if (ch != ',') {
			one += ch;
		}
		else {
			ret.push_back(stoi(one));
			one = "";
		}
	}
	if (!one.empty()) {
		ret.push_back(stoi(one));
	}
	return ret;
}

int createFile() {
	std::random_device rd;
	std::mt19937 gen(rd());

	int amount = 10000;
	int valuesRange = 100;

	std::uniform_int_distribution<int> valuesDis(1, valuesRange);

	std::ofstream outputFile("in.txt");

	if (!outputFile.is_open()) {
		std::cerr << "Error opening the file!" << std::endl;
		return 1;
	}

	for (int i = 0; i < amount; ++i) {
		int randomNumber = valuesDis(gen);
		outputFile << randomNumber;

		if (i < amount - 1) {
			outputFile << ",";
		}

		if ((i + 1) % 50 == 0) {
			outputFile << std::endl;
		}
	}

	outputFile.close();
	return 0;
}

int main()
{
	string line;
	ifstream in;
	vector<uint8_t> toCode;

	createFile();

	in.open("in.txt");
	if (in.is_open()) {
		while (std::getline(in, line))
		{
			auto b = getInts(line);
			toCode.insert(toCode.end(), b.begin(), b.end());
		}
	}
	//vector<uint8_t> text = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	buildHuffmanTree(toCode);
	return 0;
}
