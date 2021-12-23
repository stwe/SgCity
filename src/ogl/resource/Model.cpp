#include "Model.h"
#include "SgException.h"
#include "Log.h"
#include "ogl/buffer/Vao.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//-------------------------------------------------
// Ctors. / Dtor.
//-------------------------------------------------

sg::ogl::resource::Model::Model(std::string t_path)
    : m_path{ std::move(t_path) }
{
    Log::SG_LOG_DEBUG("[Model::Model()] Create Model.");
}

sg::ogl::resource::Model::~Model() noexcept
{
    Log::SG_LOG_DEBUG("[Model::~Model()] Destruct Model.");

    CleanUp();
}

//-------------------------------------------------
// Load
//-------------------------------------------------

void sg::ogl::resource::Model::Load()
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "";

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(m_path, reader_config))
    {
        if (!reader.Error().empty())
        {
            throw SG_EXCEPTION("[Model::Load()] Error: " + reader.Error());
        }
    }

    if (!reader.Warning().empty())
    {
        Log::SG_LOG_WARN("[Model::Load()] Warning: {}.", reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<float> vertices;

    // Loop over shapes
    for (auto s{ 0u }; s < shapes.size(); ++s)
    {
        // Loop over faces(polygon)
        auto index_offset{ 0u };
        for (auto f{ 0u }; f < shapes[s].mesh.num_face_vertices.size(); ++f)
        {
            auto fv{ size_t(shapes[s].mesh.num_face_vertices[f]) };

            // Loop over vertices in the face.
            for (auto v{ 0u }; v < fv; ++v)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);

                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

                    vertices.push_back(tx);
                    vertices.push_back(ty);
                }
            }

            index_offset += fv;

            // per-face material
            //shapes[s].mesh.material_ids[f];
        }

        auto vao{ std::make_unique<ogl::buffer::Vao>() };
        vao->CreateStaticVbo(vertices, 4 * 4); // todo: drawCount
        m_vaos.push_back(std::move(vao));
    }
}

//-------------------------------------------------
// Clean up
//-------------------------------------------------

void sg::ogl::resource::Model::CleanUp() const
{
    Log::SG_LOG_DEBUG("[Model::CleanUp()] Clean up Model.");

    // ...
}
