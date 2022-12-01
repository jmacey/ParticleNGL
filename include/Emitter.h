#ifndef EMITTER_H_
#define EMITTER_H_

#include <cstddef>
#include <vector>
#include <string_view>
#include "Particle.h"
class Emitter
{
public:
  Emitter(size_t _numParticles);
  size_t getNumParticles() const;
  void update();
  void render() const;
  void writeToGeo(std::string_view _fname) const;

private:
  std::vector<Particle> m_particles;
  GLuint m_vao;
  GLuint m_buffer;
};

#endif