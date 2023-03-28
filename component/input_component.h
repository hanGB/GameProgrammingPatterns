#pragma once

class Bjorn;

class InputComponent {
public:
	virtual ~InputComponent() {}
	virtual void Update(Bjorn& bjorn) = 0;
};

class PlayerInputComponent : public InputComponent {
public:
	virtual void Update(Bjorn& bjorn);

private:
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;
};

class DemoInputComponent : public InputComponent {
public:
	virtual void Update(Bjorn& bjorn);

private:
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;

	int m_direction = 1;
};