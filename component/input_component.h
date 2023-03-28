#pragma once

class CompObject;

class InputComponent {
public:
	virtual ~InputComponent() {}
	virtual void Update(CompObject& object) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
	virtual void Update(CompObject& object);

private:
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;
};

class DemoInputComponent : public InputComponent {
public:
	virtual void Update(CompObject& bjorn);

private:
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;

	int m_direction = 1;
};