#pragma once

#include "project/project.h"

#include "base/parser.h"

namespace ss
{

class ProjectParser : public base::Parser
{
public:
	ProjectParser();
	~ProjectParser();
	
	bool Parse(Project& project);

private:
	bool ParseMetadataFiles(Project& project);
	bool ParseDataFiles(Project& project);

	bool ReadMetadata(const String_t& metadataFileName, ProjectMetadata& metadata);
	bool ReadData(const String_t& dataFileName, ProjectData& data);
};

}
