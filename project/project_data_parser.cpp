
#include "project/project_data_parser.h"

namespace ss
{

ProjectDataParser::ProjectDataParser()
{
}

ProjectDataParser::~ProjectDataParser()
{
}

bool ProjectDataParser::Parse(ProjectData& project)
{
	if (!SkipEndLines())
		return false;

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* key = m_tokenizer.GetWord();
			if (Cmp("object", key))
			{
				auto object = std::make_unique<ProjectObject>();

				if (!ParseWord(object->m_name))
				{
					m_errorMessage << "for 'object' name";
					return false;
				}

				if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
				{
					m_errorMessage << "'{' expected after 'object'";
					return false;
				}

				if (!ParseObject(project, *object))
					return false;

				project.m_objects.push_back(std::move(object));
			}
		}

		if (!m_tokenizer.ParseNext())
			process = false;
	} while (process);

	return true;
}

bool ProjectDataParser::ParseObject(ProjectData& project, ProjectObject& object)
{
	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after 'object {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* key = m_tokenizer.GetWord();
			if (Cmp("data", key))
			{
				if (!ParseObjectData(project, object))
					return false;
			}
			else if (Cmp("view", key))
			{
				if (!ParseEqualsWord(object.m_viewName))
				{
					m_errorMessage << "for object.viewName";
					return false;
				}
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected for 'object'";
			process = false;
		}
	} while (process);

	return false;
}

bool ProjectDataParser::ParseObjectData(ProjectData& project, ProjectObject& object)
{
	if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
	{
		m_errorMessage << "'{' expected after object.data";
		return false;
	}

	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after object.data {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			Name_t valueName;
			std::string valueType = m_tokenizer.GetWord();

			if (!ParseWord(valueName))
			{
				m_errorMessage << "for object.data.value name";
				return false;
			}

			ProjectObject::Value_t projectValue;

			if (Cmp("bool", valueType))
			{
				bool value = false;
				if (!ParseEqualsBool(value))
				{
					m_errorMessage << "for object.data.bool name=value";
					return false;
				}
				projectValue = value;
			}
			else if (Cmp("int", valueType))
			{
				int value = 0;
				if (!ParseEqualsInt(value))
				{
					m_errorMessage << "for object.data.int name=value";
					return false;
				}
				projectValue = value;
			}
			else if (Cmp("float", valueType))
			{
				float value = 0;
				if (!ParseEqualsFloat(value))
				{
					m_errorMessage << "for object.data.float name=value";
					return false;
				}
				projectValue = value;
			}
			else if (Cmp("Vec3", valueType))
			{
				if (!ParseEquals())
				{
					m_errorMessage << "for object.data.Vec3 name=x y z";
					return false;
				}
				base::Vec3 value;
				if (!ParseFloat(value.x))
				{
					m_errorMessage << "for object.data.Vec3 name=x";
					return false;
				}
				if (!ParseFloat(value.y))
				{
					m_errorMessage << "for object.data.Vec3 name=x y";
					return false;
				}
				if (!ParseFloat(value.z))
				{
					m_errorMessage << "for object.data.Vec3 name=x y z";
					return false;
				}
				projectValue = value;
			}
			else {
				auto subObject = std::make_unique<ProjectSubObject>();
				subObject->m_subObjectType = valueType;
				if (!ParseSubObject(project, *subObject)) {
					m_errorMessage << " bool, int, float, Vec3 types expected or subobject name";
					return false;
				}
				projectValue = std::move(subObject);
			}

			auto kv = object.m_values.find(valueName);
			if (kv == object.m_values.end())
			{
				object.m_values.insert(std::make_pair(valueName, std::move(projectValue)));
			}
			else
			{
				m_errorMessage << " object already contains value " << kv->first;
				return false;
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected for object.data";
			process = false;
		}
	} while (process);

	return false;
}

bool ProjectDataParser::ParseSubObject(ProjectData& project, ProjectSubObject& subObject)
{
	if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
	{
		m_errorMessage << "'{' expected after subobject";
		return false;
	}

	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after subobject {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			Name_t valueName = m_tokenizer.GetWord();
			ProjectSubObject::ValueInstance_t valueInstance;
			if (!ParseEquals() || !m_tokenizer.ParseNext()) {
				m_errorMessage << " equals valueInstance expected";
				return false;
			}

			if (m_tokenizer.IsNumber())
			{
				double doubleValue = 0;
				if (TryParseDouble(doubleValue))
				{
					valueInstance = doubleValue;
				}
				else
				{
					m_errorMessage << " can't parse number value " << valueName;
					return false;
				}
			}
			if (m_tokenizer.IsWord())
			{
				bool boolValue = 0;
				std::string strValue;
				if (TryParseBool(boolValue))
					valueInstance = boolValue;
				else
				{
					strValue = m_tokenizer.GetWord();
					valueInstance = strValue;
				}
			}
			else if (m_tokenizer.IsString())
			{
				std::string strValue = m_tokenizer.GetString();
				valueInstance = strValue;
			}

			subObject.m_values.insert(std::make_pair(valueName, valueInstance));
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected for object.data";
			process = false;
		}
	} while (process);

	return false;
}

}
