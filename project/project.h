#pragma once

#include "base/base.h"
#include "base/vec.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace ss {

using Name_t = std::string;
using MemberName_t = std::string;
using ViewName_t = std::string;
using FileName_t = std::string;

enum class ValueType : int {
	Bool, Int, Float, Vec3
};

const std::string& ToString(ValueType type);

class ProjectObjView {
public:
	ViewName_t m_name;

	using Members_t = std::unordered_map<MemberName_t, ValueType>;
	Members_t m_members;
};

class ProjectObjInstance {
public:
	Name_t m_enumName;
	FileName_t m_pathToObject;
};

class ProjectStruct {
public:
	Name_t m_name;
	ViewName_t m_viewName;
	std::vector<std::unique_ptr<ProjectObjInstance>> m_instances;
};

class ProjectMetadata {
public:
	using Views_t = std::vector<std::unique_ptr<ProjectObjView>>;
	Views_t m_views;

	using Structs_t = std::vector<std::unique_ptr<ProjectStruct>>;
	Structs_t m_structs;
};

class ProjectSubObject {
public:
	Name_t m_subObjectType;

	enum class ValueInstanceType : int {
		Bool, Double, String
	};

	using ValueInstance_t = std::variant<bool, double, std::string>; // indices must match ValueInstanceType

	using Values_t = std::unordered_map<MemberName_t, ValueInstance_t>;
	Values_t m_values;
};


class ProjectObject {
public:
	Name_t m_name;
	ViewName_t m_viewName;

	using SubObjectValue_t = std::unique_ptr<ProjectSubObject>;

	using Value_t = std::variant<bool, int, float, base::Vec3, SubObjectValue_t>; // indices must match ValueType

	using Values_t = std::unordered_map<MemberName_t, Value_t>;
	Values_t m_values;
};

class ProjectData {
public:
	using Objects_t = std::vector<std::unique_ptr<ProjectObject>>;
	Objects_t m_objects;
};

class ProjectMetadataFile {
public:
	FileName_t m_name;
	ProjectMetadata m_meta;
};

class ProjectDataFile {
public:
	FileName_t m_name;
	ProjectData m_data;
};

class Project {
public:
	Name_t m_name;
	Name_t m_projectPath;

	std::vector<std::unique_ptr<ProjectMetadataFile>> m_metadataFiles;
	std::vector<std::unique_ptr<ProjectDataFile>> m_dataFiles;
};


std::string FindParentPath(const std::string& path);
std::string MakePath(const std::string& path, const std::string& filename);

}
