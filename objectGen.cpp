#include <iostream>
using namespace std;

#include "objectGen.hpp"
#include "objLoader.hpp" //Quick load of objects
#include "vboIndexer.hpp" //indexing for verticies
#include "shader.hpp" //Function to load shaders
#include "texture.hpp" //Function to load textures
#include <SOIL/SOIL.h>
using namespace glm;

float pi = 3.141592;

obj::obj(std::string objName, std::string uvName, GLuint pID) {

   programID = pID;

   if (uvName != "") {
      texID = (loadDDS(uvName.c_str()));

      //Don't think this works for compressed images which the DDS is (I think!)
      /*glGenTextures(1, &texID);
      glBindTexture(GL_TEXTURE_2D, texID);

      int width, height;
      unsigned char* image = SOIL_load_image(uvName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

      glGenerateMipmap(GL_TEXTURE_2D);
      SOIL_free_image_data(image);
      glBindTexture(GL_TEXTURE_2D, 0);*/
   } else {
      cerr << "Failed to load UV map!\n";
      exit(EXIT_FAILURE);
   }

   std::vector<vec3> verts, norms;
   std::vector<vec2> uvs;
   if (!loadOBJ(objName.c_str(), verts, uvs, norms)) {
      cerr << "Failed to load asset!\n";
      exit(EXIT_FAILURE);
   }

   genBuffers(verts, norms, uvs);
}


obj::obj(std::string uvName, GLuint pID) {

   programID = pID;

   if (uvName != "")
      texID = (loadDDS(uvName.c_str()));
   else {
      cerr << "Failed to load UV map!\n";
      exit(EXIT_FAILURE);
   }

   mainRoom = new room();

   genBuffers(mainRoom->roomVerts, mainRoom->roomNorms, mainRoom->roomUVs);
}




void obj::genBuffers(std::vector<vec3> bVerts, std::vector<vec3> bNorms, std::vector<vec2> bUVs) {

   indexVBO(bVerts, bUVs, bNorms, inds, vertInds, uvsInds, normInds);
   indSize = inds.size();

   glGenVertexArrays(1, &VAO);
   glBindVertexArray(VAO);

   glUseProgram(programID);

   //Bind vertices to shader
   glGenBuffers(1, &vertID);
   glBindBuffer(GL_ARRAY_BUFFER, vertID);
   glBufferData(GL_ARRAY_BUFFER, bVerts.size() * sizeof(vec3), &vertInds[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   //Bind texture locations to shader
   glGenBuffers(1, &uvID);
   glBindBuffer(GL_ARRAY_BUFFER, uvID);
   glBufferData(GL_ARRAY_BUFFER, bUVs.size() * sizeof(vec2), &uvsInds[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

   //Bind normals to shader
   glGenBuffers(1, &normID);
   glBindBuffer(GL_ARRAY_BUFFER, normID);
   glBufferData(GL_ARRAY_BUFFER, bNorms.size() * sizeof(vec3), &normInds[0], GL_STATIC_DRAW);
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

   //Bind VBO indices to shader
   glGenBuffers(1, &indID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(unsigned short), &inds[0] , GL_STATIC_DRAW);

   // Bind our texture in Texture Unit 0
   glUniform1i(glGetUniformLocation(programID, "material.diffuse"), 0);
   glUniform1i(glGetUniformLocation(programID, "material.specular"), 0);
   glUniform1f(glGetUniformLocation(programID, "material.shininess"), 32.0f);

   glBindVertexArray(0);
}




void obj::drawObj(glm::mat4& projection, glm::mat4& camSpec) {

    //Generate and send camara matrix
   glm::mat4 camView = projection * camSpec * modelMat;
   glUniformMatrix4fv(glGetUniformLocation(programID, "camView"), 1, GL_FALSE, &camView[0][0]);

   //Send model matrix
   glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &modelMat[0][0]);

   //Resets the texture
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texID);
}




obj::~obj() {

    glDeleteTextures    (1, &texID);
    glDeleteVertexArrays(1, &tempID);
    glDeleteBuffers     (1, &vertID);
    glDeleteBuffers     (1, &uvID);
    glDeleteBuffers     (1, &normID);
    glDeleteBuffers     (1, &indID);

}


