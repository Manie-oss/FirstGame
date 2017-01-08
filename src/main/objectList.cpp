#include <iostream>

#include "objectList.hpp"
#include "shader.hpp"
#include <sstream>


objectList::objectList(std::string vsName, std::string fsName) {

   if (vsName != "" && fsName != "")
      progID = LoadShaders(vsName.c_str(), fsName.c_str());
   else {
      std::cerr << "Failed to load shaders!\n";
      exit(EXIT_FAILURE);
   }

}


void objectList::createObject(std::string objFile, std::string texFile, glm::mat4 mMat) {

   objList.push_back(new obj(objFile, texFile, progID));
   //objList.back()->progID = progID;
   objList.back()->modelMat = mMat;

}

void objectList::createObject(std::string texFile, glm::mat4 mMat) {

   objList.push_back(new obj(texFile, progID));
   objList.back()->modelMat = mMat;

}



void objectList::drawScene(GLFWwindow* window) {

   cam->computeMat(window);
   glm::vec3 camPos = cam->getPos();
   GLuint cPosID = glGetUniformLocation(progID, "viewPos");
   glUniform3f(cPosID, camPos.x, camPos.y, camPos.z);

   lights = new Light(glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.3f));

   //lights->drawLight(progID, std::vector<glm::vec3>(1, glm::vec3(0.0f, 4.9f, 0.0f)));
   lights->drawLight(progID, objList[0]->mainRoom->lightPosition);

   for (unsigned iList=0; iList<objList.size(); ++iList) {
      glBindVertexArray(objList[iList]->VAO);

      objList[iList]->drawObj(cam->projection, cam->view);
      glDrawElements(GL_TRIANGLES, objList[iList]->indSize, GL_UNSIGNED_SHORT, (void*)0);
      glBindVertexArray(0);
   }
   glBindVertexArray(0);

}


objectList::~objectList() {

   for (unsigned iList=0; iList<objList.size(); ++iList)
      delete objList[iList];

   delete cam;
   glDeleteProgram(progID);

}



Light::Light(glm::vec3 ambIn, glm::vec3 diffIn, glm::vec3 specIn) {
   ambient = ambIn; diffuse = diffIn; specular = specIn;
}


void Light::drawLight(GLint pID, std::vector<glm::vec3> lPos) {

   //TODO: Need to figure out how to pass many lightsources
   for (GLuint iPos=0; iPos<lPos.size(); iPos++) {
      std::stringstream bName;
      bName << "light.pos[" << iPos << "]";
      glUniform3f(glGetUniformLocation(pID, bName.str().c_str()), lPos[iPos].x, lPos[iPos].y, lPos[iPos].z);
   }


   glUniform3f(glGetUniformLocation(pID, "light.ambient"),  ambient.x, ambient.y, ambient.z);
   glUniform3f(glGetUniformLocation(pID, "light.diffuse"),  diffuse.x, diffuse.y, diffuse.z);
   glUniform3f(glGetUniformLocation(pID, "light.specular"), specular.x, specular.y, specular.z);
   glUniform1f(glGetUniformLocation(pID, "light.linear"), 0.07);
   glUniform1f(glGetUniformLocation(pID, "light.quad"), 0.017);
   glUniform1i(glGetUniformLocation(pID, "light.num"), lPos.size());

}
