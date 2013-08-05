/*
 * ClassExample.h
 *
 *  Created on: Jul 31, 2013
 *      Author: dubath
 */

#ifndef CLASSEXAMPLE_H_
#define CLASSEXAMPLE_H_

#include<string>

class ClassExample {
public:
	ClassExample();
	virtual ~ClassExample();

	double computeSum(double first, double second);
	double divideNumbers(double first, double second);

	void doSomething(double first, double second);

	/**
	 * Getters
	 */
	double getRa() const {
		return m_ra;
	}

	double getResult() const {
		return m_result;
	}

	int64_t getSourceId() const {
		return m_sourceId;
	}

	static const std::string& getStaticString() {
		return s_staticString;
	}

private:

	static std::string s_staticString;

	/**
	 * Example of a long private member
	 *
	 *
	 */
	int64_t m_sourceId { 0 };

	/// Source right ascension
	double m_ra { 0 };

	double m_result{0};

};

#endif /* CLASSEXAMPLE_H_ */
