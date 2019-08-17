#pragma once

#include "project/project.h"

#include "base/parser.h"

namespace ss
{

class ProjectMetadataParser : public base::Parser
{
public:
	ProjectMetadataParser();
	~ProjectMetadataParser();

	bool Parse(ProjectMetadata& project);

private:
	bool ParseStruct(ProjectMetadata& meta, ProjectStruct& s);
	bool ParseObjView(ProjectMetadata& meta, ProjectObjView& view);
};

}
