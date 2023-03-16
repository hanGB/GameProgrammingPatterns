#pragma once
#include "stdafx.h"
#include "expression.h"
#include "read_file.h"

// ���������� ���� �����
class Interpreter {
public:
	Interpreter() {}
	~Interpreter() 
	{
		if (!m_expressionWaitingStack.empty()) {
			delete m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
		}
		if (m_data) delete[] m_data;
	}

	bool ExcuteFile(const char* fileName) {
		// ������ �б�
		m_data = ReadFile(fileName);

		int i = 0;
		while (m_data[i] != '\0') {
			switch (m_data[i]) {
			case ' ':
				break;
			case '(':
			{
				std::cout << '(';
				// �̹� ��ȣ�� ���� �ִ� ��� ����(��ȣ �� ��ȣ�� ����)
				if (m_isParenthesis) return false;
				m_isParenthesis = true;

				// ������ ��� ���ÿ� ( �߰�
				m_operatorWaitingStack.push('(');
				break;
			}
			case ')':
			{
				std::cout << ')';
				if (!m_isParenthesis) return false;
				m_isParenthesis = false;

				while (true) {
					if (!m_operatorWaitingStack.empty()) {
						// ��ȣ�� ���� ������ ��� ���� ������ pop
						if (m_operatorWaitingStack.top() == '(') {
							m_operatorWaitingStack.pop();
							break;
						}
						// ��ȣ�� �ƴ� ��� ���� ������ ó��(��ȣ �� ��ȣ�� ����)
						if (!m_operatorWaitingStack.empty()) {
							if (!ProcessOperator()) return false;
						}
					}
					else {
						return false;
					}
				}
				break;
			}
			case '+':
			{
				std::cout << '+';
				if (!ProcessAddSub('+')) return false;
				break;
			}
			case '-':
			{
				std::cout << '-';
				if (!ProcessAddSub('-')) return false;
				break;
			}
			case '*':
			{
				std::cout << '*';
				// �ٸ� ������ ó������ * ������ ��� ���ÿ� �߰�
				m_operatorWaitingStack.push('*');
				break;
			}
			default:
			{
				// 0 ~ 9 ������ ��� ���ڷ� ���
				if (m_data[i] >= '0' && m_data[i] <= '9') {
					int number = ExtractNumberInData(m_data, i);
					std::cout << number;

					// ǥ���� ��� ���ÿ� �߰�
					NumberExpression* numberExpression = new NumberExpression(number);
					m_expressionWaitingStack.push(numberExpression);

				}
				break;
			}
			}
			i++;
		}
		std::cout << '\n';

		// ������ ��� ������ ��� ���� ���� ��� ó��
		while (true) {
			if (!m_operatorWaitingStack.empty()) {
				// ( ����
				if (m_operatorWaitingStack.top() == '(') m_operatorWaitingStack.pop();
				if (!m_operatorWaitingStack.empty()) {
					if (!ProcessOperator()) return false;
				}
			}
			else {
				break;
			}
		}

		std::cout << m_expressionWaitingStack.top()->Evaluate() << std::endl;
		return true;
	}

private:
	bool ProcessOperator()
	{
		// 2���� Expression�� ������(left, right�� ������) ����
		if (m_expressionWaitingStack.size() < 2) {
			return false;
		}

		// �� �� ������ �ϳ� pop
		char oper = m_operatorWaitingStack.top();
		m_operatorWaitingStack.pop();

		if (oper == '+') {
			// �ڿ� ���� ���� ���̱� ������ right, left ������ ����
			Expression* right = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			Expression* left = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			// ǥ���� ���� �� ��� ���ÿ� �߰�
			AdditionExpression* add = new AdditionExpression(left, right);
			m_expressionWaitingStack.push(add);
		}
		else if (oper == '-') {
			Expression* right = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			Expression* left = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			SubtractionExpression* sub = new SubtractionExpression(left, right);
			m_expressionWaitingStack.push(sub);
		}
		else if (oper == '*') {
			Expression* right = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			Expression* left = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			MultiplicationExpression* multi = new MultiplicationExpression(left, right);
			m_expressionWaitingStack.push(multi);
		}

		return true;
	}
	bool ProcessAddSub(char oper)
	{
		if (!m_operatorWaitingStack.empty()) {
			// ��� ���� ������ �� �ֿ켱 ������ ��ȣ�� �ƴ� ��� ó��
			if (m_operatorWaitingStack.top() != '(') {
				if (!ProcessOperator()) return false;
			}
		}
		// - ������ ��� ���ÿ� �߰�
		m_operatorWaitingStack.push(oper);
	}
	int ExtractNumberInData(char* m_data, int& i)
	{
		int n = i + 1;

		// 0 ~ 9 ���̰� ���ӵ� ��� ���� ���ڷ� ���
		while (m_data[n] >= '0' && m_data[n] <= '9') {
			n++;
		}
		int size = n - i;
		char* numberText = new char[size + 1];
		n = 0;
		// ���ڸ� ���� ����
		while (n != size) {
			numberText[n] = m_data[i];
			i++;
			n++;
		}
		// �ݺ����� i�� 1 ���� ��ų ���̱� ������ 1 ���ҽ�Ŵ
		i--;
		numberText[size] = '\0';
		// ���ڿ��� ���� ���ڷ� ����
		int number = atoi(numberText);
		delete[] numberText;

		return number;
	}

	char* m_data = nullptr;
	std::stack<Expression*> m_expressionWaitingStack;
	std::stack<char> m_operatorWaitingStack;
	bool m_isParenthesis = false;	
};