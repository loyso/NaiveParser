#pragma once

#include "project/project.h"

#include "base/parser.h"

namespace ss
{

	class ProjectDataParser : public base::Parser
	{
	public:
		ProjectDataParser();
		~ProjectDataParser();

		bool Parse(ProjectData& project);

	private:
		bool ParseObject(ProjectData& project, ProjectObject& object);
		bool ParseObjectData(ProjectData& project, ProjectObject& object);
		bool ParseSubObject(ProjectData& project, ProjectSubObject& subObject);
	};

}
