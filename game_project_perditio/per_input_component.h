#pragma once
class PERObject;
class PERController;

class PERInputComponent {
public:
	virtual ~PERInputComponent() {}

	virtual void Update(PERObject& object, PERController& controller) = 0;
};