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
 *  @brief This file contains the definition of the St. Venant Kirchhoff material.
 *
 *  @version 1.0
 *  @date 01-01-2010
 *  @author Paolo Tricerri
 *
 *  @maintainer  Paolo Tricerri <paolo.tricerri@epfl.ch>
 */

#ifndef _NONLINVENANTKIRCHHOFFMATERIAL_H_
#define _NONLINVENANTKIRCHHOFFMATERIAL_H_

#include <lifev/structure/solver/StructuralConstitutiveLaw.hpp>

namespace LifeV
{
template <typename Mesh>
class VenantKirchhoffMaterialNonLinear :
        public StructuralConstitutiveLaw<Mesh>
{
    //!@name Type definitions
    //@{

public:
    typedef StructuralConstitutiveLaw<Mesh>           super;

    typedef typename super::data_Type                data_Type;

    typedef typename super::vector_Type              vector_Type;
    typedef typename super::matrix_Type              matrix_Type;

    typedef typename super::matrixPtr_Type           matrixPtr_Type;
    typedef typename super::vectorPtr_Type           vectorPtr_Type;
    typedef typename super::dataPtr_Type             dataPtr_Type;
    typedef typename super::displayerPtr_Type        displayerPtr_Type;

    typedef typename super::mapMarkerVolumesPtr_Type mapMarkerVolumesPtr_Type;
    typedef typename super::mapMarkerVolumes_Type mapMarkerVolumes_Type;
    typedef typename mapMarkerVolumes_Type::const_iterator mapIterator_Type;

    typedef typename super::vectorVolumes_Type       vectorVolumes_Type;
    typedef boost::shared_ptr<vectorVolumes_Type>    vectorVolumesPtr_Type;

    typedef typename super::FESpacePtr_Type          FESpacePtr_Type;
    typedef typename super::ETFESpacePtr_Type        ETFESpacePtr_Type;

    typedef MatrixSmall<3,3>                          matrixSmall_Type;
    //@}



    //! @name Constructor &  Destructor
    //@{

    VenantKirchhoffMaterialNonLinear();

    virtual  ~VenantKirchhoffMaterialNonLinear();

    //@}



    //!@name Methods
    //@{

    //! Setup the created object of the class StructuralConstitutiveLaw
    /*!
      \param dFespace: the FiniteElement Space
      \param monolithicMap: the MapEpetra
      \param offset: the offset parameter used assembling the matrices
    */
    void setup( const FESpacePtr_Type& dFESpace,
                const ETFESpacePtr_Type& dETFESpace,
                const boost::shared_ptr<const MapEpetra>&  monolithicMap,
                const UInt offset, const dataPtr_Type& dataMaterial, const displayerPtr_Type& displayer );


    //! Compute the Stiffness matrix in StructuralSolver::buildSystem()
    /*!
      \param dataMaterial the class with Material properties data
    */
    void computeLinearStiff( dataPtr_Type& /*dataMaterial*/, const mapMarkerVolumesPtr_Type /*mapsMarkerVolumes*/ );


    //! Updates the Jacobian matrix in StructualSolver::updateJacobian
    /*!
      \param disp: solution at the k-th iteration of NonLinearRichardson Method
      \param dataMaterial: a pointer to the dataType member in StructuralSolver class to get
                           the material coefficients (e.g. Young modulus, Poisson ratio..)
      \param displayer: a pointer to the Dysplaier member in the StructuralSolver class
    */
    void updateJacobianMatrix( const vector_Type& disp,
                               const dataPtr_Type& dataMaterial,
                               const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
                               const displayerPtr_Type& displayer );


    //! Updates the nonlinear terms in the Jacobian matrix in StructualSolver::updateJacobian
    /*!
      \param stiff: stiffness matrix provided from outside
      \param disp: solution at the k-th iteration of NonLinearRichardson Method
      \param dataMaterial: a pointer to the dataType member in StructuralSolver class to get
                           the material coefficients (e.g. Young modulus, Poisson ratio..)
      \param displayer: a pointer to the Dysplaier member in the StructuralSolver class
    */
    void updateNonLinearJacobianTerms( matrixPtr_Type& jacobian,
                                       const vector_Type& disp,
                                       const dataPtr_Type& dataMaterial,
                                       const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
                                       const displayerPtr_Type& displayer );


    //! Interface method to compute the new Stiffness matrix in StructuralSolver::evalResidual and in
    //! StructuralSolver::updateSystem since the matrix is the expression of the matrix is the same.
    /*!
      \param sol:  the solution vector
      \param factor: scaling factor used in FSI
      \param dataMaterial: a pointer to the dataType member in StructuralSolver class to get the
                           material coefficients (e.g. Young modulus, Poisson ratio..)
      \param displayer: a pointer to the Dysplaier member in the StructuralSolver class
    */
    void computeStiffness( const vector_Type& disp, Real factor, const dataPtr_Type& dataMaterial, const mapMarkerVolumesPtr_Type mapsMarkerVolumes, const displayerPtr_Type& displayer );


    //! Computes the new Stiffness vector for Neo-Hookean and Exponential materials in StructuralSolver
    //! given a certain displacement field.
    //! This function is used both in StructuralSolver::evalResidual and in StructuralSolver::updateSystem
    //! since the matrix is the expression of the matrix is the same.
    /*!
      \param sol:  the solution vector
      \param factor: scaling factor used in FSI
      \param dataMaterial: a pointer to the dataType member in StructuralSolver class to get
                           the material coefficients (e.g. Young modulus, Poisson ratio..)
      \param displayer: a pointer to the Dysplaier member in the StructuralSolver class
    */
    // void computeVector( const vector_Type& sol,
    //                     Real factor,
    //                     const dataPtr_Type& dataMaterial,
    //                     const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
    //                     const displayerPtr_Type& displayer );

    //! Computes the deformation gradient F, the cofactor matrix Cof(F),
    //! the determinant of F (J = det(F)), the trace of right Cauchy-Green tensor tr(C)
    //! This function is used in StructuralConstitutiveLaw::computeStiffness
    /*!
      \param dk_loc: the elemental displacement
    */
    void computeKinematicsVariables( const VectorElemental& /*dk_loc*/ )
    {
    }

    //! ShowMe method of the class (saved on a file the stiffness vector and the jacobian)
    void showMe( std::string const& fileNameVectStiff,
                 std::string const& fileNameJacobain );

    //@}

    //! @name Get Methods
    //@{

    //! Get the Stiffness matrix
    matrixPtr_Type  const stiffMatrix() const { return super::M_jacobian; }


    //! Get the stiffness vector
    vectorPtr_Type  const stiffVector() const  {return M_stiff; }

    void apply( const vector_Type& sol, vector_Type& res,
                const mapMarkerVolumesPtr_Type mapsMarkerVolumes) ;

    //! Compute the First Piola Kirchhoff Tensor
    /*!
       \param firstPiola Epetra_SerialDenseMatrix that has to be filled
       \param tensorF Epetra_SerialDenseMatrix the deformation gradient
       \param cofactorF Epetra_SerialDenseMatrix cofactor of F
       \param invariants std::vector with the invariants of C and the detF
       \param material UInt number to get the material parameteres form the VenantElasticData class
    */
    void computeLocalFirstPiolaKirchhoffTensor( Epetra_SerialDenseMatrix& firstPiola,
						const Epetra_SerialDenseMatrix& tensorF,
						const Epetra_SerialDenseMatrix& cofactorF,
						const std::vector<Real>& invariants,
						const UInt marker);


  //@}



protected:

    //! Vector: stiffness non-linear
    vectorPtr_Type                         M_stiff;

    //Create the indentity for F
    matrixSmall_Type                      M_identity;

};





template <typename Mesh>
VenantKirchhoffMaterialNonLinear<Mesh>::VenantKirchhoffMaterialNonLinear():
    super            ( ),
    M_stiff          ( ),
    M_identity       ( )
{
}





template <typename Mesh>
VenantKirchhoffMaterialNonLinear<Mesh>::~VenantKirchhoffMaterialNonLinear()
{}





template <typename Mesh>
void
VenantKirchhoffMaterialNonLinear<Mesh>::setup( const FESpacePtr_Type&                       dFESpace,
                                           const ETFESpacePtr_Type&                     dETFESpace,
                                           const boost::shared_ptr<const MapEpetra>&   monolithicMap,
                                           const UInt                                  offset,
                                           const dataPtr_Type&                         dataMaterial,
                                           const displayerPtr_Type&                    displayer  )
{
    this->M_displayer = displayer;
    this->M_dataMaterial  = dataMaterial;
    //    std::cout<<"I am setting up the Material"<<std::endl;

    this->M_dispFESpace                 = dFESpace;
    this->M_dispETFESpace               = dETFESpace;
    this->M_localMap                    = monolithicMap;
    this->M_offset                      = offset;

    M_stiff.reset                     (new vector_Type(*this->M_localMap));

    M_identity(0,0) = 1.0; M_identity(0,1) = 0.0; M_identity(0,2) = 0.0;
    M_identity(1,0) = 0.0; M_identity(1,1) = 1.0; M_identity(1,2) = 0.0;
    M_identity(2,0) = 0.0; M_identity(2,1) = 0.0; M_identity(2,2) = 1.0;
}

template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::computeLinearStiff(dataPtr_Type& /*dataMaterial*/,
                                                            const mapMarkerVolumesPtr_Type /*mapsMarkerVolumes*/)
{
    //! Empty method for exponential material
}





template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::updateJacobianMatrix( const vector_Type&       disp,
                                                                   const dataPtr_Type&      dataMaterial,
                                                                   const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
                                                                   const displayerPtr_Type& displayer )
{

    this->M_jacobian.reset(new matrix_Type(*this->M_localMap));

    displayer->leaderPrint(" \n*********************************\n  ");
    updateNonLinearJacobianTerms(this->M_jacobian, disp, dataMaterial, mapsMarkerVolumes, displayer);
    displayer->leaderPrint(" \n*********************************\n  ");
    std::cout << std::endl;

}

template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::updateNonLinearJacobianTerms( matrixPtr_Type&         jacobian,
                                                                           const vector_Type&     disp,
                                                                           const dataPtr_Type&     dataMaterial,
                                                                           const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
                                                                           const displayerPtr_Type& displayer )
{

    using namespace ExpressionAssembly;

    displayer->leaderPrint("   Non-Linear S-  updating non linear terms in the Jacobian Matrix (Exponential)");

    *(jacobian) *= 0.0;

    //! Nonlinear part of jacobian
    //! loop on volumes (i)

    mapIterator_Type it;

    vectorVolumesPtr_Type pointerListOfVolumes;

    for( it = (*mapsMarkerVolumes).begin(); it != (*mapsMarkerVolumes).end(); it++ )
    {
        //Given the marker pointed by the iterator, let's extract the material parameters
        UInt marker = it->first;

        pointerListOfVolumes.reset( new vectorVolumes_Type(it->second) );

        Real lambda = dataMaterial->lambda(marker);
        Real mu = dataMaterial->mu(marker);

        //Macros to make the assembly more readable
#define F ( grad( this->M_dispETFESpace,  disp, this->M_offset) + value(this->M_identity) )
#define RIGHTCAUCHYGREEN transpose(F) * F
#define IC trace( RIGHTCAUCHYGREEN )

        // //Assembling the Isochoric Part
	    // //! 1. Stiffness matrix : int { (lambda/2.0) * ( 2* F:dF) * ( F : \nabla \v ) }
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value( lambda ) * dot( F, grad(phi_j) ) * dot( F, grad(phi_i) )
                   ) >> jacobian;

	    //! 2. Stiffness matrix : int { (lambda/2.0) * ( Ic-3.0 ) * ( dF : \nabla \v ) }
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value( lambda/2.0 ) * ( IC - value(3.0) ) * dot( grad(phi_j), grad(phi_i) )
                   ) >> jacobian;

