#ifndef NN_H
#define NN_H

#include<vector>
#include<cmath>
#include<cassert>
#include<iostream>
using namespace std;

class Neuron;
typedef vector<Neuron> Layer;
class connection
{
public:
	double weight;
	double deltaweight;
};
class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void feedForward(Layer &prevLayer);
	double getOutputVal();
	void setOutputVal(double n);
	void calcHiddenGradients(const Layer &nextLayer);
	void calcOutputGradients(double targetVal);
	void updateInputWeights(Layer &prevLayer);
	static double eta;
	static double alpha;
	vector <connection> m_outputWeights;
private:
	double m_gradient;
	double m_outputVal;
	static double randomWeight();
	unsigned m_myIndex;
	double sumDOW(const Layer &nextLayer) const;
	static double transferFunctionDerivative(double x);
	static double transferFunction(double x);
};
class Network
{
	public:
		Network(const vector <unsigned> &topology);
		void backPropagate(const vector <double> &targetVals);
		void feedForward(vector <double> &inputVals);
		void getResults(vector <double> &resultVals);
		double getRecentAverageError(void) const { return m_recentAverageError; }

		vector<double> Network::GetWeights() const;
		void Network::PutWeights(vector<double> &weights);

	private:
		vector <Layer> m_layers;
		double m_gradient;
		double m_error;
		double m_recentAverageError;
		double m_recentAverageSmoothingFactor;
};

#endif