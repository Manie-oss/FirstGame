#include <iostream>
#include "buildRoom.hpp"

room::room() {

   rScale = 4;
   std::vector<glm::vec3> normals;
   std::vector< std::vector<double> > boundary;
   std::vector< std::vector<int> > blocks = createBlocks(100);

   createBound(blocks, boundary, normals);
   buildWalls(boundary, normals);
   buildFloorAndRoof(blocks);

}







void room::buildFloorAndRoof(std::vector< std::vector<int> > blocks) {

   std::vector<glm::vec3> floorVerts(6*blocks.size(), glm::vec3(0)), roofVerts(6*blocks.size(), glm::vec3(0));
   std::vector<glm::vec3> floorNorm(6*blocks.size(), glm::vec3(0,1,0)), roofNorm(6*blocks.size(), glm::vec3(0,-1,0));
   std::vector<glm::vec2> floorUV(6*blocks.size(), glm::vec2(0)), roofUV(6*blocks.size(), glm::vec2(0));

   double xShift[6] = {-0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
   double yShift[6] = {-0.5, 0.5, -0.5, -0.5, 0.5, 0.5};
   glm::vec2 uvs[6] = {glm::vec2(0.748573, 0.501077),
                       glm::vec2(0.748573, 0.750412),
                       glm::vec2(0.999110, 0.501077),
                       glm::vec2(0.999110, 0.501077),
                       glm::vec2(0.748573, 0.750412),
                       glm::vec2(0.999455, 0.750380)};

   for (int i=0; i<blocks.size(); i++) {
      for (int j=0; j<6; j++) {
         floorVerts[6*i+j].x = (blocks[i][0] + xShift[j]) * rScale;
         floorVerts[6*i+j].y = 0.0;
         floorVerts[6*i+j].z = (blocks[i][1] + yShift[j]) * rScale;
         floorUV[6*i+j] = uvs[j];
      }

      if ((i % 15) == 0)
         lightPosition.push_back(glm::vec3(blocks[i][0]*rScale, 4.5, blocks[i][1]*rScale));
   }

   roomVerts.insert(roomVerts.end(), floorVerts.begin(), floorVerts.end());
   roomUVs.insert(roomUVs.end(), floorUV.begin(), floorUV.end());
   roomNorms.insert(roomNorms.end(), floorNorm.begin(), floorNorm.end());

   for (int i=0; i<blocks.size(); i++) {
      for (int j=0; j<6; j++) {
         roofVerts[6*i+j].x = (blocks[i][0] + xShift[5-j]) * rScale;
         roofVerts[6*i+j].y = 5.0;
         roofVerts[6*i+j].z = (blocks[i][1] + yShift[5-j]) * rScale;
         roofUV[6*i+j] = uvs[j];
      }
   }

   roomVerts.insert(roomVerts.end(), roofVerts.begin(), roofVerts.end());
   roomUVs.insert(roomUVs.end(), roofUV.begin(), roofUV.end());
   roomNorms.insert(roomNorms.end(), roofNorm.begin(), roofNorm.end());
}














void room::buildWalls(std::vector< std::vector<double> > boundary, std::vector<glm::vec3> normals) {

   int xyInd[6] = {0, 2, 0, 2, 0, 2};
   double zVal[6] = {0.0, 0.0, 5.0, 5.0, 5.0, 0.0};
   glm::vec2 uvs[6] = {glm::vec2(0.748573, 0.501077),
                       glm::vec2(0.748573, 0.750412),
                       glm::vec2(0.999110, 0.501077),
                       glm::vec2(0.999110, 0.501077),
                       glm::vec2(0.748573, 0.750412),
                       glm::vec2(0.999455, 0.750380)};

   roomVerts.resize(6*boundary.size(), glm::vec3(0)); roomNorms.resize(6*boundary.size(), glm::vec3(0));
   roomUVs.resize(6*boundary.size(), glm::vec2(0));
   for (int i=0; i<boundary.size(); i++) {
      for (int j=0; j<6; j++) {
         roomVerts[6*i+j].x = boundary[i][xyInd[j]]*rScale;
         roomVerts[6*i+j].y = zVal[j];
         roomVerts[6*i+j].z = boundary[i][xyInd[j]+1]*rScale;

         roomNorms[6*i+j] = normals[i];
         roomUVs[6*i+j] = uvs[j];
      }
   }
}









void room::createBound(std::vector< std::vector<int> > blocks, std::vector< std::vector<double> >& bEdge,
                       std::vector<glm::vec3>& norm) {

   int count = 0;
   for (int i=0; i<blocks.size(); i++) {
      bool xm=true, xp=true, ym=true, yp=true;
      for (int j=0; j<blocks.size(); j++) {
         if ((blocks[i][0] + 1 == blocks[j][0]) && (blocks[i][1] == blocks[j][1]))
            xp = false;
         else if ((blocks[i][0] - 1 == blocks[j][0]) && (blocks[i][1] == blocks[j][1]))
            xm = false;
         else if ((blocks[i][0] == blocks[j][0]) && (blocks[i][1] + 1 == blocks[j][1]))
            yp = false;
         else if ((blocks[i][0] == blocks[j][0]) && (blocks[i][1] - 1 == blocks[j][1]))
            ym = false;
      }

      //Can use when three are true to place destination markers (will need to make some way of forcing possible solution)
      if (xp) {
         bEdge.push_back(std::vector<double>(4,0));
         bEdge[count][0] = blocks[i][0] + 0.5; bEdge[count][2] = blocks[i][0] + 0.5;
         bEdge[count][1] = blocks[i][1] - 0.5; bEdge[count][3] = blocks[i][1] + 0.5;
         count++;

         norm.push_back(glm::vec3(-1.0, 0.0, 0.0));
      }

      if (xm) {
         bEdge.push_back(std::vector<double>(4,0));
         bEdge[count][0] = blocks[i][0] - 0.5; bEdge[count][2] = blocks[i][0] - 0.5;
         bEdge[count][1] = blocks[i][1] + 0.5; bEdge[count][3] = blocks[i][1] - 0.5;
         count++;

         norm.push_back(glm::vec3(1.0, 0.0, 0.0));
      }

      if (yp) {
         bEdge.push_back(std::vector<double>(4,0));
         bEdge[count][0] = blocks[i][0] + 0.5; bEdge[count][2] = blocks[i][0] - 0.5;
         bEdge[count][1] = blocks[i][1] + 0.5; bEdge[count][3] = blocks[i][1] + 0.5;
         count++;

         norm.push_back(glm::vec3(0.0, 0.0, -1.0));
      }

      if (ym) {
         bEdge.push_back(std::vector<double>(4,0));
         bEdge[count][0] = blocks[i][0] - 0.5; bEdge[count][2] = blocks[i][0] + 0.5;
         bEdge[count][1] = blocks[i][1] - 0.5; bEdge[count][3] = blocks[i][1] - 0.5;
         count++;

         norm.push_back(glm::vec3(0.0, 0.0, 1.0));
      }
   }
}











std::vector< std::vector<int> > room::createBlocks(int numBlocks) {

   std::vector< std::vector<int> > bPos(numBlocks, std::vector<int>(2,0));
   std::vector<double> bProb(numBlocks, 0);

   srand(time(NULL));

   for (int i=1; i<numBlocks; i++) {

      bool flag = true;
      int bloc = i;
      while (flag) {
         //Need to add a weighting to reduce chance of reoccuring therefore making more corridors than rooms
         if (bloc < 1)
	    bloc = i;
         else
	    bloc--;

         int dir = rand() % 4, pmDir = 2*(dir%2) - 1;

         if (dir < 2) {
            bPos[i][0] = bPos[bloc][0] + pmDir;
            bPos[i][1] = bPos[bloc][1];
         } else {
            bPos[i][0] = bPos[bloc][0];
            bPos[i][1] = bPos[bloc][1] + pmDir;
         }

         flag = false;
         for (int j=0; j<i; j++) {
            if ((bPos[j][0] == bPos[i][0]) && (bPos[j][1] == bPos[i][1]))
               flag = true;
         }
      }
   }

   return bPos;
}
