#pragma once

#include <string>
#include <vector>
#include <map>

#include "Model.h"

typedef struct {
	glm::vec3 v1, v2, v3, normal;
} plane_t;

typedef struct {
	glm::vec3 points[3];
	std::string texture;
	float xoffset;
	float yoffset;
	float rotation;
	float xscale;
	float yscale;

	// After calculations
	std::vector<glm::vec3> vertices_calculated;
	glm::vec3 face_tex_vec[2];
	glm::vec2 face_tex_shift;
} mapface_t;

typedef struct {
	std::vector<mapface_t> faces;
	std::vector<plane_t> planes;
} mapbrush_t;

typedef struct {
	std::vector<mapbrush_t> brushes;
	std::map<std::string, std::string> properties;
} mapentity_t;

class Mapfile
{
public:
	Mapfile(std::string const& filepath);
	~Mapfile();

	void BuildRenderModel();

	void Load(std::string const& filepath);

protected:

	void ParseMapFromBuffer();

	std::pair<std::string, std::string> EntityParseField(const std::string& line);

	mapface_t ParseFace(const std::string& line);

	mapbrush_t ParseBrush(const std::string& brush_buffer);

	mapentity_t ParseEntity(const std::string& entity_buffer);

	void TrimBrushPlanes(mapbrush_t& brush);

	void Deduplicate(mapbrush_t& brush);

	std::vector<mapentity_t> map_entities;

	std::string filename;
	const char* data; // Raw file data

	// Stats
	size_t n_entities; // Total map entities
	size_t n_brushes; // Total map brushes
	size_t n_faces; // Total map faces
};