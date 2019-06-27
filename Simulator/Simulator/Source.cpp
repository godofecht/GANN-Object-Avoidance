#define _USE_MATH_DEFINES

#include "source.h"

vector <double> inputVals;

Hit Sprite::rayTrace(float angle)
{
	double dist = 0;
	float correctedAngle;
	correctedAngle = shape.getRotation() + angle;
	sf::Vector2f rayPos(shape.getPosition().x, shape.getPosition().y);

	for (float i = 1; i <= 200; i++)//number of iterations
	{
		rayPos.x += 1 * cos(correctedAngle*M_PI / 180);
		rayPos.y += 1 * sin(correctedAngle*M_PI / 180);
		dist += pow(pow(rayPos.x, 2)*pow(rayPos.y, 2), 1 / 2);
		for (int j = 0; j < 9; j++)
		{
			if (thisTrack->obstacles[j].polygon.getGlobalBounds().contains(rayPos))
			{
				Hit newHitResult(true, dist, rayPos);
				return newHitResult;
			}
		}
		for (int j = 0; j < 4; j++)
		{
			if (thisTrack->shapes[j].getGlobalBounds().contains(rayPos))
			{
				Hit newHitResult(true, dist, rayPos);
				return newHitResult;
			}
		}
	}
	Hit newHitResult(false, 0, rayPos);
	return newHitResult;
}

vector <double> rouletteWheel(vector<Sprite> SpritePopulation, vector<Network> Brains, int fitnessSum)
{
	int r;
	if (fitnessSum == 0)
		r = rand() % SpritePopulation.size() - 1;
	else
		r = rand() % fitnessSum;
	int iteratedSum = 0;
	for (int i = 0; i < SpritePopulation.size(); i++)
	{
		iteratedSum += SpritePopulation[i].fitness;
		if (iteratedSum >= r)
			return Brains[i].GetWeights();
	}
	return Brains[r].GetWeights();
}