	    // //! 3. Stiffness matrix : int { - mu * ( dF : \nabla \v )}
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value(-1.0 * mu) * dot( grad(phi_j), grad(phi_i) )
                   ) >> jacobian;

	    // //! 4. Stiffness matrix : int { mu * (dF * C) : \nabla v }
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value(mu) * dot( grad(phi_j) * RIGHTCAUCHYGREEN , grad(phi_i) )
                   ) >> jacobian;

	    // //! 5. Stiffness matrix : int { mu * ( ( F * F * transpose(dF) ) : \nabla \v ) }
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ),
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value(mu) * dot( F * F * transpose(grad(phi_j)) , grad(phi_i) )
                   ) >> jacobian;

	    // //! 6. Stiffness matrix : int { mu * ( ( F * F^T * transpose(dF) ) : \nabla \v ) }
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   this->M_dispETFESpace,
                   value(mu) *  dot( F * transpose(F) * grad(phi_j) , grad(phi_i) )
                   ) >> jacobian;

    }

    jacobian->globalAssemble();
}





template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::computeStiffness( const vector_Type& disp,
                                                           Real /*factor*/,
                                                           const dataPtr_Type& dataMaterial,
                                                           const mapMarkerVolumesPtr_Type mapsMarkerVolumes,
                                                           const displayerPtr_Type& displayer )
{

    using namespace ExpressionAssembly;

    M_stiff.reset(new vector_Type(*this->M_localMap));
    *(M_stiff) *= 0.0;

    displayer->leaderPrint(" \n*********************************\n  ");
    displayer->leaderPrint(" Non-Linear S-  Computing the Exponential nonlinear stiffness vector ");
    displayer->leaderPrint(" \n*********************************\n  ");

    mapIterator_Type it;
    vectorVolumesPtr_Type pointerListOfVolumes;

    for( it = (*mapsMarkerVolumes).begin(); it != (*mapsMarkerVolumes).end(); it++ )
    {

        //Given the marker pointed by the iterator, let's extract the material parameters
        UInt marker = it->first;

        pointerListOfVolumes.reset( new vectorVolumes_Type(it->second) );

        Real lambda = dataMaterial->lambda(marker);
        Real mu = dataMaterial->mu(marker);

        //Computation of the isochoric part
        // // ! Stiffness matrix : int { ( lambda / 2.0 ) * ( Ic - 3.0 ) * ( F : \nabla \v )}
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   value( lambda / 2.0 ) * ( IC - 3.0 ) * dot( F,grad(phi_i) )
                   ) >> M_stiff;

        //Computation of the isochoric part
        // // ! Stiffness matrix : int { ( - mu ) * ( F : \nabla \v )}
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   value(-mu) * dot( F ,grad(phi_i) )
                   ) >> M_stiff;

        //Computation of the isochoric part
        // // ! Stiffness matrix : int { ( mu ) * ( (F * C) : \nabla \v )}
        integrate( integrationOverSelectedVolumes( pointerListOfVolumes ) ,
                   this->M_dispFESpace->qr(),
                   this->M_dispETFESpace,
                   value(mu) * dot( F * RIGHTCAUCHYGREEN ,grad(phi_i) )
                   ) >> M_stiff;
      }

    this->M_stiff->globalAssemble();
}


