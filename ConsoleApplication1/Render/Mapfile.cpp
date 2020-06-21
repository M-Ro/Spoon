#include <windows.h> // why do we need this everywhere?

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>

#define _USE_MATH_DEFINES // for M_PI
#include <math.h>

#include "../Game/Game.h"

#include "../auxiliary/Filehandle.h"
#include "../Auxiliary/Functions.h"

#include "Mapfile.h"

#ifdef _WIN32
    #define strtok_r strtok_s
#endif

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

const float MIN_DELTA = 10e-10; // idk

// from radiant tools/quake3/q3map2/map.c
glm::vec3 texture_baseaxis[18] = {
    // normal			texture plane
    /*glm::vec3(0, 0, 1),	 glm::vec3(1, 0, 0), glm::vec3(0, -1, 0),	// floor
    glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0),	// ceiling
    glm::vec3(1, 0, 0),	 glm::vec3(0, 1, 0), glm::vec3(0, 0, -1),	// west wall
    glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, -1),	// east wall
    glm::vec3(0, 1, 0),	 glm::vec3(1, 0, 0), glm::vec3(0, 0, -1),	// south wall
    glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1)	// north wall*/

    /*glm::vec3(0, 1, 0),	 glm::vec3(1, 0, 0), glm::vec3(0, 0, -1),	// floor
    glm::vec3(0, -1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1),	// ceiling
    glm::vec3(1, 0, 0),	 glm::vec3(0, 0, 1), glm::vec3(0, -1, 0),	// west wall
    glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0),	// east wall
    glm::vec3(0, 0, 1),	 glm::vec3(1, 0, 0), glm::vec3(0, -1, 0),	// south wall
    glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)	// north wall*/

    glm::vec3(0, -1, 0),	 glm::vec3(1, 0, 0), glm::vec3(0, 0, -1),	// floor
    glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, -1),	// ceiling
    glm::vec3(1, 0, 0),	 glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),	// west wall
    glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0),	// east wall
    glm::vec3(0, 0, 1),	 glm::vec3(1, 0, 0), glm::vec3(0, 1, 0),	// south wall
    glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)	// north wall
};

// from radiant tools/quake3/q3map2/map.c
// determines best orthagonal axis to project a texture onto a wall (must be identical in radiant!)
void texture_axis_from_plane(const mapface_t& face, glm::vec3 &xv, glm::vec3 &yv)
{
    size_t best_axis = 0;
    float dot = 0;
    float best = 0;


    glm::vec3 tv1 = face.points[1] - face.points[0];
    glm::vec3 tv2 = face.points[2] - face.points[1];
    glm::vec3 n = glm::normalize(glm::cross(tv1, tv2) * -1.0f);

    for (size_t i = 0; i < 6; i++) {
        dot = glm::dot(n, texture_baseaxis[i * 3]);
        if (dot > best + MIN_DELTA) {	/* ydnar: bug 637 fix, suggested by jmonroe */
            best = dot;
            best_axis = i;
        }
    }

    for (int i = 0; i < 3; i++) {
        xv[i] = texture_baseaxis[best_axis * 3 + 1][i];
        yv[i] = texture_baseaxis[best_axis * 3 + 2][i];
    }
}

