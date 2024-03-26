//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This product includes software developed by Members of the Geant4 Collaboration (http://cern.ch/geant4).
//
// --------------------------------------------------------------------
// GEANT 4 class header file
//
// 
// G4MultiUnion_v1051
//
// Class description:
//
//   An instance of "G4MultiUnion_v1051" constitutes a grouping of several solids.
//   The constituent solids are stored with their respective location in an
//   instance of "G4Node". An instance of "G4MultiUnion_v1051" is subsequently
//   composed of one or several nodes.

// History:
// 06.04.17 G.Cosmo - Imported implementation in Geant4 for VecGeom migration
// 19.10.12 M.Gayer - Original implementation from USolids module
// --------------------------------------------------------------------
#ifndef G4MULTIUNION_V1051_HH
#define G4MULTIUNION_V1051_HH

#include <vector>

#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4Point3D.hh"
#include "G4Vector3D.hh"
#include "G4SurfBits.hh"
#include "G4Voxelizer_v1051.hh"

class G4Polyhedron;

class G4MultiUnion_v1051 : public G4VSolid
{
    friend class G4Voxelizer_v1051;

  public:

    G4MultiUnion_v1051() : G4VSolid("") {}
    G4MultiUnion_v1051(const G4String& name);
    ~G4MultiUnion_v1051();

    // Build the multiple union by adding nodes
    void AddNode(G4VSolid& solid, G4Transform3D& trans);

    G4MultiUnion_v1051(const G4MultiUnion_v1051& rhs);
    G4MultiUnion_v1051& operator=(const G4MultiUnion_v1051& rhs);

    // Accessors
    inline const G4Transform3D& GetTransformation(G4int index) const;
    inline G4VSolid* GetSolid(G4int index) const;
    inline G4int GetNumberOfSolids()const;

    // Navigation methods
    EInside Inside(const G4ThreeVector& aPoint) const;

    EInside InsideIterator(const G4ThreeVector& aPoint) const;

    // Safety methods
    G4double DistanceToIn(const G4ThreeVector& aPoint) const;
    G4double DistanceToOut(const G4ThreeVector& aPoint) const;
    inline void SetAccurateSafety(G4bool flag);

    // Exact distance methods
    G4double DistanceToIn(const G4ThreeVector& aPoint,
                          const G4ThreeVector& aDirection) const;
    G4double DistanceToOut(const G4ThreeVector& aPoint,
                           const G4ThreeVector& aDirection,
                           const G4bool calcNorm=false,
                           G4bool *validNorm=0,
                           G4ThreeVector *aNormalVector=0) const;

    G4double DistanceToInNoVoxels(const G4ThreeVector& aPoint,
                                  const G4ThreeVector& aDirection) const;
    G4double DistanceToOutVoxels(const G4ThreeVector& aPoint,
                                 const G4ThreeVector& aDirection,
                                 G4ThreeVector*       aNormalVector) const;
    G4double DistanceToOutVoxelsCore(const G4ThreeVector& aPoint,
                                     const G4ThreeVector& aDirection,
                                     G4ThreeVector*       aNormalVector,
                                     G4bool&           aConvex,
                                     std::vector<G4int>& candidates) const;
    G4double DistanceToOutNoVoxels(const G4ThreeVector& aPoint,
                                   const G4ThreeVector& aDirection,
                                   G4ThreeVector*       aNormalVector) const;

    G4ThreeVector SurfaceNormal(const G4ThreeVector& aPoint) const;

    void Extent(EAxis aAxis, G4double& aMin, G4double& aMax) const;
    void BoundingLimits(G4ThreeVector& aMin, G4ThreeVector& aMax) const;
    G4bool CalculateExtent(const EAxis pAxis,
                           const G4VoxelLimits& pVoxelLimit,
                           const G4AffineTransform& pTransform,
                           G4double& pMin, G4double& pMax) const;
    G4double GetCubicVolume();
    G4double GetSurfaceArea();

    G4VSolid* Clone() const ;

    G4GeometryType GetEntityType() const { return "G4MultiUnion_v1051"; }

    void Voxelize();
      // Finalize and prepare for use. User MUST call it once before
      // navigation use.

    EInside InsideNoVoxels(const G4ThreeVector& aPoint) const;
    inline G4Voxelizer_v1051& GetVoxels() const;

    std::ostream& StreamInfo(std::ostream& os) const;

    G4ThreeVector GetPointOnSurface() const;

    void DescribeYourselfTo ( G4VGraphicsScene& scene ) const ;
    G4Polyhedron* CreatePolyhedron () const ;
    G4Polyhedron* GetPolyhedron () const;

