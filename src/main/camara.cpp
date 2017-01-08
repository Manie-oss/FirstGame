#include <iostream>

#include "camara.hpp"
using namespace glm;


const float myPi = 3.141592;


camara::camara(GLFWwindow* window) {

   projection = perspective(45.0f, 4.0f/3.0f, 0.1f, 200.0f);
   pos = glm::vec3(0.0f, 2.0f, 0.0f);
   angle.resize(2,0);
   isJump = false;

   glfwGetWindowSize(window, &wXCen, &wYCen);
   wXCen /= 2; wYCen /= 2;

   mouseSpeed = 0.1f, speed = 10.0f, jumpSpeed = 3.0f;
   vz = jumpSpeed;

}



void camara::computeMat(GLFWwindow* window) {

   GLfloat dT = glfwGetTime() - currT;
   currT = glfwGetTime();

   vec3 facing(0, 0, 1.0f);
   if (currT > 1e0) {
      double xPos, yPos;
      glfwGetCursorPos(window, &xPos, &yPos);

      angle[0] += mouseSpeed * dT * float(wXCen - xPos);
      double dTheta = mouseSpeed * dT * float(wYCen - yPos);
      if (angle[1] + dTheta <= -0.9*myPi/2.0f)
         angle[1] = -0.9*myPi/2.0f;
      else if (angle[1] + dTheta > 0.9*myPi/2.0f)
         angle[1] = 0.9*myPi/2.0f;
      else
         angle[1] += dTheta;

      facing = vec3(cos(angle[1]) * sin(angle[0]),
                    sin(angle[1]),
                    cos(angle[1]) * cos(angle[0]));

      float wallBuffer = 9.7f;
      vec3 direction(facing[0], 0, facing[2]), right(facing[2], 0, -facing[0]);

      std::vector<short> isPressed(4,0);
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
         isPressed[0] = 1;
      if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
         isPressed[1] = 1;
      if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
         isPressed[2] = 1;
      if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
         isPressed[3] = 1;
      if ((!isJump) & (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)) {
         isJump = true; isPreJump = true;
      }

      vec3 mov(0.0f), tempPos(pos);
      if ((isPressed[0] || isPressed[1] || isPressed[2] || isPressed[3])) {
         mov =   isPressed[0] * dT * speed * direction
               - isPressed[1] * dT * speed * direction
               + isPressed[2] * dT * speed * right
               - isPressed[3] * dT * speed * right;

         tempPos = pos + mov;
      }

      //Need to stop changes in movement while jumping
      if (isJump) {
         if (isPreJump) {
            jumpMove = mov; isPreJump = false;
         }

         pos[1] += vz * dT;
         vz += -9.81 * dT;

         tempPos = pos + jumpMove;

         if (pos[1] < 2.0) {
            pos[1] = 2.0;
            isJump = false;
            vz = jumpSpeed;
            isPreJump = true;
         }
      }

      collisionDetect(pos, tempPos);

    }

    glfwSetCursorPos(window, wXCen, wYCen);
    view = lookAt(pos, pos + facing, vec3(0,1,0));
}






void camara::collisionDetect(glm::vec3& start, glm::vec3& finish) {

   start = finish;

}
