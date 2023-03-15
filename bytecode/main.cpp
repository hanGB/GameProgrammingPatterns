#include "stdafx.h"
#include "expression.h"

char* ReadDataFile(const char* fileName) 
{
	// 데이터 파일 열기
	std::fstream in;
	in.open(fileName);

	// 파일 길이 얻기
	in.seekg(0, std::ios::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[length + 1];

	// 길이 만큼 파일 읽기
	in.read(data, length);
	data[length] = '\0';

	in.close();

	return data;
}

bool ProcessOperator(std::stack<char>& operatorWaitingStack, std::stack<Expression*>& expressionWaitingStack)
{
	// 2개의 Expression이 없으면(left, right가 없으면) 오류
	if (expressionWaitingStack.size() < 2) {
		return false;
	}

	// 맨 위 연산자 하나 pop
	char oper = operatorWaitingStack.top();
	operatorWaitingStack.pop();

	if (oper == '+') {
		// 뒤에 항이 위에 쌓이기 때문에 right, left 순서로 저장
		Expression* right = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		Expression* left = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		// 표현식 생성 및 대기 스택에 추가
		AdditionExpression* add = new AdditionExpression(left, right);
		expressionWaitingStack.push(add);
	}
	else if (oper == '-') {
		Expression* right = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		Expression* left = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		SubtractionExpression* sub = new SubtractionExpression(left, right);
		expressionWaitingStack.push(sub);
	}
	else if (oper == '*') {
		Expression* right = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		Expression* left = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		MultiplicationExpression* multi = new MultiplicationExpression(left, right);
		expressionWaitingStack.push(multi);
	}

	return true;
}

int main()
{
	// 데이터 읽기
	char* data = ReadDataFile("data.txt");
	std::stack<Expression*> expressionWaitingStack;
	std::stack<char> operatorWaitingStack;
	bool parenthesis = false;
	bool error = false;

	int i = 0;
	while (data[i] != '\0') {
		switch (data[i]) {
		case ' ':
			break;
		case '(':
			std::cout << '(';

			// 이미 괄호가 열려 있는 경우 오류(괄호 안 괄호는 없음)
			if (parenthesis) {
				error = true;
				break;
			}
			parenthesis = true;
			// 연산자 대기 스택에 ( 추가
			operatorWaitingStack.push('(');
			break;
		case ')': 
		{
			std::cout << ')';

			if (!parenthesis) {
				error = true;
				break;
			}
			parenthesis = false;
			while (true) {
				if (!operatorWaitingStack.empty()) {
					// 괄호가 나올 때까지 대기 중인 연산자 pop
					if (operatorWaitingStack.top() == '(') {
						operatorWaitingStack.pop();
						break;
					}
					// 괄호가 아닌 대기 중인 연산자 처리(괄호 안 괄호는 없음)
					if (!operatorWaitingStack.empty()) {
						error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
					}
				}
				else {
					error = true;
				}
			}
			break;
			}
		case '+':
			std::cout << '+';

			if (!operatorWaitingStack.empty()) {
				// 대기 중인 연산자 중 최우선 순위기 괄호가 아닐 경우 처리
				if (operatorWaitingStack.top() != '(') {
					error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
				}
			}
			// + 연산자 대기 스택에 추가
			operatorWaitingStack.push('+');
			break;
		case '-':
			std::cout << '-';

			if (!operatorWaitingStack.empty()) {
				// 대기 중인 연산자 중 최우선 순위기 괄호가 아닐 경우 처리
				if (operatorWaitingStack.top() != '(') {
					error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
				}
			}
			// - 연산자 대기 스택에 추가
			operatorWaitingStack.push('-');
			break;
		case '*':
			std::cout << '*';

			// 다른 연산자 처리없이 * 연산자 대기 스택에 추가
			operatorWaitingStack.push('*');
			break;
		default: 
		{
			// 0 ~ 9 사이일 경우 숫자로 취급
			if (data[i] >= '0' && data[i] <= '9') {
				int n = i + 1;

				// 0 ~ 9 사이가 연속될 경우 같은 숫자로 취급
				while (data[n] >= '0' && data[n] <= '9') {
					n++;
				}
				int size = n - i;
				char* numberText = new char[size + 1];
				n = 0;
				// 문자를 따로 저장
				while (n != size) {
					numberText[n] = data[i];
					i++;
					n++;
				}
				// 반복에서 i를 1 증가 시킬 것이기 때문에 1 감소시킴
				i--;
				numberText[size] = '\0';
				// 문자열을 실제 숫자로 변경
				int number = atoi(numberText);
				delete[] numberText;

				std::cout << number;

				// 표현식 대기 스택에 추가
				NumberExpression* numberExpression = new NumberExpression(number);
				expressionWaitingStack.push(numberExpression);

			}
			break;
		}
		}
		
		if (error) {
			break;
		}
		i++;
	}
	std::cout << '\n';

	// 연산자 대기 스택이 비어 있지 않을 경우 처리
	while (true) {
		if (!operatorWaitingStack.empty()) {
			// ( 제거
			if (operatorWaitingStack.top() == '(') {
				operatorWaitingStack.pop();
			}
			if (!operatorWaitingStack.empty()) {
				error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
			}
		}
		else {
			break;
		}
	}

	if (error) {
		std::cout << "ERROR\n";
	}
	else {
		// 표현식 대기 스택의 맨 위 실행 
		std::cout << expressionWaitingStack.top()->Evaluate() << std::endl;
	}

	delete expressionWaitingStack.top();
	delete[] data;
}
