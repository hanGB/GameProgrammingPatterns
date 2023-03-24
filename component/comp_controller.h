#pragma once

struct CompInputChunk {
	bool Input_W = false;
	bool Input_A = false;
	bool Input_S = false;
	bool Input_D = false;
	bool Input_Space = false;
};

class CompController {
public:
	// �̱���
	static CompController* GetInstance();

	CompInputChunk GetInputs() const;
	void SetInputs(CompInputChunk& inputs);

	int GetXAxisDirection();
	int GetYAxisDirection();

private:
	CompController();
	~CompController();

	CompInputChunk m_inputs;
};