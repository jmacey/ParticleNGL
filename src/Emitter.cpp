#include "Emitter.h"
#include <ngl/Random.h>
#include <iostream>
#include <fstream>
#include <sstream>

ngl::Vec3 randomVectorOnSphere(float _radius = 1.0f)
{
  float phi = ngl::Random::randomPositiveNumber(static_cast<float>(M_PI * 2.0f));
  float costheta = ngl::Random::randomNumber();
  float u = ngl::Random::randomPositiveNumber();
  float theta = acos(costheta);
  float r = _radius * std::cbrt(u);
  return ngl::Vec3(r * sin(theta) * cos(phi),
                   r * sin(theta) * sin(phi),
                   r * cos(theta));
}

Emitter::Emitter(size_t _numParticles)
{
  m_particles.resize(_numParticles);
  ngl::Vec3 emitDir(0, 10.0f, 0);
  for (auto &p : m_particles)
  {
    p.direction = emitDir * ngl::Random::randomPositiveNumber() + randomVectorOnSphere() * 10;
    p.direction.m_y = std::abs(p.direction.m_y);
    p.colour = ngl::Random::getRandomColour3();
  }
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_buffer);
}

size_t Emitter::getNumParticles() const
{
  return m_particles.size();
}

void Emitter::update()
{
  float _dt = 0.01f;
  ngl::Vec3 gravity(0, -9.81f, 0);
  for (auto &p : m_particles)
  {
    p.direction += gravity * _dt * 0.5f;
    p.position += p.direction * _dt;
    p.scale += 0.001f;
    if (p.position.m_y <= 0.0f)
    {
      p.position.set(0, 0, 0);
      p.direction = ngl::Vec3{0, 10, 0} * ngl::Random::randomPositiveNumber() + randomVectorOnSphere() * 10;
      p.direction.m_y = std::abs(p.direction.m_y);
      p.colour = ngl::Random::getRandomColour3();
    }
  }
}

void Emitter::writeToGeo(std::string_view _fname) const
{
  /*  std::cout << "saving " << _fname << '\n';
    std::ofstream file(_fname.data());
    std::stringstream geo;
    geo << "PGEOMETRY V5\n";
    geo << "NPoints " << m_particles.size() << " NPrims 1\n";
    geo << "NPointGroups 0 NPrimGroups 0\n";
    geo << "NPointAttrib 2  NVertexAttrib 0 NPrimAttrib 1 NAttrib 0\n";
    geo << "PointAttrib\n";
    geo << "Cd 3 float 1 1 1 \n";
    geo << "pscale 1 float 1 \n";
    for (auto p : m_particles)
    {
      geo << p.position.x << ' ' << p.position.y << ' ' << p.position.z << " 1 (";
      geo << p.colour.x << ' ' << p.colour.y << ' ' << p.colour.z << ' ';
      geo << p.scale << ")\n";
    }
    geo << "PrimitiveAttrib\n";
    geo << "generator 1 index 1 papi\n";
    geo << "Part " << m_particles.size() << ' ';
    for (int i = 0; i < m_particles.size(); ++i)
    {
      geo << i << ' ';
    }
    geo << "[0]\n";
    geo << "beginExtra\n";
    geo << "endExtra\n";
    if (file.is_open())
    {
      file << geo.rdbuf();
    }
    file.close();
    */
}

void Emitter::render() const
{
  glPointSize(4);
  glBindVertexArray(m_vao);
  glBufferData(GL_ARRAY_BUFFER, m_particles.size() * sizeof(Particle), &m_particles[0].position.m_x, GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle),reinterpret_cast<float *>(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);



  glDrawArrays(GL_POINTS, 0, m_particles.size());
  glBindVertexArray(0);

  /*
  for (auto p : m_particles)
  {
    // std::cout<<"******************************************\n";
    std::cout << p.position.x << ',' << p.position.y << ',' << p.position.z << '\n';
    // std::cout<<p.direction.x<<' '<<p.direction.y<<' '<<p.direction.z<<'\n';
    // std::cout<<p.colour.x<<' '<<p.colour.y<<' '<<p.colour.z<<'\n';
  }
*/
}