// from radiant tools/quake3/q3map2/map.c
// creates world-to-texture mapping vecs for crappy quake plane arrangements
void face_texture_verts(mapface_t &face, const glm::vec2 &tex_shift, const float tex_rotate, const glm::vec2 &tex_scale)
{
    glm::vec3 vecs[2];
    glm::vec2 scale(tex_scale);

    int		sv, tv;
    float ang, sinv, cosv;
    float ns, nt;
    int		i, j;

    texture_axis_from_plane(face, vecs[0], vecs[1]);

    if (!scale[0])
        scale[0] = 1;
    if (!scale[1])
        scale[1] = 1;

    // rotate axis
    if (tex_rotate == 0.0f) {
        sinv = 0.0f;
        cosv = 1.0f;
    }
    else if (tex_rotate == 90.0f) {
        sinv = 1.0f;
        cosv = 0.0f;
    }
    else if (tex_rotate == 180.0f) {
        sinv = 0.0f;
        cosv = -1.0f;
    }
    else if (tex_rotate == 270.0f) {
        sinv = -1.0f;
        cosv = 0.0f;
    }
    else {
        ang = tex_rotate / 180.0f * M_PI;
        sinv = sinf(ang);
        cosv = cosf(ang);
    }

    if (vecs[0][0])
        sv = 0;
    else if (vecs[0][1])
        sv = 1;
    else
        sv = 2;

    if (vecs[1][0])
        tv = 0;
    else if (vecs[1][1])
        tv = 1;
    else
        tv = 2;

    for (i = 0; i < 2; i++) {
        ns = cosv * vecs[i][sv] - sinv * vecs[i][tv];
        nt = sinv * vecs[i][sv] + cosv * vecs[i][tv];
        vecs[i][sv] = ns;
        vecs[i][tv] = nt;
    }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 3; j++)
            face.face_tex_vec[i][j] = vecs[i][j] / scale[i];

    face.face_tex_shift = tex_shift;
}

// from radiant tools/quake3/q3map2/map.c
// project vertex into texture plane
const glm::vec2 map_texture_coords(mapface_t *face, const glm::vec3 &v, size_t tw, size_t th)
{
    return glm::vec2(
        (face->face_tex_shift.x + glm::dot(face->face_tex_vec[0], v)) / tw,
        (-face->face_tex_shift.y + glm::dot(face->face_tex_vec[1], v)) / th
    );
}

Mapfile::Mapfile(std::string const& filepath)
{
	this->Load(filepath);
}

Mapfile::~Mapfile()
{
	if (this->data) {
		delete this->data;
	}
}

void Mapfile::BuildRenderModel()
{
    // First we need to iterate through all the brush faces and create unique model sections for each material name
    std::map<std::string, ModelSection*> sections;

    size_t triangle_count = 0;

    for (mapentity_t& ent : this->map_entities) {
        for (mapbrush_t& brush : ent.brushes) {
            for (mapface_t& face : brush.faces) {
                std::string materialname(face.texture);

                if (!sections.count(materialname)) {
                    sections.emplace(materialname, new ModelSection(materialname));
                }

                ModelSection *section = sections.at(materialname);

                // FIXME texture uvs
                face_texture_verts(face, glm::vec2(face.xoffset, face.yoffset), face.rotation, glm::vec2(face.xscale, face.yscale));

                // We need to pull the texture from the renderer so we can get the width and height
                int tw = 64;
                int th = 64;

                Texture* tex = game->renderer->GetTexture(face.texture);
                if (tex) {
                    tw = tex->GetWidth();
                    th = tex->GetHeight();
                }

                for (size_t i = 1; i < face.vertices_calculated.size() - 1; i++) {
                    glm::vec3 fn = glm::normalize(glm::cross(face.vertices_calculated[i] - face.vertices_calculated[i-1], face.vertices_calculated[i+1] - face.vertices_calculated[i]));

                    glm::vec2 t1 = map_texture_coords(&face, face.vertices_calculated[0], tw, th);
                    glm::vec2 t2 = map_texture_coords(&face, face.vertices_calculated[i], tw, th);;
                    glm::vec2 t3 = map_texture_coords(&face, face.vertices_calculated[i+1], tw, th);;

                    section->AddVertex(face.vertices_calculated[0], fn, t1);
                    section->AddVertex(face.vertices_calculated[i], fn, t2);
                    section->AddVertex(face.vertices_calculated[i+1], fn, t3);

                    triangle_count++;
                }
            }
        }
    }

    Model* model = new Model();
    model->SetTriangleCount(triangle_count);

    for (const auto& kv : sections) {
        model->AddSection(kv.second);
    }

    game->renderer->PushModel("balls", model);
    model->Upload();
}

// Attempts to load file from the given filepath, if successful
// will continue on to parsing said file.
void Mapfile::Load(std::string const& filepath)
{
	Filehandle filehandle = Filehandle(filepath, true);
	if (!filehandle.IsOpen()) {
		std::cout << "Failed to open file " << filepath << std::endl;
		return;
	}

    this->filename.assign(filepath);
	this->data = reinterpret_cast<const char*>(filehandle.ReadFile());

    std::cout << "Parsing " << this->filename << std::endl;

    this->ParseMapFromBuffer();

    this->BuildRenderModel();
}

