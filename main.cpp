#include <iostream>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"

#include "SNN.hpp"

double corr_fr(double x, double y);
double uncorr_fr(double x);

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double dt_max = 0.0005;

  // initialise simulation state
  SNN snn;

  // initialise event queue
  EventManager EM;

  // set up recording
  /*
  const double rec_period = 0.5;
  const int rec_entries = int(duration/rec_period) + 1;
  double w1_record[rec_entries];
  double w2_record[rec_entries];
  double t_record[rec_entries];
  for (int i = 0; i < rec_entries; ++i)
  {
    t_record[i] = i*rec_period;
  }
  EM.insert(new RecordEvent(t_sim, 0));
  */

  // main loop
  while (EM.t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (EM.t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-EM.t_sim)) ? dt_max : (e->time-EM.t_sim);
      EM.t_sim += dt;

      for (Neuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(EM.t_sim, neuron));
          synch_event_inserted = true;
        }
      }
    }

    // event (asynchronous) update
    if (!synch_event_inserted)
    {
      e->process(EM, snn);
      EM.del_min();
    }
  }

  return 0;
}

#define FRF 0.3
double corr_fr(double x, double y)
{
  double fr = 10.0*(1.0 + FRF*x + FRF*y);
  return fr < 0.0 ? 0.0 : fr;
}

double uncorr_fr(double x)
{
  double fr = 10.0*(1.0 + FRF*sqrt(2.0)*x);
  return fr < 0.0 ? 0.0 : fr;
}
