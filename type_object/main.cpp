#include "stdafx.h"
#include "breed.h"
#include "monster.h"
#include <string>
// json �ļ�
#include "json/json.h"

void print_json(const Json::Value& value, bool emitUTF8) {
	Json::StreamWriterBuilder builder;
	builder.settings_["emitUTF8"] = emitUTF8;
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(value, &std::cout);
}

int main()
{
	//
	// �ѱ� ��� ���
	// 
	// jsoncpp.cpp ���Ͽ��� 
	// static String valueToQuotedStringN(const char* value, size_t length,bool emitUTF8 = false)�� ���� ������ �Ʒ��� ���� �����ؾ� �ѱ� ����� �����ϴ�
	// �̶� ������ ���ڵ� ������ �⺻(ANSI)�̴�
	// 
	// ���� �� switch(*c)�� default����
	// if (emitUTF8) -> if (true)���� �����Ͽ� ������ emitUTF8�� true�� ���� ó���ϵ��� �Ѵ�
	//

	// JSON ���� �� �����͸� ��� ����
	Json::Value breedJsonValue;
	// JSON ���� ����
	Json::Reader reader;

	// JSON ���� �б�
	std::ifstream jsonIn("breed_data.json");
	// ������ �̿��Ͽ� Value�� �����͸� �Ľ��� ���� 
	bool isReadJson = reader.parse(jsonIn, breedJsonValue, false);

	jsonIn.close();

	Breed* dragonBreed = new Breed(nullptr, 230, "���� ���� �ս��ϴ�!");
	Breed* greatDragonBreed = new Breed(dragonBreed, 460, nullptr);

	Breed* trollBreed = new Breed(nullptr, breedJsonValue["Ʈ��"]["ü��"].asInt(), breedJsonValue["Ʈ��"]["���ݹ���"].asCString());
	Breed* trollArcherBreed = new Breed(trollBreed, 0, breedJsonValue["Ʈ�� �ü�"]["���ݹ���"].asCString());
	Breed* trollWizardBreed = new Breed(trollBreed, 0, breedJsonValue["Ʈ�� ������"]["���ݹ���"].asCString());

	Monster* monsters[] = {
		dragonBreed->NewMonster(),
		greatDragonBreed->NewMonster(),
		trollBreed->NewMonster(),
		trollArcherBreed->NewMonster(),
		trollWizardBreed->NewMonster()
	};


	for (auto& monster : monsters) {
		std::cout << "ü���� " << monster->GetHealth() << "�� ";
		std::cout << monster->GetAttack() << std::endl;
	}


	for (auto& monster : monsters) {
		delete monster;
	}
	delete dragonBreed;
	delete trollBreed;
}