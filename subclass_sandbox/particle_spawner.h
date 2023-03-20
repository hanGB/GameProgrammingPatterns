#pragma once
#include "stdafx.h"

enum class ParticleType {
	PARITCLE_DUST,
	PARTICLE_SPARKLES
};

class ParticleSpawner {
public:
	ParticleSpawner() { }
	~ParticleSpawner() { }

	void SpawnParticle(ParticleType type, int count) {
		std::string particle = m_particleInformationMap.find(type)->second;
		std::cout << "Spawn particle: " << particle << " - count " << count << '\n';
		m_particleList.push_back(std::pair<ParticleType, std::string>(type, particle));
	}

private:
	std::list<std::pair<ParticleType, std::string>> m_particleList;
	std::map<ParticleType, std::string> m_particleInformationMap = {
		std::pair<ParticleType, std::string>(ParticleType::PARITCLE_DUST, "particle dust"),
		std::pair<ParticleType, std::string>(ParticleType::PARTICLE_SPARKLES, "particle sparkles"),
	};
};
