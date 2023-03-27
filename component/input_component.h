#pragma once

class Bjorn;

class InputComponent {
public:
	void Update(Bjorn& bjorn);

private:
	static const int FORCE_X = 500;
	static const int FORCE_Y = 10000;
};