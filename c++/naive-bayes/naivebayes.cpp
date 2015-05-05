#include <vector>
#include <set>
#include <iostream>

using namespace std;

vector<string> createVocabList(vector< vector<string> > dataSet) {
	set<string> vocabSet;
	// Iterate through the dataSet, one document at a time, adding the words
	// to the vocabulary set.
	for (vector< vector<string> >::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
		vector<string> &document = *it;
		for (vector<string>::iterator it2 = document.begin(); it2 != document.end(); ++it2) {
			vocabSet.insert(*it2);
		}
	}

	// Put the vocab set back into the vocab list.
	vector<string> vocabList;
	copy(vocabSet.begin(), vocabSet.end(), back_inserter(vocabList));
	return vocabList;
}

vector<int> setOfWords2Vec(vector<string> vocabList, vector<string> inputList) {
	//Initialize the word vector to all 0's.
	vector<int> wordVector(vocabList.size());

	// Iterate through each word in the input list. 
	// As we come across each word, we want to check the index in the vocab list.
	// At this index, we increment the count in the word vector.
	for (vector<string>::iterator it = inputList.begin(); it != inputList.end(); ++it) {
		string &word = *it;
		// Find the word in the vocabulary list.
		auto it = find(vocabList.begin(), vocabList.end(), word);
		if ( it == vocabList.end()) {
			cout << "The word " << word << " is not in my vocabulary!" << endl;
		} else {
			auto index = distance(vocabList.begin(), it);
			wordVector[index] += 1;
		}
	}
	return wordVector;
}

int main(int argc, char **argv) {
	vector< vector<string> > postingList = {
		{"my", "dog", "has", "flea", "problems", "help", "please"},
		{"maybe", "not", "take", "him", "to", "dog", "park", "stupid"},
		{"my", "dalmation", "is", "so", "cute", "I", "love", "him"},
		{"stop", "posting", "stupid", "worthless", "garbage"},
		{"mr", "licks", "ate", "my", "steak", "how", "to", "stop", "him"},
		{"quit", "buying", "worthless", "dog", "food", "stupid"}
	};
	vector<int> classVec = { 0, 1, 0, 1, 0, 1};

	vector<string> vocabList = createVocabList(postingList);
	for (vector<string>::iterator it = vocabList.begin(); it != vocabList.end(); ++it) {
		cout << *it << endl;
	}
	return 0;
}