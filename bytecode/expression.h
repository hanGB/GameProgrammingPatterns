#pragma once
#include "stdafx.h"

class Expression {
public:
	virtual ~Expression() {}
	virtual double Evaluate() = 0;
};

class NumberExpression : public Expression {
public:
	NumberExpression(double value) : m_value(value) {}
	virtual double Evaluate() { return m_value; }

private:
	double m_value;
};

class AdditionExpression : public Expression {
public:
	AdditionExpression(Expression* left, Expression* right) : m_left(left), m_right(right) {}
	virtual ~AdditionExpression() { delete m_left; delete m_right; }
	virtual double Evaluate()
	{
		double left = m_left->Evaluate();
		double right = m_right->Evaluate();

		std::cout << left << "+" << right << std::endl;

		return left + right;
	}

private:
	Expression* m_left;
	Expression* m_right;
};

class SubtractionExpression : public Expression {
public:
	SubtractionExpression(Expression* left, Expression* right) : m_left(left), m_right(right) {}
	virtual ~SubtractionExpression() { delete m_left; delete m_right; }
	virtual double Evaluate()
	{
		double left = m_left->Evaluate();
		double right = m_right->Evaluate();

		std::cout << left << "-" << right << std::endl;

		return left - right;
	}

private:
	Expression* m_left;
	Expression* m_right;
};

class MultiplicationExpression : public Expression {
public:
	MultiplicationExpression(Expression* left, Expression* right) : m_left(left), m_right(right) {}
	virtual ~MultiplicationExpression() { delete m_left; delete m_right; }
	virtual double Evaluate()
	{
		double left = m_left->Evaluate();
		double right = m_right->Evaluate();

		std::cout << left << "*" << right << std::endl;

		return left * right;
	}

private:
	Expression* m_left;
	Expression* m_right;
};
