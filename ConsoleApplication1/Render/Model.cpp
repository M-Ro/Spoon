#include <windows.h> // Needed before including GL.h on WIN32
#include <GL\glew.h>

#include "Model.h"

#include <iostream>

Model::Model()
{

}

Model::~Model()
{
	for(ModelSection *section : sections)
		delete section;

	sections.clear();
}

void Model::Upload()
{
	for(ModelSection *section : sections)
		if(section)
			section->Upload();
}

ModelSection::ModelSection(std::string material)
{
	this->material = material;
}

ModelSection::~ModelSection()
{
	/* Clear the lists */
	vertices.clear();
	normals.clear();
	uvs.clear();
}

void ModelSection::Upload()
{
	/* Create and bind the VAO to hold our buffers */
	glGenVertexArrays(1, &vertexarray);
	glBindVertexArray(vertexarray);

	// Load section into VBO
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	//glEnableVertexAttribArray(0);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);

	glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

    glBindVertexArray(0); // Cleanup, unbind VAO
}

void ModelSection::AddVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
{
	vertices.push_back(vertex);
	normals.push_back(normal);
	uvs.push_back(uv);
}