#ifndef Source_H
#define Source_H

#include<math.h>
#include<cmath>
#include<cassert>
#include<iostream>
#include <SFML/Graphics.hpp>
#include "NN.h"
#include<vector>
#include<conio.h>
#include "GenAlg.h"
#include "Source.h"
#include<SFML/Graphics/Shape.hpp>

using namespace std;

float avgFitnessPerGeneration;
bool bAllDead;


//start and destination points

sf::Vector2f startPos(300, 500);
sf::Vector2f destination(500, 300);
//////////////////////////////


int fitnessSum = 0;
float positionSpeed=0.5, rotationSpeed=200;
//Initialize topology of Neural Network
vector<unsigned> topology;

vector<Network> Brains;
vector<Network> newBrains;

vector<double> parentA;
vector<double> parentB;

Children children(parentA,parentB);

int generationNum;
const int numRays = 7;
//Rendering Window
sf::RenderWindow window(sf::VideoMode(1200, 1000), "ADA Project");

class Hit
{
public:
	bool bHit;
	double hitDist;
	sf::Vector2f hitPos;
	Hit()
	{
		bHit = false;
		hitDist = 0;
		hitPos = sf::Vector2f(0,0);
	}
	Hit(bool bh, double hd,sf::Vector2f hp)
	{
		bHit = bh;
		hitDist = hd;
		hitPos = hp;
	}
};
class Ray
{
public:
	sf::VertexArray line;
	Ray()
	{
		sf::VertexArray line(sf::LinesStrip, 2);
		line[0].color = sf::Color::Green;
		line[1].color = sf::Color::Green;
	}
};
//Initializing Class Sprite
class Sprite{
public:
	vector<double> inputVals;
	vector<double> targetVals;
	vector<double> resultVals;
	sf::RectangleShape shape;
	Ray rays[numRays];


	float distFromDestination;

	Hit hit[5];
	int fitness;
	int state;
	Sprite()
	{
		state = 1;
		distFromDestination = pow(pow(shape.getPosition().x, 2) + pow(shape.getPosition().y, 2),1/2);
		for (int i = 0; i < 5; i++)
		{
			rays[i].line.append(shape.getPosition());
		}
		shape.setSize(sf::Vector2f(50,30));
		shape.setFillColor(sf::Color(300, 300, 300));
		shape.setPosition(sf::Vector2f(300,500));
		shape.setOrigin(shape.getSize() / 2.0f);
		shape.setRotation(45);
		fitness = 0;
	}
	Hit rayTrace(float angle);
//	void isGettingCloser()
//	{
//		float newDist = pow(pow(shape.getPosition().x, 2) + pow(shape.getPosition().y, 2),1/2);
//		if (distFromDestination > newDist)
//			fitness += (newDist - distFromDestination);
//
//		else if (distFromDestination <= newDist)
//			fitness -= (distFromDestination - newDist);
//
//		distFromDestination = newDist;
//	}
};
class DirectionVec
{
	int xVec, yVec;
public:
	DirectionVec(float x, float y)
	{
		xVec = x; yVec = y;
	}
	float getXDir()
	{
		if (xVec > 0) return 0.1;
		return -0.1;
	}
	float getYDir()
	{
		if (yVec > 0) return 0.1;
		return -0.1;
	}
};
DirectionVec getVec(float spriteposx,float spriteposy,float blockposx,float blockposy)
{
	float xDiff, yDiff;
	xDiff = spriteposx - blockposx;
	yDiff = spriteposy = blockposy;
	DirectionVec thisDirectionVec(xDiff, yDiff);
	return thisDirectionVec;
}
class Food{
public:
	sf::RectangleShape shape;
	Food()
	{
		shape.setSize(sf::Vector2f(100, 30));
		shape.setFillColor(sf::Color::Blue);
	}
};
class Obstacle{
public:
	sf::RectangleShape polygon;
	Obstacle()
	{
		polygon.setSize(sf::Vector2f(100, 100));
	}
};

class Track{
public:
	sf::RectangleShape shapes[4];
	Obstacle obstacles[9];
	Track()
	{

		shapes[0].setSize(sf::Vector2f(800, 5));
		shapes[2] = shapes[0];
		shapes[1].setSize(sf::Vector2f(5, 800));
		shapes[3] = shapes[1];
		shapes[0].setPosition(sf::Vector2f(100, 50));
		shapes[2].setPosition(sf::Vector2f(100, 850));
		shapes[3].setPosition(sf::Vector2f(100, 50));
		shapes[1].setPosition(sf::Vector2f(900, 50));
		obstacles[0].polygon.setPosition(sf::Vector2f(120,200));
		obstacles[1].polygon.setPosition(sf::Vector2f(200, 300));
		obstacles[2].polygon.setPosition(sf::Vector2f(300, 600));
		obstacles[3].polygon.setPosition(sf::Vector2f(120, 480));
		obstacles[4].polygon.setPosition(sf::Vector2f(200, 560));
		obstacles[5].polygon.setPosition(sf::Vector2f(300, 700));
		obstacles[6].polygon.setPosition(sf::Vector2f(120, 120));
		obstacles[7].polygon.setPosition(sf::Vector2f(200, 35057));
		obstacles[8].polygon.setPosition(sf::Vector2f(300, 660));
	}
};
vector <Food> FoodPopulation;
vector <Sprite> SpritePopulation;

vector<double> rouletteWheel(vector<Sprite> SpritePopulation,vector<Network>Brains,int fitnessSum);
Track *thisTrack = new Track();
#endif