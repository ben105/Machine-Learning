from math import log

def calcShannonEnt(dataset):

	# Figure out how many entries there are in the 
	# dataset passed in.
	numEntries = len(dataset)

	# Keep track of how many of each distinct label
	# occurs in the dataset. We will keep track of 
	# this in the following dictionary.
	labelCounts = {}

	
	for featureVector in dataset:
		# We want to get the last label in the 
		# in the feature vector, because we are
		# assuming that this label is the class
		# identifier.
		classLabel = featureVector[-1]
		# Check if the classLabel is already in 
		# the dictionary of label counts. If it 
		# isn't then we should create a new entry.
		if classLabel not in labelCounts.keys():
			labelCounts[classLabel] = 0
		labelCounts[classLabel] += 1

	# Let's calculate the Shannon entropy.
	shannonEntropy = 0.0
	for key in labelCounts.keys():
		probability = float(labelCounts[key]) / numEntries
		shannonEntropy -= probability * log(probability, 2)
	
	# Return the final result
	return shannonEntropy


if __name__ == "__main__":
	dataset = [[1, 1, 'maybe'],
			[1, 1, 'yes'],
			[1, 0, 'no'],
			[0, 1, 'no'],
			[0, 1, 'no'],]

	entropy = calcShannonEnt(dataset)
	print("Entropy of dataset: {}".format(entropy))