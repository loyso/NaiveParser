
#include "base/base.h"
#include "base/memoryblock.h"
#include "project/project_parser.h"

#include <iostream>

int main(int argc, const char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::ostream& out = std::cout;

	if (argc < 2)
	{
		out << "Usage: <project.ssproj>\n";
		return 1;
	}

	try
	{
		std::string fileName = argv[1];
		
		ss::Project project;
		project.m_projectPath = ss::FindParentPath(fileName);
		{
			base::MemoryBlock content;
			if (!base::ReadWholeFile(fileName, content))
			{
				out << "Can't read file " << fileName << std::endl;
				return 2;
			}

			ss::ProjectParser parser;
			parser.Open(content.pBlock, content.size);
			if (!parser.Parse(project))
			{
				out << "Can't parse file " << fileName << std::endl;
				out << "Parse error: " << parser.ErrorMessage() << std::endl;
				return 3;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	out << "Parsing completed. All good!" << std::endl;
	return 0;
}

