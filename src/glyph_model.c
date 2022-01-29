#include <glyph.h>

static inline void vertex_array_bind(unsigned int id, unsigned int* VAO, array_t* vertices, array_t* indices, obj_flag type)
{
    if (!vertices || !indices) return;
    unsigned int EBO;
    glGenBuffers(1, VAO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, *VAO);
    glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->used * indices->bytes, indices->data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)0);
    if (type == OBJ_VTN) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, uv));
    }
    if (type != OBJ_V) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, normal));
    }
}

static inline void vmesh_bind(unsigned int id, unsigned int* VAO, vmesh_t* mesh)
{
    if (mesh) vertex_array_bind(id, VAO, mesh->vertices, mesh->indices, mesh->type);
}

void model3D_rebind(const Model3D* restrict model)
{
    glBindVertexArray(model->id);
    glBindBuffer(GL_ARRAY_BUFFER, model->VAO);
    glBufferData(GL_ARRAY_BUFFER, model->mesh->vertices->used * model->mesh->vertices->bytes, model->mesh->vertices->data, GL_STATIC_DRAW);
}

Model3D model3D_new(vmesh_t* mesh, const texture_t texture)
{
    Model3D model;
    model.id = glee_buffer_id();
    model.mesh = mesh;
    model.texture = texture;
    vmesh_bind(model.id, &model.VAO, model.mesh);
    return model;
}

void model3D_free(Model3D* model)
{
    vmesh_free(model->mesh);
}