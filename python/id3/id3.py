from math import log
import json
import sys

def calcShannonEnt(dataSet):
	numEntries = len(dataSet) 
	labelCounts = {}
	for featVec in dataSet:
		currentLabel = featVec[-1]
		if currentLabel not in labelCounts.keys():
			labelCounts[currentLabel] = 0
		labelCounts[currentLabel] += 1
	shannonEnt = 0.0
	for key, value in labelCounts.iteritems():
		prob = float(value)/numEntries
		shannonEnt -= prob*log(prob,2)
	return shannonEnt

def createDataset(filename):
	f = open(filename, "r")
	jsonData = json.load(f)
	return jsonData["dataSet"], jsonData["labels"]

def splitDataSet(dataSet, axis, value):
	retDataSet = []
	for featVec in dataSet:
		if featVec[axis] == value:
			reducedFeatVec = featVec[:axis]
			reducedFeatVec.extend(featVec[axis+1:])
			retDataSet.append(reducedFeatVec)
	return retDataSet

def chooseBestFeatureToSplit(dataSet):
	numFeatures = len(dataSet[0]) - 1
	baseEntropy = calcShannonEnt(dataSet)
	bestInfoGain = 0.0
	bestFeature = -1
	for i in xrange(numFeatures):
		featList = [example[i] for example in dataSet]
		uniqueVals = set(featList)
		newEntropy = 0.0
		for value in uniqueVals:
			subDataSet = splitDataSet(dataSet, i, value)
			prob = len(subDataSet)/float(len(dataSet))
			newEntropy += prob * calcShannonEnt(subDataSet)
		infoGain = baseEntropy - newEntropy
		if (infoGain > bestInfoGain):
			bestInfoGain = infoGain
			bestFeature = i
	return bestFeature

def createTree(dataSet, labels):
	classList = [example[-1] for example in dataSet]
	if classList.count(classList[0]) == len(classList):
		return classList[0]
	if len(dataSet[0]) == 1:
		return majorityCnt(classList)
	bestFeat = chooseBestFeatureToSplit(dataSet)
	bestFeatureLabel = labels[bestFeat]
	myTree = {bestFeatureLabel: {}}
	del(labels[bestFeat])
	featValues = [example[bestFeat] for example in dataSet]
	uniqueVals = set(featValues)
	for value in uniqueVals:
		subLabels = labels[:]
		myTree[bestFeatureLabel][value] = createTree(splitDataSet\
				(dataSet, bestFeat, value), subLabels)
	return myTree
		
def majorityCnt(classList):
	classCount = {}
	for vote in classList:
		if vote not in classCount.keys(): 
			classCount[vote] = 0
		classCount[vote] += 1
	sortedClassCount = sorted(classCount.iteritems(),key=operator.itemgetter(1),reverse=True)
	return sortedClassCount[0][0]

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("usage: python id3.py <filename>")
		quit()
	dataset, labels = createDataset(sys.argv[1])
	tree = createTree(dataset, labels)
	print(json.dumps(tree))
