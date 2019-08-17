#include "base/parser.h"

#include <string.h>

namespace base {

bool Parser::Cmp(const char* t1, const char* t2)
{
	return strcmp(t1, t2) == 0;
}

bool Parser::Cmp(const char* t1, const std::string& t2)
{
	return strcmp(t1, t2.c_str()) == 0;
}

void Parser::Open(const char* text, size_t size)
{
	m_tokenizer.SetDefaultCharsets();
	m_tokenizer.SetCharacterRule('=', base::PARSING_RULE_SYMBOL);
	m_tokenizer.SetCharacterRule('{', base::PARSING_RULE_SYMBOL);
	m_tokenizer.SetCharacterRule('}', base::PARSING_RULE_SYMBOL);

	m_tokenizer.Open(text, size);
}

void Parser::Close()
{
	m_tokenizer.Close();
}

bool Parser::SkipEndLines()
{
	do
	{
		if (!m_tokenizer.ParseNext())
			return false;
	} while (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '\n');

	return true;
}

bool Parser::ParseWord(String_t& value)
{
	if (!m_tokenizer.ParseNext() || !m_tokenizer.IsWord())
	{
		m_errorMessage << "Word identifier expected ";
		return false;
	}

	value = m_tokenizer.GetWord();
	return true;
}

bool Parser::ParseEquals()
{
	if (!m_tokenizer.ParseNext() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '=')
	{
		m_errorMessage << "'=' expected ";
		return false;
	}

	return true;
}

bool Parser::ParseEqualsWord(String_t& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [Word identifier] ";
		return false;
	}

	return ParseWord(value);
}

bool Parser::ParseBool(bool& value)
{
	String_t strValue;
	if (!ParseWord(strValue))
	{
		m_errorMessage << " for bool value";
		return false;
	}

	if (strValue.compare("true") == 0)
		value = true;
	else if (strValue.compare("false") == 0)
		value = false;
	else {
		m_errorMessage << " true or false expected for bool value";
		return false;
	}

	return true;
}

bool Parser::ParseInt(int& value)
{
	String_t strValue;
	if (!ParseNum(strValue))
	{
		m_errorMessage << " for int value";
		return false;
	}

	try {
		value = std::stoi(strValue);
	}
	catch (std::exception& e) {
		m_errorMessage << " can't parse int value " << e.what();
		return false;
	}

	return true;
}

bool Parser::ParseFloat(float& value)
{
	String_t strValue;
	if (!ParseNum(strValue))
	{
		m_errorMessage << " for float value";
		return false;
	}

	try {
		value = std::stof(strValue);
	}
	catch (std::exception& e) {
		m_errorMessage << " can't parse float value " << e.what();
		return false;
	}

	return true;
}

bool Parser::TryParseBool(bool& value)
{
	if (!m_tokenizer.IsWord())
		return false;

	String_t strValue = m_tokenizer.GetWord();

	if (strValue.compare("true") == 0)
		value = true;
	else if (strValue.compare("false") == 0)
		value = false;
	else
		return false;

	return true;
}

bool Parser::TryParseDouble(double& value)
{
	if (!m_tokenizer.IsNumber())
		return false;

	String_t strValue = m_tokenizer.GetNumber();

	try {
		value = std::stod(strValue);
	}
	catch (...) {
		return false;
	}

	return true;
}

bool Parser::ParseEqualsBool(bool& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [bool identifier] ";
		return false;
	}

	return ParseBool(value);
}

bool Parser::ParseEqualsInt(int& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [int identifier] ";
		return false;
	}

	return ParseInt(value);
}

bool Parser::ParseEqualsFloat(float& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [float identifier] ";
		return false;
	}

	return ParseFloat(value);
}

bool Parser::ParseString(String_t& value)
{
	if (!m_tokenizer.ParseNext() || !m_tokenizer.IsString())
	{
		m_errorMessage << "Quoted string expected ";
		return false;
	}

	value = m_tokenizer.GetString();
	return true;
}

bool Parser::ParseEqualsString(String_t& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [Quoted string] ";
		return false;
	}

	return ParseString(value);
}

bool Parser::ParseNum(String_t& value)
{
	if (!m_tokenizer.ParseNext() || !m_tokenizer.IsNumber())
	{
		m_errorMessage << "Number expected ";
		return false;
	}

	value = m_tokenizer.GetNumber();
	return true;
}

bool Parser::ParseEqualsNum(String_t& value)
{
	if (!ParseEquals())
	{
		m_errorMessage << "for [Number] ";
		return false;
	}

	return ParseNum(value);
}

Parser::String_t Parser::ErrorMessage() const
{
	return m_errorMessage.str();
}

}