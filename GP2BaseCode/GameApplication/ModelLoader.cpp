#include "ModelLoader.h"
#include "GameObject.h"
#include "Components.h"
#include "../D3D10Renderer/Vertex.h"
#include "../D3D10Renderer/D3D10Renderer.h"

#include <fbxsdk.h>

GameObject * ModelLoader::loadModelFromFile(const string& filename, IRenderer *pRenderer)
{
	GameObject *pObject=NULL;

	string extension=filename.substr(filename.find('.')+1);

	if (extension.compare("fbx")==0)
		pObject=loadFbxModelFromFile(filename,pRenderer);
	return pObject;
}

GameObject * ModelLoader::loadFbxModelFromFile(const string& filename, IRenderer *pRenderer)
{
	//Null Game Object
	GameObject *pRootObject=NULL;
	int noVerts=0;
	int noIndices=0;
	int *pIndices=NULL;
	Vertex * pVerts=NULL;

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
	FbxGeometryConverter converter( lSdkManager);

    // Use the first argument as the filename for the importer.
	if(!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
		return NULL;
    }

	// Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");
	FbxAxisSystem SceneAxisSystem = lScene->GetGlobalSettings().GetAxisSystem();


    INT iUpAxisSign;
	FbxAxisSystem::EUpVector UpVector = SceneAxisSystem.GetUpVector( iUpAxisSign );

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();
	FbxNode* lRootNode = lScene->GetRootNode();
	FbxMesh * pMesh=NULL;
	if(lRootNode) {
		pRootObject=new GameObject();
		pRootObject->setName(lRootNode->GetName());
		for (int i=0;i<lRootNode->GetChildCount();i++){
			FbxNode * modelNode=lRootNode->GetChild(i);
			for(int i=0;i<modelNode->GetNodeAttributeCount();i++)
			{
				FbxNodeAttribute *pAttributeNode=modelNode->GetNodeAttributeByIndex(i);
				if (pAttributeNode->GetAttributeType()==FbxNodeAttribute::eMesh)
				{
					//found mesh
					pMesh=(FbxMesh*)pAttributeNode;
					if (pMesh)
					{
						GameObject *pChildGO=new GameObject();
						pChildGO->setName(pMesh->GetName());
						pMesh=converter.TriangulateMesh(pMesh);
						FbxVector4 * verts=pMesh->GetControlPoints();
						int noVerts=pMesh->GetControlPointsCount();

						int noIndices=pMesh->GetPolygonVertexCount();
						int *pIndices=pMesh->GetPolygonVertices();

						Vertex * pVerts=new Vertex[noVerts];
						for(int i=0;i<noVerts;i++)
						{

								pVerts[i].position.x=verts[i][0];
								pVerts[i].position.y=verts[i][1];
								pVerts[i].position.z=verts[i][2];
						}
						for (int iPolygon = 0; iPolygon < pMesh->GetPolygonCount(); iPolygon++) { 
							for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++) {	
								int fbxCornerIndex = pMesh->GetPolygonVertex(iPolygon, iPolygonVertex);
								FbxVector4 fbxVertex = verts[fbxCornerIndex];

								FbxVector4 fbxNormal;	
								pMesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);	
								fbxNormal.Normalize();	
								pVerts[fbxCornerIndex].normal.x=fbxNormal[0];
								pVerts[fbxCornerIndex].normal.y=fbxNormal[1];
								pVerts[fbxCornerIndex].normal.z=fbxNormal[2];

								FbxVector2 fbxUV = FbxVector2(0.0, 0.0);	
								FbxLayerElementUV* fbxLayerUV = pMesh->GetLayer(0)->GetUVs();
								// Get texture coordinate	
								if (fbxLayerUV) {		
									int iUVIndex = 0;		
									switch (fbxLayerUV->GetMappingMode()) {	
										case FbxLayerElement::eByControlPoint:
											iUVIndex = fbxCornerIndex;				
										break;	
										case FbxLayerElement::eByPolygonVertex:
											iUVIndex = pMesh->GetTextureUVIndex(iPolygon, iPolygonVertex, FbxLayerElement::eTextureDiffuse);	
										break;		
									}		
									fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);	
									pVerts[fbxCornerIndex].textureCoords.x=fbxUV[0];
									pVerts[fbxCornerIndex].textureCoords.y= 1.0f-fbxUV[1];
								}
							}
						}

						VisualComponent *pVisualComponent=new VisualComponent();
						pVisualComponent->createVertexBuffer(noVerts,pVerts,pRenderer);
						pVisualComponent->createIndexBuffer(noIndices,pIndices,pRenderer);
						pChildGO->addComponent(pVisualComponent);
						pRootObject->addChild(pChildGO);
						if (pVerts)
						{
							delete [] pVerts;
							pVerts=NULL;
						}
					}
				}
			}
		}
	}

	return pRootObject;
}