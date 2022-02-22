
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

double nevergrad_leadingones(const std::vector<int> &x) { 
    py::object p = py::module_::import("nevergrad.functions").attr("corefuncs").attr("DiscreteFunction")("leadingones",2);
    return p(py::cast(x)).cast<double>();
}

double nevergrad_jump(const std::vector<int> &x) { 
    py::object p = py::module_::import("nevergrad.functions").attr("corefuncs").attr("DiscreteFunction")("jump",2);
    return p(py::cast(x)).cast<double>();
}

double nevergrad_pbo_onemax(const std::vector<int> &x) { 
    py::object p = py::module_::import("nevergrad.functions.iohprofiler").attr("PBOFunction")(1,1,x.size());
    return p(py::cast(x)).cast<double>();
}


/*****************************************************************************
 * IOH + Nevergrad problem adaptation.
 *****************************************************************************/

auto prepare_problem() {
    ioh::problem::wrap_function<int>(&nevergrad_onemax, "onemax", ioh::common::OptimizationType::Maximization, 0, 1);
    ioh::problem::wrap_function<int>(&nevergrad_leadingones, "leadingones", ioh::common::OptimizationType::Maximization, 0, 1);
    ioh::problem::wrap_function<int>(&nevergrad_jump, "jump", ioh::common::OptimizationType::Maximization, 0, 1);
    ioh::problem::wrap_function<int>(&nevergrad_pbo_onemax, "pbo_onemax", ioh::common::OptimizationType::Maximization, 0, 1);

    auto &factory = ioh::problem::ProblemRegistry<ioh::problem::Integer>::instance();
    return &factory;
}