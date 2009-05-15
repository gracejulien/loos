/*
  This file is part of LOOS.

  LOOS (Lightweight Object-Oriented Structure library)
  Copyright (c) 2008, Tod D. Romo, Alan Grossfield
  Department of Biochemistry and Biophysics
  School of Medicine & Dentistry, University of Rochester

  This package (LOOS) is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation under version 3 of the License.

  This package is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <sfactories.hpp>
#include <sys/stat.h>


#include <boost/algorithm/string.hpp>

#include <AtomicGroup.hpp>
#include <pdb.hpp>
#include <psf.hpp>
#include <amber.hpp>

#include <Trajectory.hpp>
#include <dcd.hpp>
#include <amber_traj.hpp>
#include <amber_rst.hpp>
#include <ccpdb.hpp>


namespace loos {

  AtomicGroup createSystem(const std::string& s) {

    if (boost::iends_with(s, ".pdb")) {
      PDB pdb(s);
      return(pdb);
    } else if (boost::iends_with(s, ".psf")) {
      PSF psf(s);
      return(psf);
    } else if (boost::iends_with(s, ".prmtop")) {
      
      // Special handling to see if there is a crds file...
      std::string coords = findBaseName(s);
      coords += ".inpcrd";
      struct stat buf;
      if (stat(coords.c_str(), &buf) == 0) {
        Amber amber(s, coords);
        return(amber);
      }

      Amber amber(s);
      return(amber);

    } else
      throw(std::runtime_error("Error- cannot divine system file type from name '" + s + "'"));
  }


  pAtomicGroup createSystemPtr(const std::string& s) {

    pAtomicGroup pag;

    if (boost::iends_with(s, ".pdb")) {
      pPDB p(new PDB(s));
      pag = p;
    } else if (boost::iends_with(s, ".psf")) {
      pPSF p(new PSF(s));
      pag = p;
    } else if (boost::iends_with(s, ".prmtop")) {
      pAmber p(new Amber(s));
      pag = p;
    } else
      throw(std::runtime_error("Error- cannot divine system file type from name '" + s + "'"));

    return(pag);
  }



  pTraj createTrajectory(const std::string& s, const AtomicGroup& g) {

    if (boost::iends_with(s, ".dcd")) {
      pDCD pd(new DCD(s));
      pTraj pt(pd);
      return(pt);
    } else if (boost::iends_with(s, ".mdcrd")) {
      pAmberTraj pat(new AmberTraj(s, g.size()));
      pTraj pt(pat);
      return(pt);
    } else if (boost::iends_with(s, ".rst") || boost::iends_with(s, ".rst7")) {
      pAmberRst par(new AmberRst(s, g.size()));
      pTraj pt(par);
      return(pt);
    } else if (boost::iends_with(s, ".pdb")) {
      pCCPDB ppdb(new CCPDB(s));
      pTraj pt(ppdb);
      return(pt);
    } else
      throw(std::runtime_error("Error- cannot divine trajectory file type from name '" + s + "'"));
  }


}

