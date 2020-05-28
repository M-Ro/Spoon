// Only functions with triangles
// Todo: Triangulation function to convert quads
// Note this will most likely nullify model formats normal data if it exists

#pragma once

#include <vector>
#include <string>

#include <GL\glu.h>
#include <glm/glm.hpp>

class ModelSection;

class Model
{
public:
	Model();
	~Model();

	void Upload();

	void AddSection(ModelSection *section) { sections.push_back(section); }

	void SetTriangleCount(unsigned int tri_count) { triangle_count = tri_count; }

	std::vector<ModelSection *> &GetSections() { return sections; }

protected:
	std::vector<ModelSection *> sections;

	unsigned int triangle_count;
};

/* A 'segment' of the model that has a specific material */
class ModelSection
{
public:
	ModelSection(std::string material);
	~ModelSection();

	void Upload();

	void AddVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv);

	std::string const &GetMaterial() { return material; }

	GLuint const &GetVertexArray() { return vertexarray; }

	GLuint const &GetVertexBuffer() { return vertexbuffer; }

	GLuint const &GetUVBuffer() { return uvbuffer; }

	GLuint const &GetNormalBuffer() { return normalbuffer; }

	unsigned int GetVertexCount() { return vertices.size(); }

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::string material;

	GLuint vertexarray;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
};