// Parses map from loaded string buffer 
void Mapfile::ParseMapFromBuffer()
{
	if (!this->data) {
		return;
	}

    size_t depth = 0; // Current parser depth level in { }
    char* ebuffer = (char*)malloc(1024 * 1024 * 2); // 2MB
    memset(ebuffer, 0, 1024 * 1024 * 2);
    int ebufferlen = 0;

    char* mapfile = (char *)malloc(strlen(this->data) + 1);
    memcpy(mapfile, this->data, strlen(this->data) + 1);

    char* rest = NULL;
    char* line = strtok_r(mapfile, "\n", &rest);
    while (line != NULL) {
        if (line[0] == '/') { // Ignore comments
            line = strtok_r(NULL, "\n", &rest);
            continue;
        }

        memcpy(ebuffer + ebufferlen, line, strlen(line));
        ebufferlen += strlen(line);
        ebuffer[ebufferlen] = '\n';
        ebufferlen++;

        if (line[0] == '{') {
            depth++;
        }
        else if (line[0] == '}') {
            depth--;

            if (depth == 0) {
                this->map_entities.push_back(this->ParseEntity(ebuffer));

                // Clear buffer for next ent
                memset(ebuffer, 0, 1024 * 1024 * 2);
                ebufferlen = 0;
            }
        }

        line = strtok_r(NULL, "\n", &rest);
    }

    free(ebuffer);
    free(mapfile);
}

void Mapfile::Deduplicate(mapbrush_t& brush)
{
    const float THRESH = 0.1f;

    for (int j = 0; j < brush.faces.size(); j++)
    {
        mapface_t* f = &brush.faces[j];
        for (int i = 0; i < f->vertices_calculated.size(); i++) {
            for (int k = 0; k < f->vertices_calculated.size(); k++) {
                if (i == k) {
                    continue;
                }

                glm::vec3 vi = f->vertices_calculated[i];
                glm::vec3 vk = f->vertices_calculated[k];

                int in_threshold = 0;
                for (int p = 0; p < 3; p++) {
                    if ((float)abs(vi[p] - vk[p]) < THRESH) {
                        in_threshold++;
                    }
                }

                if (in_threshold == 3) {
                    f->vertices_calculated.erase(f->vertices_calculated.begin() + k);
                    k--;
                }
            }
        }
    }
}

void Mapfile::TrimBrushPlanes(mapbrush_t& brush)
{
    for (int j = 0; j < brush.faces.size(); j++)
    {
        mapface_t* f = &brush.faces[j];
        for (int i = 0; i < brush.planes.size(); i++)
        {
            plane_t p = brush.planes[i];
            if (i == j)
                continue;

            for (int k = 0; k < f->vertices_calculated.size(); k++)     // Trim the face
            {
                glm::vec3 dir = f->vertices_calculated[(k + 1) % f->vertices_calculated.size()] - f->vertices_calculated[k];

                if (abs(glm::dot(dir, p.normal)) < 0.01) {    // If this product is 0, then the edge and plane are parallel and can't intersect(or they are on top of each others but we don't care about that either)
                    continue;
                }

                float dist = glm::dot(p.v1 - f->vertices_calculated[k], p.normal) / glm::dot(dir, p.normal);

                if (dist < 0.0 || dist > 1.0) { // If either is true, then the plane intersects the extrapolated line
                    continue;
                }

                glm::vec3 intersectionPoint = f->vertices_calculated[k] + (dist * dir);
                f->vertices_calculated.insert(f->vertices_calculated.begin() + k + 1, intersectionPoint);
                k++;
            }
            for (int k = 0; k < f->vertices_calculated.size(); k++)     //	Trim the face
            {
                if (glm::dot(p.normal, f->vertices_calculated[k] - p.v1) < -0.01) {
                    f->vertices_calculated.erase(f->vertices_calculated.begin() + (k--));
                }
            }
        }
    }
}

