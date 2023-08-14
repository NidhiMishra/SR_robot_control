#pragma once
#include <string>
#include <vector>
class Joint
{
public:
	Joint(unsigned char aDefaultPos);
	~Joint(void);
	void addTrajectory(std::vector<unsigned char> aTrajectory);//, int size);
	void addIdleMovement(std::vector<unsigned char> aTrajectory);
	void addFrame(unsigned char aFrame);
	void clearTrajectory();
	unsigned char getFrame();
	void updateDefaultPos(unsigned char aPos);
	std::string id;
	unsigned char defaultPos;
	std::vector<unsigned char> trajectory;
	int state;//0 idle, 1 Moving
};

