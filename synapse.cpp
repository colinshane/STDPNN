#include "synapse.hpp"
#include <iostream>

Synapse::Synapse(Neuron *pre, Neuron *post, double w) : pre(pre), post(post), w(w)
{
}

double Synapse::get_w()
{
  return w;
}

SynapseNetwork::SynapseNetwork(int num_neurons) :
  A_post(num_neurons, std::vector<Synapse *>()),
  A_pre(num_neurons, std::vector<Synapse *>()),
  num_neurons(num_neurons)
{
}

SynapseNetwork::~SynapseNetwork()
{
  for (std::vector<Synapse *> &node : A_post)
  {
    for (Synapse *&sy : node)
    {
      delete sy;
    }
  }
}

void SynapseNetwork::add_edge(Neuron *pre, Neuron *post, double w)
{
  Synapse *sy = new Synapse(pre, post, w);
  A_post[pre->id].push_back(sy);
  A_pre[post->id].push_back(sy);
}

std::vector<Synapse *> & SynapseNetwork::inputs(Neuron *post)
{
  return A_pre[post->id];
}

std::vector<Synapse *> & SynapseNetwork::outputs(Neuron *pre)
{
  return A_post[pre->id];
}