#pragma once

#include "base/base.h"
#include "base/tokenizer.h"

#include <string>
#include <sstream>
#include <vector>

namespace base {

class Parser {
public:
	using String_t = std::string;
	using StringStream_t = std::stringstream;

	void Open(const char* text, size_t size);
	void Close();

	String_t ErrorMessage() const;

protected:
	bool Cmp(const char* t1, const char* t2);
	bool Cmp(const char* t1, const std::string& t2);

	bool ParseWord(String_t& value);
	bool ParseString(String_t& value);
	bool ParseNum(String_t& value);

	bool ParseEquals();
	bool ParseBool(bool& value);
	bool ParseInt(int& value);
	bool ParseFloat(float& value);

	bool TryParseBool(bool& value);
	bool TryParseDouble(double& value);

	bool ParseEqualsBool(bool& value);
	bool ParseEqualsInt(int& value);
	bool ParseEqualsFloat(float& value);

	bool ParseEqualsWord(String_t& value);
	bool ParseEqualsString(String_t& value);
	bool ParseEqualsNum(String_t& value);

	bool SkipEndLines();

	base::TextTokenizer m_tokenizer;

	StringStream_t m_errorMessage;
};

}