template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::showMe( std::string const& fileNameStiff,
                                                 std::string const& fileNameJacobian )
{
    this->M_stiff->spy(fileNameStiff);
    this->M_jacobian->spy(fileNameJacobian);
}


template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::apply( const vector_Type& sol,
                                                vector_Type& res, const mapMarkerVolumesPtr_Type mapsMarkerVolumes )
{
    computeStiffness(sol, 0, this->M_dataMaterial, mapsMarkerVolumes, this->M_displayer);
    res += *M_stiff;
}

template <typename Mesh>
void VenantKirchhoffMaterialNonLinear<Mesh>::computeLocalFirstPiolaKirchhoffTensor( Epetra_SerialDenseMatrix& firstPiola,
									       const Epetra_SerialDenseMatrix& tensorF,
									       const Epetra_SerialDenseMatrix& cofactorF,
									       const std::vector<Real>& invariants,
									       const UInt marker)
{

  //Get the material parameters
  Real lambda  	= this->M_dataMaterial->lambda(marker);
  Real mu    	= this->M_dataMaterial->mu(marker);
  Real coef = ( lambda / 2.0 ) * ( invariants[0] - 3.0 );

  Epetra_SerialDenseMatrix firstTerm(tensorF);
  firstTerm.Scale(coef);

  Epetra_SerialDenseMatrix secondTerm(tensorF);
  Real coeff = -1.0 * mu;
  secondTerm.Scale( coeff );

  Epetra_SerialDenseMatrix thirdTerm(this->M_dispFESpace->fieldDim(),this->M_dispFESpace->fieldDim());
  Epetra_SerialDenseMatrix rightCauchyC(this->M_dispFESpace->fieldDim(),this->M_dispFESpace->fieldDim());
  rightCauchyC.Scale(0.0);


  //Compute the tensors C
  rightCauchyC.Multiply('T','N',1.0,tensorF,tensorF,0.0); //see Epetra_SerialDenseMatrix

  thirdTerm.Multiply('N','N',1.0,tensorF,rightCauchyC,0.0);
  thirdTerm.Scale(mu);

  firstPiola.Scale(0.0);

  firstPiola += firstTerm;
  firstPiola += secondTerm;
  firstPiola += thirdTerm;
}



template <typename Mesh>
inline StructuralConstitutiveLaw<Mesh>* createVenantKirchhoffNonLinear() { return new VenantKirchhoffMaterialNonLinear<Mesh >(); }

namespace
{
static bool registerVKNL = StructuralConstitutiveLaw<LifeV::RegionMesh<LinearTetra> >::StructureMaterialFactory::instance().registerProduct( "nonLinearVenantKirchhoff", &createVenantKirchhoffNonLinear<LifeV::RegionMesh<LinearTetra> > );
}

} //Namespace LifeV

#endif /* __NONLINVENANTKIRCHHOFF_H */