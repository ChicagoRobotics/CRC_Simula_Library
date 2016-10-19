// BehaviorTree.h

#ifndef _BEHAVIORTREE_h
#define _BEHAVIORTREE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Motor.h"
#include "CRC_Sensors.h"
#include "CRC_AudioManager.h"
#include "CRC_Lights.h"
#include <StandardCplusplus.h>
#include <list>
#include <vector>
#include <initializer_list>
#include <algorithm>

struct TREE_STATE {
	bool treeActive = false;
};
extern struct TREE_STATE treeState;

class Behavior_Tree {  // Note:  A proper copy constructor and assignment operator should be defined, since the implicit ones use shallow copies only.
private:
	
public:
	class Node {  // This class represents each node in the behaviour tree.
	public:
		virtual bool run() = 0;
	};

	class CompositeNode : public Node {  //  This type of Node follows the Composite Pattern, containing a list of other Nodes.
	private:
		std::vector<Node*> children;
	public:
		const std::vector<Node*>& getChildren() const { return children; }
		void addChild(Node* child) { children.push_back(child); }
		void addChildren(std::initializer_list<Node*>&& newChildren) { for (Node* child : newChildren) addChild(child); }
		template <typename CONTAINER>
		void addChildren(const CONTAINER& newChildren) { for (Node* child : newChildren) addChild(child); }
	protected:
		std::vector<Node*> childrenShuffled() const { 
			std::vector<Node*> temp = children;  
			std::random_shuffle(temp.begin(), temp.end());  
			return temp; 
		}
	};

	class Selector : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) {  // The generic Selector implementation
				if (child->run())  // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
					return true;
			}
			return false;  // All children failed so the entire run() operation fails.
		}
	};

	class RandomSelector : public CompositeNode {  //Shuffles children prior to running.
	public:
		virtual bool run() override {
			for (Node* child : childrenShuffled()) {
				if (child->run())
					return true;
			}
			return false;
		}
	};

	class Sequence : public CompositeNode {
	public:
		virtual bool run() override {
			for (Node* child : getChildren()) {  // The generic Sequence implementation.
				if (!child->run())  // If one child fails, the entire operation run() fails.  Success only results if all children succeed.
					return false;
			}
			return true;  // All children suceeded, so the entire run() operation succeeds.
		}
	};

	class Root : public Node {
	private:
		Node* child;
		friend class Behavior_Tree;
		void setChild(Node* newChild) { child = newChild; }
		virtual bool run() override { return child->run(); }
	};
private:
	Root* root;
public:
	Behavior_Tree() : root(new Root) {}
	void setRootChild(Node* rootChild) const { root->setChild(rootChild); }
	bool run() const { return root->run(); }
};
class Button_Stop : public Behavior_Tree::Node {
private:
	bool buttonState = true;
	int lastButtonState = HIGH;
	unsigned long debounceTime;
	const long debounceDelay = 10;
	virtual bool run() override {
		int reading = digitalRead(hardware.pinButton);
		if (reading != lastButtonState) {
			debounceTime = millis();
		}

		if ((millis() - debounceTime) > debounceDelay) {
			if (reading != buttonState) {
				buttonState = reading;
				if (buttonState == HIGH) {
					treeState.treeActive = !treeState.treeActive;
					if (!treeState.treeActive)
					{
						Serial.println(F("Behavior tree off."));
						motors.allStop();
						sensors.deactivate();
						simulation.showLedNone();
						crcLights.setButtonLevel(0);
					}
					else {
						Serial.println(F("Activating behavior tree."));
						sensors.activate();
						simulation.showLedBio();
						delay(50);
						//return true to allow sensors to read before next tree loop.
						return true;
					}
				}
			}
		}
		lastButtonState = reading;
		return !treeState.treeActive;
	}
};
class Battery_Check : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	unsigned long now;
	unsigned long lastCheck = 0;
	int interval = 10000;
	virtual bool run() override {
		now = millis();
		if (!nodeActive) {
			if ((lastCheck == 0) || (now > lastCheck + interval)) {
				lastCheck = now;
				float postVoltage = hardware.readBatteryVoltage();
				if (postVoltage < hardware.lowBatteryVoltage) {
					nodeActive = true;
					Serial.print(F("Batteries low. Measurement: "));
					Serial.println(postVoltage);
					Serial.print(F("below threshold of: "));
					Serial.println(hardware.lowBatteryVoltage);
					crcAudio.playRandomAudio("effects/PwrDn_", 10, ".mp3");
				}
			}
		}
		return nodeActive;
	}
};

