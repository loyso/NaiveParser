
#include "project/project_parser.h"
#include "project/project_data_parser.h"
#include "project/project_metadata_parser.h"

#include "base/memoryblock.h"

namespace ss
{

ProjectParser::ProjectParser()
{
}

ProjectParser::~ProjectParser()
{
}

bool ProjectParser::Parse(Project& project)
{
	if( !SkipEndLines() )
		return false;

	bool process = true;
	do
	{
		if( m_tokenizer.IsWord() )
		{
			const char* key = m_tokenizer.GetWord();
			if( Cmp( "name", key ) )
			{
				if( !ParseEqualsString( project.m_name ) )
				{
					m_errorMessage << "for project.name";
					return false;
				}
			}
			else if (Cmp("meta", key))
			{
				if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
				{
					m_errorMessage << "'{' expected after 'meta'";
					return false;
				}

				if (!ParseMetadataFiles(project))
					return false;
			}
			else if( Cmp( "data", key ) )
			{
				if (!SkipEndLines() || !m_tokenizer.IsSymbol() || m_tokenizer.GetSymbol() != '{')
				{
					m_errorMessage << "'{' expected after 'data'";
					return false;
				}

				if (!ParseDataFiles(project))
					return false;
			}
		}

		if( !m_tokenizer.ParseNext() )
			process = false;
	} while( process );

	return true;
}

bool ProjectParser::ParseMetadataFiles(Project& project) {
	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after 'meta {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* valueType = m_tokenizer.GetWord();
			if (Cmp("file", valueType))
			{
				auto file = std::make_unique<ProjectMetadataFile>();
				if (!ParseEqualsString(file->m_name))
				{
					m_errorMessage << "for meta file";
					return false;
				}

				auto path = MakePath(project.m_projectPath, file->m_name);
				if (!ReadMetadata(path, file->m_meta))
					return false;

				project.m_metadataFiles.push_back(std::move(file));
			}
			else {
				m_errorMessage << "file entries expected";
				return false;
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected after meta";
			process = false;
		}
	} while (process);

	return false;
}

bool ProjectParser::ParseDataFiles(Project& project) {
	if (!SkipEndLines())
	{
		m_errorMessage << "Parameters expected after 'data {'";
		return false;
	}

	bool process = true;
	do
	{
		if (m_tokenizer.IsWord())
		{
			const char* valueType = m_tokenizer.GetWord();
			if (Cmp("file", valueType))
			{
				auto file = std::make_unique<ProjectDataFile>();
				if (!ParseEqualsString(file->m_name))
				{
					m_errorMessage << "for data file";
					return false;
				}

				auto path = MakePath(project.m_projectPath, file->m_name);
				if (!ReadData(path, file->m_data))
					return false;

				project.m_dataFiles.push_back(std::move(file));
			}
			else {
				m_errorMessage << "file entries expected";
				return false;
			}
		}
		else if (m_tokenizer.IsSymbol() && m_tokenizer.GetSymbol() == '}')
			return true;

		if (!m_tokenizer.ParseNext())
		{
			m_errorMessage << "'}' expected after data";
			process = false;
		}
	} while (process);

	return false;
}

bool ProjectParser::ReadMetadata(const String_t& fileName, ProjectMetadata& metadata)
{
	base::MemoryBlock content;
	if (!base::ReadWholeFile(fileName, content))
	{
		m_errorMessage << "Can't read file " << fileName << std::endl;
		return false;
	}

	ss::ProjectMetadataParser metadataParser;
	metadataParser.Open(content.pBlock, content.size);
	if (!metadataParser.Parse(metadata))
	{
		m_errorMessage << "Can't parse metadata file " << fileName << std::endl;
		m_errorMessage << "Parse error: " << metadataParser.ErrorMessage() << std::endl;
		return false;
	}

	return true;
}

bool ProjectParser::ReadData(const String_t& fileName, ProjectData& data)
{
	base::MemoryBlock content;
	if (!base::ReadWholeFile(fileName, content))
	{
		m_errorMessage << "Can't read file " << fileName << std::endl;
		return false;
	}

	ss::ProjectDataParser dataParser;
	dataParser.Open(content.pBlock, content.size);
	if (!dataParser.Parse(data))
	{
		m_errorMessage << "Can't parse data file " << fileName << std::endl;
		m_errorMessage << "Parse error: " << dataParser.ErrorMessage() << std::endl;
		return false;
	}

	return true;
}

}
