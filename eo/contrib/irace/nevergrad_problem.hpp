
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include "ioh.hpp"
namespace py = pybind11;
py::scoped_interpreter python;
using namespace ioh::common;
using namespace ioh::problem;

double nevergrad_onemax(const std::vector<int> &x) { 
    py::object p = py::module_::import("nevergrad.functions").attr("corefuncs").attr("DiscreteFunction")("onemax",2);
    return p(py::cast(x)).cast<double>();
}

/*****************************************************************************
 * IOH + Nevergrad problem adaptation.
 *****************************************************************************/

auto prepare_problem() {
    ioh::problem::wrap_function<int>(&nevergrad_onemax,  // the new function
                                      "onemax", // name of the new function
                                      ioh::common::OptimizationType::Maximization, // optimization type
                                      0,  // lowerbound  
                                      1  // upperbound
                                    );
    auto &factory = ioh::problem::ProblemRegistry<ioh::problem::Integer>::instance();
    return &factory;
}