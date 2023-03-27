#pragma once

class Bjorn;
class CompWorld;

class PhysicsComponent {
public:
	void Update(Bjorn& bjorn, CompWorld& world, double elapsedTimeInSec);

	CompVector2<double> GetVolume() const;
	void SetVolume(CompVector2<double> volume);

private:
	CompVector2<double> m_volume;
};