size_t Mapfile::EntityBrushCount(char* entity_buffer)
{
    char* tmp = (char *)malloc((strlen(entity_buffer) + 1) * sizeof(char));
    char* freetmp = tmp; // Unmangled pointer for free() later
    memcpy(tmp, entity_buffer, strlen(entity_buffer));

    int depth = 0;
    int nbrushes = 0;

    char* rest;
    char* line = strtok_r(tmp, "\n", &rest);
    while (line != NULL) {

        if (line[0] == '{')
        {
            if (depth == 1) {
                nbrushes++;
            }
            depth++;
        }
        else if (line[0] == '}')
        {
            depth--;
        }

        line = strtok_r(NULL, "\n", &rest);
    }

    free(freetmp);
    return nbrushes;
}

size_t Mapfile::EntityFieldCount(char* entity_buffer)
{
    char* tmp = (char *)malloc((strlen(entity_buffer) + 1) * sizeof(char));
    char* freetmp = tmp;
    memcpy(tmp, entity_buffer, strlen(entity_buffer));

    int nfields = 0;

    char* rest = NULL;
    char* line = strtok_r(tmp, "\n", &rest);
    while (line != NULL) {

        if (line[0] == '"')
        {
            nfields++;
        }

        line = strtok_r(NULL, "\n", &rest);
    }

    free(freetmp);
    return nfields;
}

std::pair<std::string, std::string> Mapfile::EntityParseField(char* line)
{
    // Find quote positions
    int qpos[4] = { 0, 0, 0, 0 };
    int qc = 0;
    size_t len = strlen(line);

    for (int i = 0; i < len; i++) {
        char c = line[i];
        if (c == '"') {
            qpos[qc] = i;
            qc++;
        }
    }

    // 0set key/val
    std::pair<std::string, std::string> field;

    for (int i = qpos[0] + 1; i < qpos[1]; i++)
    {
        field.first.push_back(line[i]);
    }

    for (int i = qpos[2] + 1; i < qpos[3]; i++)
    {
        field.second.push_back(line[i]);
    }

    return field;
}

mapface_t Mapfile::ParseFace(char* line)
{
    mapface_t face;

    // First we parse the points
    long long cstart = -1; // character index
    for (int i = 0; i < 3; i++)
    {
        char* pointbuf = (char *)malloc(256 + sizeof(char));
        char* freepointbuf = pointbuf;
        int pindex = 0;
        memset(pointbuf, 0, 256);

        cstart++;
        char c = line[cstart];
        while (c != ')') {
            pointbuf[pindex] = c;

            cstart++;
            pindex++;
            c = line[cstart];
        }

        // Trim
        for (int p = 0; p < strlen(pointbuf); p++)
        {
            if (pointbuf[p] == '(' || pointbuf[p] == ')')
            {
                pointbuf[p] = ' ';
            }
        }
        pointbuf = ctrim(pointbuf);

        char* rest = NULL;
        char* coord = strtok_r(pointbuf, " ", &rest);
        int n = 0;
        while (coord != NULL) {
            switch (n)
            {
            case 0: face.points[i].x = (float)atof(coord); break;
            case 1: face.points[i].z = (float)atof(coord); break;
            case 2: face.points[i].y = (float)atof(coord); break;
            }

            n++;
            coord = strtok_r(NULL, " ", &rest);
        }

        free(freepointbuf);
    }

    // Now we parse the texture, offsets, rot and scale from the face line
    char* facepropbuf = (char *)malloc((strlen(line) + 1) - (cstart + 1));
    char* freefacepropbuf = facepropbuf;
    memset(facepropbuf, 0, (strlen(line)+1) - (cstart + 1));
    memcpy(facepropbuf, line + cstart + 1, strlen(line + cstart + 1));
    facepropbuf = ctrim(facepropbuf);

    memset(face.texture, 0, 64);
    memcpy(face.texture, "textures/", 9);

    char* rest = NULL;
    int n = 0;
    char* prop = strtok_r(facepropbuf, " ", &rest);
    while (prop != NULL) {
        switch (n)
        {
        case 0: memcpy(face.texture + 9, prop, strlen(prop)); break;
        case 1: face.xoffset = (float)atof(prop); break;
        case 2: face.yoffset = (float)atof(prop); break;
        case 3: face.rotation = (float)atof(prop); break;
        case 4: face.xscale = (float)atof(prop); break;
        case 5: face.yscale = (float)atof(prop); break;
        }

        n++;
        prop = strtok_r(NULL, " ", &rest);
    }

    // Ensure face texture is lowercase always
    for (int i = 0; face.texture[i]; i++) {
        face.texture[i] = tolower(face.texture[i]);
    }

    free(freefacepropbuf);

    return face;
}

