#include <cstdio>
#include <map>
#include <vector>
#include <math.h>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

typedef vector<vector<string> > data;

bool _classCountCompare(const pair<string, int>  &p1, const pair<string, int> &p2);
void printTree(void* tree);


class ClassificationTree {
	typedef map<string, ClassificationTree*> branchMap;
public:
	string classification;
	string feature;
	branchMap children;

	ClassificationTree(){
		this->classification = "";
		this->feature = "";
	}

	void print() {
		if (this->classification != "") {
			cout << " >>> " << this->classification << endl;
		} else {
			for (branchMap::iterator it = children.begin(); it != children.end(); ++it) {
				cout << this->feature << " - " << it->first << " - " ;
				it->second->print();
			}
		}
	}
};


double calcShannonEnt(data dataSet) {
	int numEntries = dataSet.size();
	map<string, int> labelCounts;
	for (data::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
		string currentLabel = it->back();
		if (labelCounts.find(currentLabel) == labelCounts.end()) {
			labelCounts[currentLabel] = 0;
		}
		labelCounts[currentLabel] += 1;
	}
	double shannonEnt = 0.0;
	for (map<string, int>::iterator it = labelCounts.begin(); it != labelCounts.end(); ++it) {
		double prob = float(it->second) / numEntries;
		shannonEnt -= prob * log2(prob);
	}
	return shannonEnt;
}

data splitDataSet(data dataSet, unsigned int axis, const string value) {
	data retDataSet = {};
	for (data::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
		if ( (*it)[axis] == value ) {
			vector<string> reducedFeatVec;
			reducedFeatVec.reserve(it->size());
			copy(it->begin(), it->end(), back_inserter(reducedFeatVec));
			reducedFeatVec.erase(reducedFeatVec.begin() + axis);
			retDataSet.push_back(reducedFeatVec);
		}
	}
	return retDataSet;
}

int chooseBestFeatureToSplit(data dataSet) {
	int numFeatures = dataSet[0].size() - 1;
	double baseEntropy = calcShannonEnt(dataSet);
	double bestInfoGain = 0.0;
	int bestFeature = -1;
	for ( unsigned int i = 0; i < numFeatures; ++i) {
		set<string> uniqueVals;
		for (data::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
			uniqueVals.insert((*it)[i]);
		}
		double newEntropy = 0.0;
		for (set<string>::iterator it = uniqueVals.begin(); it != uniqueVals.end(); ++it) {
			data subDataSet = splitDataSet(dataSet, i, *it);
			double prob = float(subDataSet.size()) / float(dataSet.size());
			newEntropy += prob * calcShannonEnt(subDataSet);
		}
		double infoGain = baseEntropy - newEntropy;
		if (infoGain > bestInfoGain) {
			bestInfoGain = infoGain;
			bestFeature = i;
		}
	}
	return bestFeature;
}

string majorityCnt(vector<string> classList) {
	map<string, int> classCount;
	for (vector<string>::iterator it = classList.begin(); it != classList.end(); ++it) {
		string vote = *it;
		if (classCount.find(vote) == classCount.end()) {
			classCount[vote] = 0;
		}
		classCount[vote] += 1;
	}
	vector< pair<string, int> > classCountVector(classCount.size());
	copy(classCount.begin(), classCount.end(), back_inserter(classCountVector));
	sort(classCountVector.begin(), classCountVector.end(), _classCountCompare);
	return classCountVector[0].first;
	// return "";
}

ClassificationTree* createTree(data dataSet, vector<string> labels) {
	ClassificationTree *tree = new ClassificationTree;

	vector<string> classList;
	for (data::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
		classList.push_back(it->back());
	}
	if (count(classList.begin(), classList.end(), classList[0]) == classList.size()) {
		tree->classification = classList[0];
		cout << classList[0] << endl;
		return tree;
	}
	if (dataSet[0].size() == 1) {
		tree->classification = majorityCnt(classList);
		return tree;
	}
	int bestFeat = chooseBestFeatureToSplit(dataSet);
	string bestFeatureLabel = labels[bestFeat];
	tree->feature = bestFeatureLabel;

	labels.erase(labels.begin() + bestFeat);
	set<string> uniqueVals;
	for (data::iterator it = dataSet.begin(); it != dataSet.end(); ++it) {
		uniqueVals.insert((*it)[bestFeat]);
	}
	for (set<string>::iterator it = uniqueVals.begin(); it != uniqueVals.end(); ++it) {
		vector<string> subLabels;
		copy(labels.begin(), labels.end(), back_inserter(subLabels));
		cout << bestFeatureLabel << endl;
		tree->children[*it] = createTree(splitDataSet(dataSet, bestFeat, *it), subLabels);
	}
	return tree;
}

bool _classCountCompare(const pair<string, int>  &p1, const pair<string, int> &p2)
{
    return p1.second > p2.second;
}

int main(int argc, char **argv) {

	data dataSet = {
		{"yes", "yes", "yes"},
		{"yes", "yes", "yes"},
		{"yes", "no", "no"},
		{"no", "yes", "no"},
		{"no", "yes", "no"}
	};

	ClassificationTree *tree = createTree(dataSet, { "breath underwater", "has fins" });
	printf("Shannon entropy of data: %p \n", tree);
	tree->print();
	return 0;
}