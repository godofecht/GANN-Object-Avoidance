#include<vector>
#include<cmath>
#include<cassert>
#include<iostream>
#include "NN.h"
#include<time.h>
#include<stdlib.h>
using namespace std;


Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}

void Neuron::feedForward(Layer &prevLayer)
{
	double sum = 0.0;

	// We sum the previous layer's output
	// Include the bias node from the previous layer.

	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		sum += prevLayer[n].getOutputVal() *
			prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = Neuron::transferFunction(sum);
}

double Neuron::getOutputVal()
	{
		return m_outputVal;
	}
void Neuron::setOutputVal(double n)
	{
		m_outputVal = n;
	}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}
void Neuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}
void Neuron::updateInputWeights(Layer &prevLayer) //Formulas adapted into code.
{
	for (unsigned n = 0; n < prevLayer.size(); ++n) {
		Neuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaweight;


		double newDeltaWeight =
			// Individual input is magnified by the gradient and train rate:
			eta
			* neuron.getOutputVal()
			* m_gradient
			// Also adding momentum = a fraction of the previous delta weight;
			+ alpha
			* oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaweight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}


double Neuron::randomWeight()
	{
		return (rand() / double(RAND_MAX));
	}
double Neuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;

	// Sum our contributions of the errors at the nodes we feed.

	for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}

	return sum;
}
double Neuron::transferFunctionDerivative(double x)
	{
		return 1 - x*x;
	}
double Neuron::transferFunction(double x)
	{
		return tanh(x);
	}
double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;


Network::Network(const vector <unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have a new layer, now fill it with neurons, and a bias neuron in each layer.
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Made a Neuron!" << endl;
		}

		// Force the bias node's output to 1.0 (it was the last neuron pushed in this layer):
		m_layers.back().back().setOutputVal(1.0);
	}
}
void Network::backPropagate(const vector <double> &targetVals)
{
	// Calculate overall net error (RMS of output neuron errors)

	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS

	// Implement a recent average measurement

	m_recentAverageError =
		(m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
		/ (m_recentAverageSmoothingFactor + 1.0);

	// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}

	// Calculate hidden layer gradients

	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum) {
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum) {
		Layer &layer = m_layers[layerNum];
		Layer &prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n) {
			layer[n].updateInputWeights(prevLayer);
		}
	}
}
void Network::feedForward(vector <double> &inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1);

	// Assign (latch) the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); ++i) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	// forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum) {
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}
void Network::getResults(vector <double> &resultVals)
{
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

vector<double> Network::GetWeights() const
{
	//this will hold the weights
	vector<double> weights;

	//for each layer
	for (int i = 0; i<m_layers.size()-1; ++i)
	{
		//for each neuron
		for (int j = 0; j<m_layers[i].size(); ++j)
		{
			//for each weight
			for (int k = 0; k<m_layers[i][j].m_outputWeights.size(); ++k)
			{
				weights.push_back(m_layers[i][j].m_outputWeights[k].weight);
			}
		}
	}

	return weights;
}

void Network::PutWeights(vector<double> &weights)
{
	int cWeight = 0;

	//for each layer
	for (int i = 0; i<m_layers.size()-1; ++i)
	{

		//for each neuron
		for (int j = 0; j<m_layers[i].size(); ++j)
		{
			//for each weight
			for (int k = 0; k<m_layers[i][j].m_outputWeights.size(); ++k)
			{
				m_layers[i][j].m_outputWeights[k].weight = weights[cWeight++];
			}
		}
	}

	return;
}


double m_recentAverageSmoothingFactor = 100.0;