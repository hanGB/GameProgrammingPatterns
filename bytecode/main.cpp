#include "stdafx.h"
#include "expression.h"

char* ReadDataFile(const char* fileName) 
{
	// ������ ���� ����
	std::fstream in;
	in.open(fileName);

	// ���� ���� ���
	in.seekg(0, std::ios::end);
	size_t length = in.tellg();
	in.seekg(0, std::ios::beg);

	char* data = new char[length + 1];

	// ���� ��ŭ ���� �б�
	in.read(data, length);
	data[length] = '\0';

	in.close();

	return data;
}

bool ProcessOperator(std::stack<char>& operatorWaitingStack, std::stack<Expression*>& expressionWaitingStack)
{
	// 2���� Expression�� ������(left, right�� ������) ����
	if (expressionWaitingStack.size() < 2) {
		return false;
	}

	// �� �� ������ �ϳ� pop
	char oper = operatorWaitingStack.top();
	operatorWaitingStack.pop();

	if (oper == '+') {
		// �ڿ� ���� ���� ���̱� ������ right, left ������ ����
		Expression* right = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		Expression* left = expressionWaitingStack.top();
		expressionWaitingStack.pop();
		// ǥ���� ���� �� ��� ���ÿ� �߰�
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
	// ������ �б�
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

			// �̹� ��ȣ�� ���� �ִ� ��� ����(��ȣ �� ��ȣ�� ����)
			if (parenthesis) {
				error = true;
				break;
			}
			parenthesis = true;
			// ������ ��� ���ÿ� ( �߰�
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
					// ��ȣ�� ���� ������ ��� ���� ������ pop
					if (operatorWaitingStack.top() == '(') {
						operatorWaitingStack.pop();
						break;
					}
					// ��ȣ�� �ƴ� ��� ���� ������ ó��(��ȣ �� ��ȣ�� ����)
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
				// ��� ���� ������ �� �ֿ켱 ������ ��ȣ�� �ƴ� ��� ó��
				if (operatorWaitingStack.top() != '(') {
					error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
				}
			}
			// + ������ ��� ���ÿ� �߰�
			operatorWaitingStack.push('+');
			break;
		case '-':
			std::cout << '-';

			if (!operatorWaitingStack.empty()) {
				// ��� ���� ������ �� �ֿ켱 ������ ��ȣ�� �ƴ� ��� ó��
				if (operatorWaitingStack.top() != '(') {
					error = !ProcessOperator(operatorWaitingStack, expressionWaitingStack);
				}
			}
			// - ������ ��� ���ÿ� �߰�
			operatorWaitingStack.push('-');
			break;
		case '*':
			std::cout << '*';

			// �ٸ� ������ ó������ * ������ ��� ���ÿ� �߰�
			operatorWaitingStack.push('*');
			break;
		default: 
		{
			// 0 ~ 9 ������ ��� ���ڷ� ���
			if (data[i] >= '0' && data[i] <= '9') {
				int n = i + 1;

				// 0 ~ 9 ���̰� ���ӵ� ��� ���� ���ڷ� ���
				while (data[n] >= '0' && data[n] <= '9') {
					n++;
				}
				int size = n - i;
				char* numberText = new char[size + 1];
				n = 0;
				// ���ڸ� ���� ����
				while (n != size) {
					numberText[n] = data[i];
					i++;
					n++;
				}
				// �ݺ����� i�� 1 ���� ��ų ���̱� ������ 1 ���ҽ�Ŵ
				i--;
				numberText[size] = '\0';
				// ���ڿ��� ���� ���ڷ� ����
				int number = atoi(numberText);
				delete[] numberText;

				std::cout << number;

				// ǥ���� ��� ���ÿ� �߰�
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

	// ������ ��� ������ ��� ���� ���� ��� ó��
	while (true) {
		if (!operatorWaitingStack.empty()) {
			// ( ����
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
		// ǥ���� ��� ������ �� �� ���� 
		std::cout << expressionWaitingStack.top()->Evaluate() << std::endl;
	}

	delete expressionWaitingStack.top();
	delete[] data;
}
