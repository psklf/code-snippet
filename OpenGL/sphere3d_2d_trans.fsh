// calculate attitude correct
"    vec4 new_pos = attitude_mat4 * vary_position;\n"
"    float alpha = -1.0f * asin(new_pos.y);\n"
"    float beta = atan(new_pos.z, new_pos.x);\n"
"    if (beta < 0.0f) {\n"
"        beta += 2.0f * PI;\n"
"    }\n"
"    vec2 texture_coord = vec2(0.5f * (beta) / PI, alpha / PI + 0.5f);\n"
