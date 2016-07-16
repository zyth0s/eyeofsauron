/*
 * =====================================================================================
 *
 *       Filename:  pymodule.cpp
 *
 *    Description:  Python API for the EyeOfSauron C++ library
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

// EyeOfSauron
#include "include/eyeos.hpp"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>
using namespace boost::python;


BOOST_PYTHON_MODULE(libeyeos)
{
    //def("Convert", Convert);

    //class_<EyeOfSauron>("EyeOfSauron", init<std::string>())
    class_<EyeOfSauron,boost::noncopyable>("EyeOfSauron")
        .def("ReadFile", & EyeOfSauron::ReadFile)
        .def("ReadBasin", & EyeOfSauron::ReadBasin)
        .def("ReadSurf", & EyeOfSauron::ReadSurf)
        .def("Show", & EyeOfSauron::Show)
    ;
}