mapbrush_t Mapfile::ParseBrush(char* brush_buffer)
{
    mapbrush_t brush;

    // Get face count
    brush.nfaces = 0;
    for (int i = 0; i < strlen(brush_buffer); i++)
    {
        if (brush_buffer[i] == '\n')
        {
            brush.nfaces++;
        }
    }

    // Parse faces
    char* rest = NULL;
    char* line = strtok_r(brush_buffer, "\n", &rest);
    while (line != NULL) {
        mapface_t face = this->ParseFace(line);

        // Add plane from this brush
        glm::vec3 tv1 = face.points[1] - face.points[0];
        glm::vec3 tv2 = face.points[2] - face.points[1];
        glm::vec3 normal = -glm::normalize(glm::cross(tv1, tv2));

        plane_t plane{
            face.points[0],
            face.points[1],
            face.points[2],
            normal
        };

        //std::cout << "Normal: " << normal.x << std::endl;

        brush.planes.push_back(plane);

        // Add the face
        float scale = 4096.0f;
        glm::vec3 fv1 = glm::normalize(plane.v3 - plane.v1) * scale;
        glm::vec3 fv2 = glm::normalize(plane.v2 - plane.v1) * scale;

        face.vertices_calculated.push_back(fv1 + fv2 + plane.v1);
        face.vertices_calculated.push_back(fv1 - fv2 + plane.v1);
        face.vertices_calculated.push_back(glm::vec3(0, 0, 0) - fv1 - fv2 + plane.v1);
        face.vertices_calculated.push_back(glm::vec3(0, 0, 0) - fv1 + fv2 + plane.v1);

        brush.faces.push_back(face);


        line = strtok_r(NULL, "\n", &rest);
    }

    this->TrimBrushPlanes(brush);
    this->Deduplicate(brush);

    return brush;
}

mapentity_t Mapfile::ParseEntity(char* entity_buffer)
{
    mapentity_t ent;

    char* tmp = (char *)malloc((strlen(entity_buffer) + 1) * sizeof(char));
    tmp[strlen(entity_buffer)] = '\0';
    memcpy(tmp, entity_buffer, strlen(entity_buffer));

    char* freetmp = tmp;

    int depth = 0;
    int findex = 0; // field index

    int bindex = 0; // brush index
    char* bbuffer = (char*)malloc(1024 * 16);
    memset(bbuffer, 0, 1024 * 16);
    size_t bbufferlen = 0;

    char* rest = NULL;
    char* line = strtok_r(tmp, "\n", &rest);

    while (line != NULL) {
        if (line[0] == '"')
        {
            std::pair<std::string, std::string> prop = this->EntityParseField(line);
            ent.properties.emplace(prop);

            findex++;
            line = strtok_r(NULL, "\n", &rest);
            continue;
        }
        else if (line[0] == '{')
        {
            depth++;
        }
        else if (line[0] == '}')
        {
            if (depth == 2)
            {
                mapbrush_t brush = this->ParseBrush(bbuffer);
                ent.brushes.push_back(brush);

                // Do stuff
                memset(bbuffer, 0, 1024 * 16);
                bbufferlen = 0;
                bindex++;
            }

            depth--;
        }
        else {
            memcpy(bbuffer + bbufferlen, line, strlen(line));
            bbufferlen += strlen(line);
            bbuffer[bbufferlen] = '\n';
            bbufferlen++;
        }

        line = strtok_r(NULL, "\n", &rest);
    }

    free(bbuffer);
    free(freetmp);

    return ent;
}

