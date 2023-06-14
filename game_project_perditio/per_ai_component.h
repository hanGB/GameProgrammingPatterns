#pragma once
class PERObject;

class PERAiComponent {
public:
	virtual ~PERAiComponent() {}

	virtual void Update(PERObject& object, double dTime) = 0;
};