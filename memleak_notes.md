# Shared Data Types

# Sometimes copied


2. int* Model.vertices_y in model_copy_faces
2. int* Model.face_alphas in model_copy_faces
2. int* Model.face_infos in model_copy_faces
2. int* Model.face_color_a in model_copy_faces
2. int* Model.face_color_b in model_copy_faces
2. int* Model.face_color_c in model_copy_faces
2. int* Model.vertex_normal in model_copy_faces

2. int* Model.vertices_x in model_share_colored
2. int* Model.vertices_y in model_share_colored
2. int* Model.vertices_z in model_share_colored
2. int* Model.face_colors in model_share_colored
2. int* Model.face_alphas in model_share_colored

2. int* Model.vertices_x in model_share_alpha
2. int* Model.vertices_y in model_share_alpha
2. int* Model.vertices_z in model_share_alpha
2. int* Model.face_alphas in model_share_alpha


2. int* Model.face_color_a in model_calculate_normals
2. int* Model.face_color_b in model_calculate_normals
2. int* Model.face_color_c in model_calculate_normals
2. int* Model.vertex_normal in model_calculate_normals
2. int* Model.vertex_normal[i] in model_calculate_normals
2. int* Model.vertex_normal_original in model_calculate_normals
2. int* Model.vertex_normal_original[i] in model_calculate_normals






# Definitely shared
    new->vertex_count = src->vertex_count;
    new->face_count = src->face_count;
    new->textured_face_count = src->textured_face_count;
    new->vertices_x = src->vertices_x;
    new->vertices_z = src->vertices_z;
    new->face_colors = src->face_colors;
    new->face_alphas = src->face_alphas;
    new->face_priorities = src->face_priorities;
    new->model_priority = src->model_priority;
    new->face_indices_a = src->face_indices_a;
    new->face_indices_b = src->face_indices_b;
    new->face_indices_c = src->face_indices_c;
    new->textured_p_coordinate = src->textured_p_coordinate;
    new->textured_m_coordinate = src->textured_m_coordinate;
    new->textured_n_coordinate = src->textured_n_coordinate;
    new->max_y = src->max_y;
    new->min_y = src->min_y;
    new->radius = src->radius;
    new->min_depth = src->min_depth;
    new->max_depth = src->max_depth;
    new->min_x = src->min_x;
    new->max_z = src->max_z;
    new->min_z = src->min_z;
    new->max_x = src->max_x;