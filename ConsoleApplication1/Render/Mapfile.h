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
	char texture[64];
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
	unsigned char nfaces;
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

	size_t EntityBrushCount(char* entity_buffer);

	size_t EntityFieldCount(char* entity_buffer);

	std::pair<std::string, std::string> EntityParseField(char* line);

	mapface_t ParseFace(char* line);

	mapbrush_t ParseBrush(char* brush_buffer);

	mapentity_t ParseEntity(char* entity_buffer);

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