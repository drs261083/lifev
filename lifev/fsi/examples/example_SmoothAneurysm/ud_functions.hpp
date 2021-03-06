//@HEADER
/*
*******************************************************************************

    Copyright (C) 2004, 2005, 2007 EPFL, Politecnico di Milano, INRIA
    Copyright (C) 2010 EPFL, Politecnico di Milano, Emory University

    This file is part of LifeV.

    LifeV is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LifeV is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LifeV.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/
//@HEADER

/*!
 *  @file
 *  @brief File containing the boundary conditions for the Monolithic Test
 *
 *  @date 2009-04-09
 *  @author Paolo Crosetto <crosetto@iacspc70.epfl.ch>
 *
 *  @contributor Cristiano Malossi <cristiano.malossi@epfl.ch>
 *  @maintainer Paolo Crosetto <crosetto@iacspc70.epfl.ch>
 *
 *  Contains the functions to be assigned as boundary conditions, in the file boundaryConditions.hpp . The functions
 *  can depend on time and space, while they can take in input an ID specifying one of the three principal axis
 *  if the functions to assign is vectorial and the boundary condition is of type \c Full \c.
 */

#ifndef UDF_HPP
#define UDF_HPP

// LifeV includes
#include <lifev/core/LifeV.hpp>

namespace LifeV
{

class aortaVelIn
{
public:
    static Real S_timestep;
};

Real f (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);

Real u1 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);

Real fZero (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);
Real outerWallPressure (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);
Real epsilon (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);
Real pressureInitial (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);

// Initial velocity
Real u0 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);
Real p0 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);
Real E  (const Real& t, const Real& /*x*/, const Real& /*y*/, const Real& /*z*/, const ID& /*i*/);
Real hydro (const Real& t, const Real& /*x*/, const Real& /*y*/, const Real& /*z*/, const ID& i);
Real u2 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);


// Initial displacement and velocity
Real d0 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);

Real w0 (const Real& t, const Real& x, const Real& y, const Real& z, const ID& i);

Real fluxFunctionAneurysm (const Real& t, const Real& /*x*/, const Real& /*y*/, const Real& /*z*/, const ID& i);

Real aneurismFluxInVectorial (const Real&  t, const Real& x, const Real& y, const Real& z, const ID& i);

Real squareSinusoidalFluxFunction (const Real& t, const Real& /*x*/, const Real& /*y*/, const Real& /*z*/, const ID& /*i*/);

//Fiber Directions
Real Family1 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);
Real Family2 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);
Real Family3 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);
Real Family4 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);
Real Family5 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);
Real Family6 ( const Real& /*t*/, const Real& x, const Real& y, const Real& z, const ID& i);

class fibersDirectionList
{
public:

    typedef boost::function<Real (  Real const&, Real const&, Real const&, Real const&, ID const& ) > fiberFunction_Type;
    typedef boost::shared_ptr<fiberFunction_Type>                                       fiberFunctionPtr_Type;
    typedef std::map< std::string, fiberFunctionPtr_Type>                               mapNameDefinitionFiberFunction_Type;

    fibersDirectionList();

    ~fibersDirectionList();

    fiberFunctionPtr_Type fiberDefinition( const std::string nameFamily );
    void setupFiberDefinitions( const UInt nbFamilies );

private:
    mapNameDefinitionFiberFunction_Type     M_mapNameDefinition;
};

}



#endif