class Orientation_Check : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	const int Z_Orient_Min = 15000;
	virtual bool run() override {

		if ((sensors.lsm.accelData.z < Z_Orient_Min) && (!motors.active()) && (!crcAudio.isPlayingAudio())) {
			crcAudio.playRandomAudio(F("emotions/scare_"), 9, F(".mp3"));
			//Serial.print("Z: ");
			//Serial.println(sensors.lsm.accelData.z);
			nodeActive = true;
		}
		else {
			nodeActive = false;
		}
		return nodeActive;

		/*Serial.print("Accel X: "); Serial.print((int)sensors.lsm.accelData.x); Serial.print(" ");
		Serial.print("Y: "); Serial.print((int)sensors.lsm.accelData.y);       Serial.print(" ");
		Serial.print("Z: "); Serial.println((int)sensors.lsm.accelData.z);     Serial.print(" ");
		Serial.print("Mag X: "); Serial.print((int)sensors.lsm.magData.x);     Serial.print(" ");
		Serial.print("Y: "); Serial.print((int)sensors.lsm.magData.y);         Serial.print(" ");
		Serial.print("Z: "); Serial.println((int)sensors.lsm.magData.z);       Serial.print(" ");
		Serial.print("Gyro X: "); Serial.print((int)sensors.lsm.gyroData.x);   Serial.print(" ");
		Serial.print("Y: "); Serial.print((int)sensors.lsm.gyroData.y);        Serial.print(" ");
		Serial.print("Z: "); Serial.println((int)sensors.lsm.gyroData.z);      Serial.print(" ");
		Serial.print("Temp: "); Serial.print((int)sensors.lsm.temperature);    Serial.println(" ");*/
	}
};

class Cliff_Center : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	const long duration = 200;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	virtual bool run() override {

		currentTime = millis();

		if (!nodeActive) {
			if (sensors.irLeftCliff && sensors.irRightCliff && motors.active()) {
				nodeActive = true;
				Serial.println(F("Cliff center detected."));
				nodeStartTime = currentTime;
				motors.setPower(-120, -120);
			}
		}
		else
		{
			if ((nodeStartTime + duration < currentTime) && (!sensors.irLeftCliff && !sensors.irRightCliff)) {
				Serial.println(F("Cliff center complete."));
				nodeActive = false;
				nodeStartTime = 0;
				motors.allStop();
			}
		}
		return nodeActive;
	}
};
class Cliff_Left : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	const long backDuration = 400;
	const long turnDuration = 400;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	bool turnStarted = false;
	virtual bool run() override {

		currentTime = millis();

		if (!nodeActive) {
			if (sensors.irLeftCliff && !sensors.irRightCliff && motors.active()) {
				Serial.println(F("Cliff left detected."));
				nodeStartTime = currentTime;
				nodeActive = true;
				motors.setPower(-120, -120);
			}
		}
		else {
			if ((nodeStartTime + backDuration < currentTime) && !turnStarted) {
				Serial.println(F("Cliff left turning."));
				turnStarted = true;
				motors.setPower(180, -180);
			}
			if (nodeStartTime + backDuration + turnDuration < currentTime) {
				Serial.println(F("Cliff left stopping."));
				nodeStartTime = 0;
				motors.allStop();
				nodeActive = false;
				turnStarted = false;
			}
		}
		return nodeActive;
	}
};
class Cliff_Right : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	const long backDuration = 400;
	const long turnDuration = 400;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	bool turnStarted = false;
	virtual bool run() override {

		currentTime = millis();
		if (!nodeActive) {
			if (!sensors.irLeftCliff && sensors.irRightCliff && motors.active()) {
				Serial.println(F("Cliff right detected."));
				nodeStartTime = currentTime;
				nodeActive = true;
				motors.setPower(-120, -120);
			}
		}
		else {
			if ((nodeStartTime + backDuration < currentTime) && !turnStarted) {
				Serial.println(F("Cliff right turning."));
				turnStarted = true;
				motors.setPower(-180, 180);
			}
			if (nodeStartTime + backDuration + turnDuration < currentTime) {
				Serial.println(F("Cliff right stopping."));
				nodeStartTime = 0;
				motors.allStop();
				nodeActive = false;
				turnStarted = false;
			}
		}
		return nodeActive;
	}
};

