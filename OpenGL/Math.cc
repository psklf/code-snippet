// glm quat
glm::vec3 eulerAngles(rad_X, rad_Y, rad_Z);
gRotateQuat = glm::quat(eulerAngles);

glm::mat4 thisRotateMatrix;
thisRotateMatrix = glm::mat4_cast(gRotateQuat);
