/*
 * =====================================================================================
 *
 *       Filename:  pymodule.cpp
 *
 *    Description:  Python API for the Escher C++ library
 *
 *        Version:  1.0
 *        Created:  07/05/16 18:00:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel (dmc), danielmail7@gmail.com
 *   Organization:  University of Oviedo
 *
 * =====================================================================================
 */

//#include <string>

#include "include/escher.hpp"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(libescherpy)
{
    //def("Convert", Convert);

    //class_<Escher>("Escher", init<std::string>())
    class_<Escher,boost::noncopyable>("Escher")
        .def("ReadFile", & Escher::ReadFile)
        .def("ReadBasin", & Escher::ReadBasin)
        .def("ReadSurf", & Escher::ReadSurf)
        .def("Show", & Escher::Show)
    ;
}
