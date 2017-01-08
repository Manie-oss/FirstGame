#ifndef ROOM_HPP
#define ROOM_HPP

#include <glm/glm.hpp>
#include <vector>
#include <time.h>

class room {
   public:
      room();
      std::vector<glm::vec3> roomVerts, roomNorms, lightPosition;
      std::vector<glm::vec2> roomUVs;

   protected:
      double rScale;

      std::vector< std::vector<int> > createBlocks(int);
      void createBound(std::vector< std::vector<int> >, std::vector< std::vector<double> >&, std::vector<glm::vec3>&);
      void buildWalls(std::vector< std::vector<double> >, std::vector<glm::vec3>);
      void buildFloorAndRoof(std::vector< std::vector<int> >);
};

#endif