int main()
{
	bool bIsFalling = true; int dist = 0;
	bool hbIsFalling = true; int hdist = 0;

	//Specifies which generation
	generationNum = 0;
	//Initialization of Topology Specifics
	topology.push_back(5); topology.push_back(4); topology.push_back(5);
	//Initializing Sprite Population from Class, as well as their respective neural networks. Also initializing obstacles/food.
	for (int i = 0; i < 8; i++)
	{
		Brains.push_back(Network(topology));
		newBrains.push_back(Network(topology));
		SpritePopulation.push_back(Sprite());
	}


	//Rendering Window
	double centerPosx, centerPosy, worldPosx, worldPosy;
	vector <double> position;

	//Initializes clock to set number of cycles.
	sf::Clock clock;
	sf::Time elapsedTime;


	//Text on Screen//
	///////////////
	sf::Font counterFont;
	counterFont.loadFromFile("arial.ttf");

	//Generation Counter
	sf::Text genCountText;
	sf::Text afpgText;

	genCountText.setString("0");
	genCountText.setFont(counterFont);
	genCountText.setPosition(30, 30);
	genCountText.setCharacterSize(30);
	genCountText.setColor(sf::Color::Red);

	afpgText.setString("0");
	afpgText.setFont(counterFont);
	afpgText.setPosition(30, 60);
	afpgText.setCharacterSize(30);
	afpgText.setColor(sf::Color::Yellow);
	//Time Counter
	sf::Text timeCountText;
	////////////////


	sf::Mouse Mouse;
	double posx, posy;
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	while (window.isOpen())
	{


		sf::Event event;
		while (window.pollEvent(event))
		{


			if (event.type == sf::Event::Closed)
				window.close();
		}

		elapsedTime = clock.getElapsedTime();

		bAllDead = true;
		for (int i = 0; i < SpritePopulation.size(); i++)
		{
			if (SpritePopulation[i].state == 1)
				bAllDead = false;
		}
		if (bAllDead || elapsedTime.asSeconds() >= 20)
		{
			for (int i = 0; i < SpritePopulation.size(); i++)
				fitnessSum += SpritePopulation[i].fitness;
			//Rank SpritePopulation and brains in ascending order of fitness
			for (int i = 0; i < SpritePopulation.size() - 1; i++)
			for (int j = 0; j < SpritePopulation.size() - 1; j++)
			{
				if (SpritePopulation[j].fitness >= SpritePopulation[j + 1].fitness)
				{
					Sprite tempSprite = SpritePopulation[j];
					SpritePopulation[j] = SpritePopulation[j + 1];
					SpritePopulation[j + 1] = tempSprite;
					Network tempNetwork = Brains[j];
					Brains[j] = Brains[j + 1];
					Brains[j + 1] = tempNetwork;
				}
			}
			//Perform Roulette Wheel Selections and obtain new weights by crossing over using the genetic algorithm.
			for (int i = 0; i < SpritePopulation.size(); i += 2)
			{
				parentA = rouletteWheel(SpritePopulation, Brains, fitnessSum);
				parentB = rouletteWheel(SpritePopulation, Brains, fitnessSum);
				children = Crossover(parentA, parentB);
				newBrains[i].PutWeights(children.childOne);
				if (i + 1 < SpritePopulation.size())
					newBrains[i + 1].PutWeights(children.childTwo);
			}
			//Update existing brains with the new values
			for (int i = 0; i < SpritePopulation.size(); i++)
			{
				SpritePopulation[i].fitness = 0;
				Brains[i].PutWeights(newBrains[i].GetWeights());
			}


			avgFitnessPerGeneration = fitnessSum / SpritePopulation.size();
			//Update clock and generation number
			clock.restart();
			generationNum++;
			genCountText.setString(to_string(generationNum));
			afpgText.setString(to_string(avgFitnessPerGeneration));

			//Reset positions of sprites
			for (int i = 0; i < SpritePopulation.size(); i++)
			{
				SpritePopulation[i].shape.setPosition(300, 500);
				SpritePopulation[i].state = 1;
			}
			//ResetFitnessSum
			fitnessSum = 0;


			thisTrack->obstacles[0].polygon.setPosition(sf::Vector2f(120, 200));
			thisTrack->obstacles[1].polygon.setPosition(sf::Vector2f(500, 300));
			thisTrack->obstacles[2].polygon.setPosition(sf::Vector2f(100, 600));
			thisTrack->obstacles[3].polygon.setPosition(sf::Vector2f(600, 480));
			thisTrack->obstacles[4].polygon.setPosition(sf::Vector2f(450, 360));
			thisTrack->obstacles[5].polygon.setPosition(sf::Vector2f(450, 600));
			thisTrack->obstacles[6].polygon.setPosition(sf::Vector2f(700, 400));
			thisTrack->obstacles[7].polygon.setPosition(sf::Vector2f(300, 157));
			thisTrack->obstacles[8].polygon.setPosition(sf::Vector2f(347, 460));
		}
		//////////////////////////////////////////////////
		//Update position of each Sprite in Population
		for (int i = 0; i < SpritePopulation.size(); i++)
		{

			//Mapping Inputs

			SpritePopulation[i].hit[0] = SpritePopulation[i].rayTrace(0); SpritePopulation[i].rays[0].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[0].line.append(SpritePopulation[i].hit[0].hitPos);
			SpritePopulation[i].hit[1] = SpritePopulation[i].rayTrace(-10); SpritePopulation[i].rays[1].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[1].line.append(SpritePopulation[i].hit[1].hitPos);
			SpritePopulation[i].hit[2] = SpritePopulation[i].rayTrace(10); SpritePopulation[i].rays[2].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[2].line.append(SpritePopulation[i].hit[2].hitPos);
			SpritePopulation[i].hit[3] = SpritePopulation[i].rayTrace(20); SpritePopulation[i].rays[3].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[3].line.append(SpritePopulation[i].hit[3].hitPos);
			SpritePopulation[i].hit[4] = SpritePopulation[i].rayTrace(-20); SpritePopulation[i].rays[4].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[4].line.append(SpritePopulation[i].hit[4].hitPos);
	//		SpritePopulation[i].hit[5] = SpritePopulation[i].rayTrace(180); SpritePopulation[i].rays[5].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[5].line.append(SpritePopulation[i].hit[5].hitPos);
	//		SpritePopulation[i].hit[6] = SpritePopulation[i].rayTrace(360); SpritePopulation[i].rays[6].line.append(SpritePopulation[i].shape.getPosition()); SpritePopulation[i].rays[6].line.append(SpritePopulation[i].hit[6].hitPos);
			inputVals.clear();
			for (int j = 0; j < 5; j++){
				inputVals.push_back(SpritePopulation[i].hit[j].hitDist);
			}
			Brains[i].feedForward(inputVals);
			Brains[i].getResults(SpritePopulation[i].resultVals);

			//Sprite Movement mapping to Neural Network
			//Mapping Outputs
			//	SpritePopulation[i].resultVals[0] > SpritePopulation[i].resultVals[1] ? SpritePopulation[i].shape.setRotation(SpritePopulation[i].shape.getRotation() + rotationSpeed*(SpritePopulation[i].resultVals[0])) : SpritePopulation[i].shape.setRotation(SpritePopulation[i].shape.getRotation() - rotationSpeed*(SpritePopulation[i].resultVals[1]));
			SpritePopulation[i].shape.setRotation(SpritePopulation[i].shape.getRotation() + rotationSpeed*(SpritePopulation[i].resultVals[0] - SpritePopulation[i].resultVals[1]));
			SpritePopulation[i].shape.setPosition(SpritePopulation[i].shape.getPosition().x + positionSpeed*SpritePopulation[i].resultVals[2] * cos(SpritePopulation[i].shape.getRotation()*M_PI / 180), SpritePopulation[i].shape.getPosition().y + positionSpeed*SpritePopulation[i].resultVals[2] * sin(SpritePopulation[i].shape.getRotation()*M_PI / 180));
		}
		for (int i = 0; i < SpritePopulation.size(); i++)
		{
			//Update fitness
			if (SpritePopulation[i].state == 1)
			{
				SpritePopulation[i].fitness++;
			}
			for (int j = 0; j < 9; j++)//6 obstacles
			{
				//Kill Sprite if it hits obstacles or boundary
				if (thisTrack->obstacles[j].polygon.getGlobalBounds().contains(SpritePopulation[i].shape.getPosition()))
					SpritePopulation[i].state = 0;
			}
			for (int j = 0; j < 4; j++)//4 Bounding Lines
			{
				if (thisTrack->shapes[j].getGlobalBounds().contains(SpritePopulation[i].shape.getPosition()))
					SpritePopulation[i].state = 0;
			}
		}
		for (int i = 0; i < SpritePopulation.size(); i++)
		{
			if (SpritePopulation[i].state == 1)
				SpritePopulation[i].fitness++;
		}
		///////////////////////////

		window.clear();
		//Draw every shape of every Sprite.
		for (int i = 0; i < SpritePopulation.size(); i++)
		{
			//draw Sprite
			if (SpritePopulation[i].state == 1)
			{
				window.draw(SpritePopulation[i].shape);
				for (int j = 0; j < 7; j++)
				{
					window.draw(SpritePopulation[i].rays[j].line);
					SpritePopulation[i].rays[j].line.clear();
				}
			}
		}
		for (int i = 0; i < 9; i++)
		{
			//Update position of blocks
			dist++;
			if (dist<20000)
				thisTrack->obstacles[i].polygon.setPosition(thisTrack->obstacles[i].polygon.getPosition().x+.1, thisTrack->obstacles[i].polygon.getPosition().y - .1);
			else if (dist<40000)
				thisTrack->obstacles[i].polygon.setPosition(thisTrack->obstacles[i].polygon.getPosition().x+.1, thisTrack->obstacles[i].polygon.getPosition().y + .1);
			else if (dist<60000)
				thisTrack->obstacles[i].polygon.setPosition(thisTrack->obstacles[i].polygon.getPosition().x-0.1, thisTrack->obstacles[i].polygon.getPosition().y + .1);
			else if (dist<80000)
				thisTrack->obstacles[i].polygon.setPosition(thisTrack->obstacles[i].polygon.getPosition().x-0.1, thisTrack->obstacles[i].polygon.getPosition().y - .1);
			if (dist == 60000)
				dist = 0;
		}
		for (int i = 0; i < 4; i++)//4 bounding lines
		{
			window.draw(thisTrack->shapes[i]);
		}
		window.draw(genCountText);
		window.draw(afpgText);
		for (int i = 0; i < 9; i++)//6 obstacles
		{
			window.draw(thisTrack->obstacles[i].polygon);
		}
		window.draw(afpgText);
		window.display();
	}
	getch();
	return 0;
}