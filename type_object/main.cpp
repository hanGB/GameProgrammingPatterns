#include "stdafx.h"
#include "breed.h"
#include "monster.h"
#include <string>
// json 파서
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
	// 한글 출력 방법
	// 
	// jsoncpp.cpp 파일에서 
	// static String valueToQuotedStringN(const char* value, size_t length,bool emitUTF8 = false)의 정의 내용을 아래와 같이 변경해야 한글 출력이 가능하다
	// 이때 파일을 인코딩 형식은 기본(ANSI)이다
	// 
	// 정의 내 switch(*c)의 default에서
	// if (emitUTF8) -> if (true)으로 변경하여 무조건 emitUTF8가 true인 경우로 처리하도록 한다
	//

	// JSON 파일 내 데이터를 담는 변수
	Json::Value breedJsonValue;
	// JSON 파일 리더
	Json::Reader reader;

	// JSON 파일 읽기
	std::ifstream jsonIn("breed_data.json");
	// 리더를 이용하여 Value에 데이터를 파싱해 저장 
	bool isReadJson = reader.parse(jsonIn, breedJsonValue, false);

	jsonIn.close();

	Breed* dragonBreed = new Breed(nullptr, 230, "용이 불을 뿜습니다!");
	Breed* greatDragonBreed = new Breed(dragonBreed, 460, nullptr);

	Breed* trollBreed = new Breed(nullptr, breedJsonValue["트롤"]["체력"].asInt(), breedJsonValue["트롤"]["공격문구"].asCString());
	Breed* trollArcherBreed = new Breed(trollBreed, 0, breedJsonValue["트롤 궁수"]["공격문구"].asCString());
	Breed* trollWizardBreed = new Breed(trollBreed, 0, breedJsonValue["트롤 마법사"]["공격문구"].asCString());

	Monster* monsters[] = {
		dragonBreed->NewMonster(),
		greatDragonBreed->NewMonster(),
		trollBreed->NewMonster(),
		trollArcherBreed->NewMonster(),
		trollWizardBreed->NewMonster()
	};


	for (auto& monster : monsters) {
		std::cout << "체력이 " << monster->GetHealth() << "인 ";
		std::cout << monster->GetAttack() << std::endl;
	}


	for (auto& monster : monsters) {
		delete monster;
	}
	delete dragonBreed;
	delete trollBreed;
}