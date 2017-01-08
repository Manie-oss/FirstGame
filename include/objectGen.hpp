#ifndef OBJ_GEN_HPP
#define OBJ_GEN_HPP

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "buildRoom.hpp" //Function to build room


class obj {
    public:
        GLuint programID, VAO, indSize;
        glm::mat4 modelMat;
        room* mainRoom;

        obj(std::string, std::string, GLuint);
        obj(std::string, GLuint);
        ~obj();
        void drawObj(glm::mat4&, glm::mat4&);

    protected:
        std::vector<unsigned short> inds;
        std::vector<glm::vec2> uvsInds;
        std::vector<glm::vec3> vertInds, normInds;
        glm::mat4 camSpec;
        GLuint texID, tempID, vertID, uvID, normID, indID;

        void genBuffers(std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::vec2>);
};

#endif