    G4MultiUnion_v1051(__void__&);
      // Fake default constructor for usage restricted to direct object
      // persistency for clients requiring preallocation of memory for
      // persistifiable objects.

  private:

    EInside InsideWithExclusion(const G4ThreeVector& aPoint,
                                G4SurfBits* bits = 0) const;
    G4int SafetyFromOutsideNumberNode(const G4ThreeVector& aPoint,
                                      G4double& safety) const;
    G4double DistanceToInCandidates(const G4ThreeVector& aPoint,
                                    const G4ThreeVector& aDirection,
                                     std::vector<G4int>& candidates,
                                    G4SurfBits& bits) const;

    // Conversion utilities
    inline G4ThreeVector GetLocalPoint(const G4Transform3D& trans,
                                       const G4ThreeVector& gpoint) const;
    inline G4ThreeVector GetLocalVector(const G4Transform3D& trans,
                                       const G4ThreeVector& gvec) const;
    inline G4ThreeVector GetGlobalPoint(const G4Transform3D& trans,
                                       const G4ThreeVector& lpoint) const;
    inline G4ThreeVector GetGlobalVector(const G4Transform3D& trans,
                                       const G4ThreeVector& lvec) const;
    void TransformLimits(G4ThreeVector& min, G4ThreeVector& max,
                         const G4Transform3D& transformation) const;
  private:

    struct G4MultiUnionSurface
    {
      G4ThreeVector point;
      G4VSolid* solid;
    };

    std::vector<G4VSolid*> fSolids;
    std::vector<G4Transform3D> fTransformObjs;
    G4Voxelizer_v1051 fVoxels;           // Pointer to the vozelized solid
    G4double       fCubicVolume;   // Cubic Volume
    G4double       fSurfaceArea;   // Surface Area
    G4double       kRadTolerance;  // Cached radial tolerance
    mutable G4bool fAccurate;      // Accurate safety (off by default)

    mutable G4bool fRebuildPolyhedron;
    mutable G4Polyhedron* fpPolyhedron;
};

//______________________________________________________________________________
inline G4Voxelizer_v1051& G4MultiUnion_v1051::GetVoxels() const
{
  return (G4Voxelizer_v1051&)fVoxels;
}

//______________________________________________________________________________
inline const G4Transform3D& G4MultiUnion_v1051::GetTransformation(G4int index) const
{
  return fTransformObjs[index];
}

//______________________________________________________________________________
inline G4VSolid* G4MultiUnion_v1051::GetSolid(G4int index) const
{
  return fSolids[index];
}

//______________________________________________________________________________
inline G4int G4MultiUnion_v1051::GetNumberOfSolids() const
{
  return fSolids.size();
}

//______________________________________________________________________________
inline void G4MultiUnion_v1051::SetAccurateSafety(G4bool flag)
{
  fAccurate = flag;
}

//______________________________________________________________________________
inline
G4ThreeVector G4MultiUnion_v1051::GetLocalPoint(const G4Transform3D& trans,
                                          const G4ThreeVector& global) const
{
  // Returns local point coordinates converted from the global frame defined
  // by the transformation. This is defined by multiplying the inverse
  // transformation with the global vector.

  return trans.inverse()*G4Point3D(global);
}

//______________________________________________________________________________
inline
G4ThreeVector G4MultiUnion_v1051::GetLocalVector(const G4Transform3D& trans,
                                           const G4ThreeVector& global) const
{
  // Returns local point coordinates converted from the global frame defined
  // by the transformation. This is defined by multiplying the inverse
  // transformation with the global vector.

  G4Rotate3D rot;
  G4Translate3D transl ;
  G4Scale3D scale;

  trans.getDecomposition(scale,rot,transl);
  return rot.inverse()*G4Vector3D(global);
}

//______________________________________________________________________________
inline
G4ThreeVector G4MultiUnion_v1051::GetGlobalPoint(const G4Transform3D& trans,
                                           const G4ThreeVector& local) const
{
  // Returns global point coordinates converted from the local frame defined
  // by the transformation. This is defined by multiplying this transformation
  // with the local vector.

  return trans*G4Point3D(local);
}

//______________________________________________________________________________
inline
G4ThreeVector G4MultiUnion_v1051::GetGlobalVector(const G4Transform3D& trans,
                                            const G4ThreeVector& local) const
{
  // Returns vector components converted from the local frame defined by the
  // transformation to the global one. This is defined by multiplying this
  // transformation with the local vector while ignoring the translation.

  G4Rotate3D rot;
  G4Translate3D transl ;
  G4Scale3D scale;

  trans.getDecomposition(scale,rot,transl);
  return rot*G4Vector3D(local);
}

#endif
