/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkSlicerSRepLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerSRepLogic_h
#define __vtkSlicerSRepLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"


// MRML includes
#include "vtkMRMLSRepNode.h"
#include "vtkMRMLEllipticalSRepNode.h"


// STD includes
#include <cstdlib>
#include <memory>

#include "vtkSlicerSRepModuleLogicExport.h"

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_SREP_MODULE_LOGIC_EXPORT vtkSlicerSRepLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerSRepLogic *New();
  vtkTypeMacro(vtkSlicerSRepLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  /// Creates new Rectangular Grid SRep node
  /// @returns id of new node.
  std::string AddNewRectangularGridSRepNode(const std::string& name = "", vtkMRMLScene* scene = nullptr);
  /// Creates new Elliptical SRep node
  /// @returns id of new node.
  std::string AddNewEllipticalSRepNode(const std::string& name = "", vtkMRMLScene* scene = nullptr);
  /// Creates new SRep display node and add it to srep node
  /// @returns id of new display node.
  std::string AddFirstDisplayNodeForSRepNode(vtkMRMLSRepNode *srepNode);

  /// Import SRep from files
  /// @param filename The file to import the SRep from
  /// @return The MRML id of the created node.
  std::string ImportRectangularGridSRepFromXML(const std::string& filename);

  /// Exports SRep to files.
  ///
  /// @param srepNode Node to write to file. If nullptr, this function returns false.
  /// @param headerFilename File to write the header information to. If empty, this function returns false.
  /// @param upFilename File to write the up spokes to. If empty, this function returns false.
  /// @param downFilename File to write the down spokes to. If empty, this function returns false.
  /// @param crestFilename File to write the crest spokes to. If empty, this function returns false.
  /// @return True if files were written, false otherwise.
  bool ExportRectangularGridSRepToXML(vtkMRMLSRepNode *srepNode,
                  const std::string& headerFilename,
                  const std::string& upFilename,
                  const std::string& downFilename,
                  const std::string& crestFilename);

  /// Load a srep from fileName, return nullptr on error, node ID string
  /// otherwise. Adds the appropriate storage and display nodes to the scene
  /// as well.
  const char* LoadSRep(const char* fileName, const char* nodeName=nullptr);

  /// Creates a new SRep from srepNode with interpolated spokes
  /// @param srepNode The srep to interpolate.
  /// @param interpolationlevel How much denser to make the spokes as a power to 2. An interpolation level of 3 would
  ///        increase density by 8 times (2^3).
  /// @returns The id of the newly created interpolated SRep node
  std::string InterpolateSRep(vtkMRMLEllipticalSRepNode* srepNode, size_t interpolationlevel, const std::string& newNodeName = "");

  bool InterpolateSRep(vtkMRMLEllipticalSRepNode* srepNode, size_t interpolationlevel, vtkMRMLEllipticalSRepNode* destination);

  std::unique_ptr<srep::EllipticalSRep> InterpolateSRep(const srep::EllipticalSRep& srep, size_t interpolationlevel);

protected:
  vtkSlicerSRepLogic();
  virtual ~vtkSlicerSRepLogic();

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;

private:
  vtkSlicerSRepLogic(const vtkSlicerSRepLogic&) = delete;
  vtkSlicerSRepLogic& operator=(const vtkSlicerSRepLogic&) = delete;
  vtkSlicerSRepLogic(vtkSlicerSRepLogic&&) = delete;
  vtkSlicerSRepLogic& operator=(vtkSlicerSRepLogic&&) = delete;
};

#endif
