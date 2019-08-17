
#include "project/project_metadata_parser.h"

namespace ss
{

ProjectMetadataParser::ProjectMetadataParser()
{
}

ProjectMetadataParser::~ProjectMetadataParser()
{
}

bool ProjectMetadataParser::Parse(ProjectMetadata& meta)
{
	if (!SkipEndLines())
		return false;

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* key = m_tokenizer.GetWord();
			if (Cmp("view", key))
			{
				auto view = std::make_unique<ProjectObjView>();
				if (!ParseWord(view->m_name))
				{
					m_errorMessage << "for 'view' name";
					return false;
				}

				if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
				{
					m_errorMessage << "'{' expected after 'view name'";
					return false;
				}

				if (!ParseObjView(meta, *view))
					return false;

				meta.m_views.push_back(std::move(view));
			}
			else if (Cmp("struct", key))
			{
				auto s = std::make_unique<ProjectStruct>();
				if (!ParseWord(s->m_name))
				{
					m_errorMessage << "for 'struct' name";
					return false;
				}

				if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
				{
					m_errorMessage << "'{' expected after 'struct name'";
					return false;
				}

				if (!ParseStruct(meta, *s))
					return false;

				meta.m_structs.push_back(std::move(s));
			}
			else {
				m_errorMessage << "'view' or 'sstruct' identifiers expected but got: " << key;
				return false;
			}
		}

		if (!m_tokenizer.ParseNext())
			process = false;
	} while (process);

	return true;
}

bool ProjectMetadataParser::ParseStruct(ProjectMetadata& meta, ProjectStruct& s)
{
	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after 'struct {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* key = m_tokenizer.GetWord();
			if (Cmp("view", key))
			{
				if (!ParseEqualsWord(s.m_viewName))
				{
					m_errorMessage << " for struct.view";
					return false;
				}
			}
			else if (Cmp("instance", key))
			{
				auto instance = std::make_unique<ProjectObjInstance>();
				if (!ParseWord(instance->m_enumName))
				{
					m_errorMessage << " for struct.instance.enumName";
					return false;
				}

				if (!ParseEqualsString(instance->m_pathToObject))
				{
					m_errorMessage << " for struct.instance.pathToObject";
					return false;
				}

				s.m_instances.push_back(std::move(instance));
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected for 'struct'";
			process = false;
		}
	} while (process);

	return false;
}

bool ProjectMetadataParser::ParseObjView(ProjectMetadata& meta, ProjectObjView& view)
{
	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after 'view' {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			Name_t memberName;
			std::string memberType = m_tokenizer.GetWord();

			if (!ParseWord(memberName))
			{
				m_errorMessage << "for 'view' type memberName";
				return false;
			}

			ValueType valueType;
			if (Cmp("bool", memberType))
			{
				valueType = ValueType::Bool;
			}
			else if (Cmp("int", memberType))
			{
				valueType = ValueType::Int;
			}
			else if (Cmp("float", memberType))
			{
				valueType = ValueType::Float;
			}
			else if (Cmp("Vec3", memberType))
			{
				valueType = ValueType::Vec3;
			}
			else {
				m_errorMessage << "member " << memberName << ": bool, int, float, Vec3 types expected";
				return false;
			}

			// optional
			if (m_tokenizer.ParseNext() && m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '=') {
				std::string mapsToPath;
				if (!ParseString(mapsToPath))
				{
					m_errorMessage << " for member " << memberName;
					return false;
				}
			}

			auto kv = view.m_members.find(memberName);
			if (kv == view.m_members.end())
			{
				view.m_members.insert(std::make_pair(memberName, valueType));
			}
			else
			{
				m_errorMessage << " 'view' already contains member " << kv->first;
				return false;
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected for 'view'";
			process = false;
		}
	} while (process);

	return false;
}

}