class Perimeter_Center : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	uint8_t alarmCM = 11;
	const long duration = 200;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive) {
			if (sensors.irFrontCM < alarmCM && sensors.irFrontCM > hardware.irMinimumCM) {
				nodeStartTime = currentTime;
				nodeActive = true;
				Serial.print(F("Permimeter center alarm: "));
				Serial.println(sensors.irFrontCM);
				//50% chance of turning either directon
				long randNum = random(1, 101);

				Serial.print(F("random number: "));
				Serial.println(randNum);
				if (randNum <= 50) {
					Serial.println(F("Turning left."));
					motors.setPower(-160, 160);
				}
				else
				{
					Serial.println(F("Turning right."));
					motors.setPower(160, -160);
				}
				return nodeActive;
			}
		}
		else {
			if ((nodeStartTime + duration < currentTime) && (sensors.irFrontCM >= alarmCM)) {
				Serial.println(F("Perimeter center complete."));
				motors.allStop();
				nodeStartTime = 0;
				nodeActive = false;
			}
		}
		return nodeActive;
	}
};
class Perimeter_Left : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	uint8_t alarmCM = 11;
	const long duration = 200;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive) {
			if (sensors.irLeftFrontCM < alarmCM && sensors.irLeftFrontCM > hardware.irMinimumCM) {
				nodeStartTime = currentTime;
				nodeActive = true;
				Serial.print(F("Permimeter left front alarm: "));
				Serial.println(sensors.irLeftFrontCM);
				motors.setPower(160, -160);
			}
		}
		else {
			if ((nodeStartTime + duration < currentTime) && sensors.irLeftFrontCM >= alarmCM) {
				Serial.println(F("Perimeter left front complete."));
				motors.allStop();
				nodeStartTime = 0;
				nodeActive = false;
			}
		}
		return nodeActive;
	}
};
class Perimeter_Right : public Behavior_Tree::Node {
private:
	bool nodeActive = false;
	uint8_t alarmCM = 11;
	const long duration = 200;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive) {
			if (sensors.irRightFrontCM < alarmCM && sensors.irRightFrontCM > hardware.irMinimumCM) {
				nodeStartTime = currentTime;
				nodeActive = true;
				Serial.print(F("Permimeter right front alarm: "));
				Serial.println(sensors.irRightFrontCM);
				motors.setPower(-160, 160);
			}
		}
		else {
			if ((nodeStartTime + duration < currentTime) && sensors.irRightFrontCM >= alarmCM) {
				Serial.println(F("Perimeter right front complete."));
				motors.allStop();
				nodeStartTime = 0;
				nodeActive = false;
			}
		}
		return nodeActive;
	}
};

class Do_Nothing : public Behavior_Tree::Node {
	//This function is checked every checkInterval to see if it should run.
	//The interval is randomized, as is the duration of the time doing nothing.
public:
	Do_Nothing(int chance) : percentChance(chance) {}
private:
	int percentChance;

	bool nodeActive = false;
	long duration = 1000;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;

	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive && !simulation.actionActive) {
			long randNum = random(1, 101);
			Serial.print(F("Do_Nothing number: "));
			Serial.println(randNum);
			if (randNum <= percentChance) {
				nodeActive = true;
				simulation.actionActive = true;
				nodeStartTime = currentTime;
				Serial.println(F("Do_Nothing active."));
			}
		}
		if (nodeActive && (nodeStartTime + duration < currentTime)) {
			Serial.println(F("Do_Nothing finished."));
			simulation.actionActive = false;
			nodeActive = false;
			nodeStartTime = 0;
		}
		return nodeActive;
	}
};

class Forward_Random : public Behavior_Tree::Node {
	//This function is checked every checkInterval to see if it should run.
	//The interval is randomized, as is the duration of the time doing nothing.
public:
	Forward_Random(int chance) : percentChance(chance) {}
private:
	int percentChance;

	bool nodeActive = false;
	long duration;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;

	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive && !simulation.actionActive) {
			long randNum = random(1, 101);
			duration = random(100, 2000);
			Serial.print(F("Forward_Random number: "));
			Serial.println(randNum);
			if (randNum <= percentChance) {
				nodeActive = true;
				simulation.actionActive = true;
				nodeStartTime = currentTime;
				Serial.println(F("Forward_Random active."));
				motors.setPower(160, 160);
			}
		}
		if (nodeActive && (nodeStartTime + duration < currentTime)) {
			Serial.println(F("Forward_Random finished."));
			simulation.actionActive = false;
			nodeActive = false;
			nodeStartTime = 0;
		}
		return nodeActive;
	}
};

class Turn_Random : public Behavior_Tree::Node {
	//This function is checked every checkInterval to see if it should run.
	//The interval is randomized, as is the duration of the time doing nothing.
public:
	Turn_Random(int chance, bool clockwise) : percentChance(chance), _clockwise(clockwise) {}
private:
	int percentChance;
	bool _clockwise;

	bool nodeActive = false;
	long duration;
	unsigned long currentTime;
	unsigned long nodeStartTime = 0;
	
	virtual bool run() override {
		currentTime = millis();
		if (!nodeActive && !simulation.actionActive){
			long randNum = random(1, 101);
			Serial.print(F("turnRandom number: "));
			Serial.println(randNum);
			if (randNum <= percentChance) {
				duration = random(50, 1500);
				nodeActive = true;
				simulation.actionActive = true;
				nodeStartTime = currentTime;
				Serial.println(F("turnRandom active."));
				if (_clockwise) {
					motors.setPower(-160, 160);
				}
				else {
					motors.setPower(160, -160);
				}
				
			}
		}
		if(nodeActive && (nodeStartTime + duration < currentTime)){
			Serial.println(F("turnRandom finished."));
			simulation.actionActive = false;
			motors.allStop();
			nodeActive = false;
			nodeStartTime = 0;
		}
		return nodeActive;
	}
};




#endif

