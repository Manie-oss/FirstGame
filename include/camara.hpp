#ifndef MOVE_HPP
#define MOVE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <vector>

class camara {
   public:
      glm::mat4 view, projection;
      camara(GLFWwindow*);
      void computeMat(GLFWwindow*);
      glm::vec3 getPos() { return pos; };

   protected:
      GLfloat mouseSpeed, speed, jumpSpeed, vz;
      int wXCen, wYCen;
      glm::vec3 pos, jumpMove;
      double currT;
      std::vector<float> angle;
      bool isJump, isPreJump;

      void collisionDetect(glm::vec3&, glm::vec3&);
};

#endif
