#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "../auxiliary/Filehandle.h"

#include "OBJModel.h"
#include <glm/glm.hpp>

// TODO DOESN'T TRIM LINES (HANDLE SPACES)
// Handle generation of normals

namespace OBJModel
{

bool IsComment(std::string const &line);

Model *Load(std::string const &filepath)
{
	Filehandle filehandle = Filehandle(filepath, true);
	if(!filehandle.IsOpen())
		return 0;

	std::istream *iss = filehandle.GetIStream();

	unsigned int triangle_count = 0; // Informative purposes

	std::vector<glm::vec3> obj_vertices;
	std::vector<glm::vec2> obj_uvs;
	std::vector<glm::vec3> obj_normals;

	std::string active_material;

	Model *model = new Model();
	ModelSection *current_section = 0;

	int line_num = 0;
	std::string line;
	while (getline(*iss, line))
	{
		line_num++;

		if(line.empty())
			continue;

		if(IsComment(line))
			continue;

		std::stringstream ss(line);

		std::string keyword;
		ss >> keyword;

		if(keyword == "v") // Vertex
		{
			float x, y, z;

			if(!(ss >> x >> y >> z))
			{
				std::cout << "Parse error on line " << line_num << std::endl;
				continue;
			}

			obj_vertices.push_back(glm::vec3(x, y, z));
		}
		else if(keyword == "vt") // UV Coordinate
		{
			float x, y;

			if(!(ss >> x >> y))
			{
				std::cout << "Parse error on line " << line_num << std::endl;
				continue;
			}

			obj_uvs.push_back(glm::vec2(x, y));
		}
		else if(keyword == "vn") // Vertex Normal
		{
			float x, y, z;
			if(!(ss >> x >> y >> z))
			{
				std::cout << "Parse error on line " << line_num << std::endl;
				continue;
			}

			obj_normals.push_back(glm::vec3(x, y, z));
		}
		else if(keyword == "usemtl") // Specify material/texture
		{
			/* The active material has changed, add current modelsection to
				our model and start a new model section */
			if(current_section != 0)
				model->AddSection(current_section);

			current_section = 0;
			active_material = "";

			if(!(ss >> active_material))
			{
				std::cout << "Parse error on line " << line_num << std::endl;
				continue;
			}

			current_section = new ModelSection(active_material);
		}
		else if(keyword == "f") // Face/polygon definition
		{
			/* If we have no fragment (i.e no material was specified), create one! */
			if(current_section == 0)
				current_section = new ModelSection("textures/null");

			/* Parse the line (3 vertex points) */
			for(int i=0; i<3; i++)
			{
				std::string point;
				ss >> point;

				std::replace(point.begin(), point.end(), '/', ' ');
				std::stringstream stream_point(point);

				unsigned int v_index, uv_index, vn_index;

				if(!(stream_point >> v_index >> uv_index >> vn_index))
				{
					std::cout << "Parse error on line " << line_num << std::endl;
					continue;
				}

				/* Subtract 1 from indexes. Obj indexing starts at 1. Vectors start at 0 */
				v_index--;
				vn_index--;
				uv_index--;

				/* Sanity check to ensure he aren't accessing invalid data */
				if(v_index < 0 || vn_index < 0 || uv_index < 0 ||
					v_index >= obj_vertices.size() || vn_index >= obj_normals.size() || uv_index >= obj_uvs.size())
				{
					std::cout << "Invalid index on line: " << line_num << std::endl;
					continue;
				}

				/* Add vertex to our model section */
				current_section->AddVertex(obj_vertices[v_index], obj_normals[vn_index], obj_uvs[uv_index]);
			}

			triangle_count++;
		}
	}

	/* Add last section to model if necessary */
	if(current_section != 0)
		model->AddSection(current_section);

	model->SetTriangleCount(triangle_count);

	std::cout << "Loaded: " << filepath << " Vertices: " << obj_vertices.size() << " Triangles: " << triangle_count << std::endl;
	
	return model;
}

bool IsComment(std::string const &line)
{
	return line.at(0) == '#';
}

}
