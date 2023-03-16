#pragma once
#include "stdafx.h"
#include "expression.h"
#include "read_file.h"

// 인터프리터 패턴 실행기
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
		// 데이터 읽기
		m_data = ReadFile(fileName);

		int i = 0;
		while (m_data[i] != '\0') {
			switch (m_data[i]) {
			case ' ':
				break;
			case '(':
			{
				std::cout << '(';
				// 이미 괄호가 열려 있는 경우 오류(괄호 안 괄호는 없음)
				if (m_isParenthesis) return false;
				m_isParenthesis = true;

				// 연산자 대기 스택에 ( 추가
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
						// 괄호가 나올 때까지 대기 중인 연산자 pop
						if (m_operatorWaitingStack.top() == '(') {
							m_operatorWaitingStack.pop();
							break;
						}
						// 괄호가 아닌 대기 중인 연산자 처리(괄호 안 괄호는 없음)
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
				// 다른 연산자 처리없이 * 연산자 대기 스택에 추가
				m_operatorWaitingStack.push('*');
				break;
			}
			default:
			{
				// 0 ~ 9 사이일 경우 숫자로 취급
				if (m_data[i] >= '0' && m_data[i] <= '9') {
					int number = ExtractNumberInData(m_data, i);
					std::cout << number;

					// 표현식 대기 스택에 추가
					NumberExpression* numberExpression = new NumberExpression(number);
					m_expressionWaitingStack.push(numberExpression);

				}
				break;
			}
			}
			i++;
		}
		std::cout << '\n';

		// 연산자 대기 스택이 비어 있지 않을 경우 처리
		while (true) {
			if (!m_operatorWaitingStack.empty()) {
				// ( 제거
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
		// 2개의 Expression이 없으면(left, right가 없으면) 오류
		if (m_expressionWaitingStack.size() < 2) {
			return false;
		}

		// 맨 위 연산자 하나 pop
		char oper = m_operatorWaitingStack.top();
		m_operatorWaitingStack.pop();

		if (oper == '+') {
			// 뒤에 항이 위에 쌓이기 때문에 right, left 순서로 저장
			Expression* right = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			Expression* left = m_expressionWaitingStack.top();
			m_expressionWaitingStack.pop();
			// 표현식 생성 및 대기 스택에 추가
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
			// 대기 중인 연산자 중 최우선 순위기 괄호가 아닐 경우 처리
			if (m_operatorWaitingStack.top() != '(') {
				if (!ProcessOperator()) return false;
			}
		}
		// - 연산자 대기 스택에 추가
		m_operatorWaitingStack.push(oper);
	}
	int ExtractNumberInData(char* m_data, int& i)
	{
		int n = i + 1;

		// 0 ~ 9 사이가 연속될 경우 같은 숫자로 취급
		while (m_data[n] >= '0' && m_data[n] <= '9') {
			n++;
		}
		int size = n - i;
		char* numberText = new char[size + 1];
		n = 0;
		// 문자를 따로 저장
		while (n != size) {
			numberText[n] = m_data[i];
			i++;
			n++;
		}
		// 반복에서 i를 1 증가 시킬 것이기 때문에 1 감소시킴
		i--;
		numberText[size] = '\0';
		// 문자열을 실제 숫자로 변경
		int number = atoi(numberText);
		delete[] numberText;

		return number;
	}

	char* m_data = nullptr;
	std::stack<Expression*> m_expressionWaitingStack;
	std::stack<char> m_operatorWaitingStack;
	bool m_isParenthesis = false;	
};