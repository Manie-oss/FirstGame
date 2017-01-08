#ifndef OBJ_LIST_HPP
#define OBJ_LIST_HPP

#include "objectGen.hpp"
#include "camara.hpp" //function to move camara


#include <vector>


class Light {
   public:
      Light(glm::vec3, glm::vec3, glm::vec3);
      virtual ~Light() {};

      void drawLight(GLint, std::vector<glm::vec3>);

   protected:
      glm::vec3 ambient, diffuse, specular;
};


class objectList {
   public:
      camara* cam;
      Light* lights;

      objectList(std::string, std::string);
      virtual ~objectList();
      void createObject(std::string, std::string, glm::mat4);
      void createObject(std::string, glm::mat4);
      void drawScene(GLFWwindow*);

   protected:
      std::vector<obj*> objList;
      GLuint progID;
};



#endif // OBJ_LIST_HPP
