// http://www.cplusplus.com/doc/tutorial/files/
// https://stackoverflow.com/questions/7663709/how-can-i-convert-a-stdstring-to-int
// https://stackoverflow.com/questions/4176326/arguments-to-main-in-c
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

void readFile(std::vector<int> &elements, int &nbElements, int &maxBlockSum, std::ifstream &file) {
	std::string line;

	getline(file, line);
	nbElements = std::stoi(line);

	getline(file, line);
	maxBlockSum = std::stoi(line);

	while(getline(file, line)) {
		elements.push_back(std::stoi(line));
	}
}

int sum(std::vector<int> block) {
	int finalSum = 0;

	for(int i = 0; i < block.size(); i++) {
		finalSum += block.at(i);
	}

	return finalSum;
}

void order(std::vector<int> &block) {
	for(int i = 0; i < block.size(); i++) {
		int j = i;

		while((j > 0) && (block.at(j-1) < block.at(j))) {
			int temp = block.at(j);
			block.at(j) = block.at(j - 1);
			block.at(j - 1) = temp;
			j--;
		}
	}
}

std::vector<int> randOrder(std::vector<int> block) {
	std::vector<int> tempBlock = block;
	std::vector<int> returnBlock;

	while(!tempBlock.empty()) {
		int choice = rand() % tempBlock.size();

		returnBlock.push_back(tempBlock.at(choice));
		tempBlock.erase(tempBlock.begin() + choice);
	}

	return returnBlock;
}

//nb of bins : 5, 37, 133, 1052
//grades : 14, 0, 10, 10
//grade : 8.5
std::vector<std::vector<int>> firstSort(std::vector<int> block, int max) {
	std::vector<std::vector<int>> blocks;
	std::vector<int> currentBlock;

	for(int i = 0; i < block.size(); i++) {
		if(((sum(currentBlock) + block.at(i)) >= max)) {
			blocks.push_back(currentBlock);
			currentBlock.clear();
		}

		currentBlock.push_back(block.at(i));
	}

	blocks.push_back(currentBlock);

	return blocks;
}

//nb of bins : 5, 31, 128, 1049
//grades : 14, 14, 12, 10
//grade : 12.5
std::vector<std::vector<int>> secondSort(std::vector<int> block, int max) {
	std::vector<int> tempBlock = block;
	std::vector<int> currentBlock;
	std::vector<int> erase;
	std::vector<std::vector<int>> blocks;

	while(!tempBlock.empty()) {
		for(int i = 0; i < tempBlock.size(); i++) {
			if(sum(currentBlock) + tempBlock.at(i) < max) {
				currentBlock.push_back(tempBlock.at(i));
				erase.push_back(i);
			} else if(sum(currentBlock) + tempBlock.at(i) == max) {
				currentBlock.push_back(tempBlock.at(i));
				erase.push_back(i);
				break;
			}
		}

		for(int i = 0; i < erase.size(); i++) {
			tempBlock.erase(tempBlock.begin() + erase.at(i) - i);
		}

		erase.clear();

		blocks.push_back(currentBlock);
		currentBlock.clear();
	}

	return blocks;
}

//nb of bins : 4, 31, 127, 1019
//grades : 20, 14, 14, 12
//grade : 15
std::vector<std::vector<int>> thirdSort(std::vector<int> block, int max) {
	std::vector<int> tempBlock = block;
	std::vector<std::vector<int>> blocks;
   	
   	do {
   		tempBlock = randOrder(tempBlock);
   		blocks = secondSort(tempBlock, max);
   	} while(blocks.size() > ceil((sum(tempBlock) / max) + 0.1 * (sum(tempBlock) / max)));

   	return blocks;
}

//nb of bins : 4, 31, 127, 1019
//grades : 20, 14, 14, 12
//grade : 15
// Way faster than the last one
std::vector<std::vector<int>> fourthSort(std::vector<int> block, int max) {
	std::vector<std::vector<int>> blocks;
	std::vector<int> tempBlock = block;
	std::vector<int> currentBlock;
	order(tempBlock);

	currentBlock.push_back(tempBlock.at(0));
	blocks.push_back(currentBlock);

	int bestBlock = -1;
	int bestBlockSum = -1;
	int currentBlockSum;

	for(int i = 1; i < tempBlock.size(); i++) {
		for(int j = 0; j < blocks.size(); j++) {
			currentBlockSum = sum(blocks.at(j));
			if((bestBlockSum < currentBlockSum) && ((currentBlockSum + tempBlock.at(i)) <= max)) {
				bestBlockSum = currentBlockSum;
				bestBlock = j;
			}
		}

		if(bestBlock == -1) {
			currentBlock.clear();
			currentBlock.push_back(tempBlock.at(i));
			blocks.push_back(currentBlock);
		} else {
			blocks.at(bestBlock).push_back(tempBlock.at(i));
			bestBlock = -1;
			bestBlockSum = -1;
		}
	}

	return blocks;
}

int main(int argc, char **argv) {

	std::string inputFileName;
	std::string outputFileName;

	// request inputFileName if it is not set and initialize outputFileName
	if(argc < 2) {
		std::cout << "Saisir le nom de votre fichier : ";
		std::cin >> inputFileName;
	} else {
		inputFileName = argv[1];
	}

	outputFileName = inputFileName.substr(0, inputFileName.size() - 4) + ".out1";

	// open the input file
   	std::ifstream inputFile;
   	inputFile.open(inputFileName, std::fstream::in);

   	// open the output file
   	std::ofstream outputFile;
   	outputFile.open(outputFileName, std::fstream::out);

   	// check if all files are open
   	if(inputFile.is_open() && outputFile.is_open()) {
   		// instantiate useful variables
   		std::vector<int> elements;
   		int nbElements;
   		int maxBlockSum;

   		// read content from the input file and initialize needed variables
   		readFile(elements, nbElements, maxBlockSum, inputFile);

   		inputFile.close();

   		// sort elements in blocks
   		std::vector<std::vector<int>> blocks = fourthSort(elements, maxBlockSum);

   		// output all block
   		outputFile << blocks.size() << std::endl;
   		for(int i = 0; i < blocks.size(); i++) {
   			std::vector<int> currentBlock = blocks.at(i);

   			for(int j = 0; j < currentBlock.size(); j++) {
   				outputFile << currentBlock.at(j) << std::endl;
   			}

   			outputFile << ' ' << std::endl;
   		}
   	} else {
   		std::cerr << "Impossible d'ouvrir le fichier" << std::endl;
   	}

   	return 0;
}