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

// SRep Logic includes
#include "vtkSlicerSRepLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSRepDisplayNode.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

#include "vtkMRMLSRepNode.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerSRepLogic);

//----------------------------------------------------------------------------
vtkSlicerSRepLogic::vtkSlicerSRepLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerSRepLogic::~vtkSlicerSRepLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerSRepLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerSRepLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerSRepLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);

  vtkMRMLScene *scene = this->GetMRMLScene();

  scene->RegisterNodeClass(vtkSmartPointer<vtkMRMLSRepNode>::New());
  scene->RegisterNodeClass(vtkSmartPointer<vtkMRMLSRepDisplayNode>::New());
}

//---------------------------------------------------------------------------
void vtkSlicerSRepLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerSRepLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerSRepLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
std::string vtkSlicerSRepLogic
::ImportSRep(const std::string& filename)
{
  const auto srepID = this->AddNewSRepNode();
  if (srepID.empty()) {
    vtkErrorMacro("LoadSRep: failed to instantiate vtkMRMLSRepNode");
    return std::string{};
  }

  auto* srep = vtkMRMLSRepNode::SafeDownCast(this->GetMRMLScene()->GetNodeByID(srepID));
  srep->LoadSRepFromFile(filename);
  return srep->GetID();
}

std::string vtkSlicerSRepLogic::AddNewSRepNode(const std::string& name, vtkMRMLScene* scene) {
  std::string id;
  if (!scene && !this->GetMRMLScene()) {
    vtkErrorMacro("AddNewSRepNode: no scene to add a srep node to!");
    return id;
  }

  vtkMRMLScene *addToThisScene = scene ? scene : this->GetMRMLScene();

  // create and add the node
  auto mnode = vtkSmartPointer<vtkMRMLSRepNode>::New();
  addToThisScene->AddNode(mnode);

  // add a display node
  std::string displayID = this->AddFirstDisplayNodeForSRepNode(mnode);

  if (displayID.compare("") != 0) {
    // get the node id to return
    id = std::string(mnode->GetID());
    if (!name.empty()) {
      mnode->SetName(name.c_str());
    }
  }

  return id;
}

std::string vtkSlicerSRepLogic::AddFirstDisplayNodeForSRepNode(vtkMRMLSRepNode *srepNode) {
  const std::string emptyId;
  if (!srepNode || !srepNode->GetScene()) {
    vtkErrorMacro("AddNewDisplayNodeForSRepNode: unable to add a srep display node!");
    return emptyId;
  }

  if (srepNode->GetDisplayNode()) {
    return srepNode->GetDisplayNodeID();
  }

  srepNode->CreateDefaultDisplayNodes();
  auto* displayNode = vtkMRMLSRepDisplayNode::SafeDownCast(srepNode->GetDisplayNode());
  if (!displayNode) {
    vtkErrorMacro("AddNewDisplayNodeForSRepNode: error creating new display node");
    return emptyId;
  }

  return displayNode->GetID();
}
