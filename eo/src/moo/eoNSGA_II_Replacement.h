#ifndef eoNSGA_II_Replacement_h
#define eoNSGA_II_Replacement_h

#include <moo/eoFrontSorter.h>
#include <eoReplacement.h>

/** @brief Fast Elitist Non-Dominant Sorting Genetic Algorithm

  Adapted from Deb, Agrawal, Pratab and Meyarivan: A Fast Elitist
  Non-Dominant Sorting Genetic Algorithm for MultiObjective
  Optimization: NSGA-II KanGAL Report No. 200001

  Note that this class does not do the sorting per se, but the sorting
  of it worth_std::vector will give the right order

*/
template <class EOT>
class eoNSGA_II_Replacement : public eoReplacement<EOT>
{
  public:
  
  void operator()(eoPop<EOT>& parents, eoPop<EOT>& offspring) {
        unsigned origSize = parents.size();

        std::copy(offspring.begin(), offspring.end(), std::back_inserter(parents));

        typename eoFrontSorter<EOT>::front_t front = sorter(parents);

        for (unsigned i = 0; i < front.size(); ++i) {
            assign_worths(front[i], front.size() - i, parents); 
        }
        
        // sort on worth (assuming eoMOFitness) 
        std::sort(parents.begin(), parents.end());
        
        // truncate
        parents.resize(origSize);
  }


  eoFrontSorter<EOT> sorter;
  
  private:
  typedef std::pair<double, unsigned> double_index_pair;

  class compare_nodes
  {
  public :
    bool operator()(const double_index_pair& a, const double_index_pair& b) const
    {
      return a.first < b.first;
    }
  };

  /// _cf points into the elements that consist of the current front
  void assign_worths(const std::vector<detail::FitnessInfo>& front, unsigned rank, eoPop<EOT>& parents) {
  
    typedef typename EOT::Fitness::fitness_traits traits;
    unsigned i;

    unsigned nObjectives = traits::nObjectives(); //_pop[_cf[0]].fitness().size();
    
    std::vector<double> niche_distance(front.size());

    for (unsigned o = 0; o < nObjectives; ++o)
    {

      std::vector<std::pair<double, unsigned> > performance(front.size());
      for (i =0; i < front.size(); ++i)
      {
        performance[i].first = front[i].fitness[o];
        performance[i].second = i;
      }

      std::sort(performance.begin(), performance.end(), compare_nodes()); // a lambda operator would've been nice here

      std::vector<double> nc(front.size(), 0.0);

      for (i = 1; i < front.size()-1; ++i)
      { // and yet another level of indirection
        nc[performance[i].second] = performance[i+1].first - performance[i-1].first;
      }

      // set boundary at max_dist + 1 (so it will get chosen over all the others
      //nc[performance[0].second]     += 0;
      nc[performance.back().second] += std::numeric_limits<double>::infinity(); // best on objective
      
      for (i = 0; i < nc.size(); ++i)
      {
        niche_distance[i] += nc[i];
      }
    }
    
    // now we've got niche_distances, scale them between (0, 1), making sure that infinities get maximum rank
    
    double max = 0;
    for (unsigned i = 0; i < niche_distance[i]; ++i) {
        if (niche_distance[i] != std::numeric_limits<double>::infinity()) {
            max = std::max(max, niche_distance[i]);
        }
    }
    
    for (unsigned i = 0; i < front.size(); ++i) {
        double dist = niche_distance[i];
        if (dist == std::numeric_limits<double>::infinity()) {
            dist = 1.0;
        } else {
            dist /= (1+max);
        }
        
        unsigned idx = front[i].index;
        
        typename EOT::Fitness f = parents[idx].fitness();
        f.setWorth(rank + dist);
        //std::cout << "Base rank " << rank << " dist " << dist << " result " << (rank+dist) << std::endl;
         
        parents[idx].fitness(f);
    }

  }
};

#endif
