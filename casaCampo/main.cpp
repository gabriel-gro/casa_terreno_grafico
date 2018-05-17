
#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/mesh.h>
#include <vart/contrib/viewerglutogl.h>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <iostream>

using namespace std;
using namespace VART;

class Casa: public MeshObject {
	public:
		Casa(){
			vector<double> vertPiramide({
				0, 7, 0,		// v0
				3, 0, 3,		// v1
				-3, 0, 3,		// v2
				-3, 0, -3,		// v3
				3, 0, -3,		// v4
					
				3, 4, 3,		// v5
				-3, 4, 3,		// v6
				-3, 4, -3,		// v7
				3, 4, -3,		// v8
				
				3.01, 0, 0.6,	// v9
				3.01, 2, 0.6,	// v10
				3.01, 2, 2.1,	// v11
				3.01, 0, 2.1,	// v12
				
				3.01, 1.8, -2.3,	// v13
				3.01, 3.2, -2.3,	// v14
				3.01, 3.2, -0.3,	// v15
				3.01, 1.8, -0.3 	// v16

			});
			
			vertCoordVec = vertPiramide;
				
			vector<unsigned int> faceTriang({
				0, 6, 5,
				0, 7, 6,
				0, 8, 7,
				0, 5, 8
			});
			
			vector<unsigned int> faceRetang({
				//~ 1, 2, 3, 4,  # Chao da casa
				1, 4, 8, 5,
				6, 2, 1, 5,
				7, 3, 2, 6,
				8, 4, 3, 7
			});
			
			vector<unsigned int> faceDetalhes({
				9, 10, 11, 12,
				13, 14, 15, 16
			});
			
			Material PLASTIC_BROWN(Color(77, 18, 18));
			
			VART::Mesh triangulos;
			triangulos.type = Mesh::TRIANGLES;
			triangulos.indexVec = faceTriang;
			triangulos.material = PLASTIC_BROWN;
			
			meshList.push_back(triangulos);
			
			VART::Mesh paredes;
			paredes.type = Mesh::QUADS;
			paredes.indexVec = faceRetang;
			paredes.material = Material::PLASTIC_WHITE();
			
			meshList.push_back(paredes);
			
			VART::Mesh detalhes;
			detalhes.type = Mesh::QUADS;
			detalhes.indexVec = faceDetalhes;
			detalhes.material = PLASTIC_BROWN;
			
			meshList.push_back(detalhes);
			
			ComputeBoundingBox();
			ComputeRecursiveBoundingBox();
			ComputeVertexNormals();
		}	
	
};

class Jardin: public MeshObject{
	public:
		Jardin(){
			srand(time(NULL));
			
			vector<double> vertChao;
			int tamQuadrado = 17;  		//numeros impares para coincidir com centro da casa
			int colunas = tamQuadrado;
			int linhas = tamQuadrado;
			int valX = -tamQuadrado / 2;
			int valZ = -tamQuadrado / 2;
			for (int i = 0; i < linhas; i++){
				valZ = -tamQuadrado / 2;
				for (int j = 0; j < colunas; j++){
					vertChao.push_back(valX);
					vertChao.push_back(randomizeAltura());
					vertChao.push_back(valZ);
					++valZ;
				}
				++valX;
			}
			vertCoordVec = vertChao;
			
			for (int i = 0; i < linhas - 1; i++){
				vector<unsigned int> f1;
				for (int j = 0; j < colunas; j++){
					f1.push_back((i+1) * colunas + j);
					f1.push_back((i) * colunas + j);
				}
				
				Mesh chao;
				chao.type = Mesh::TRIANGLE_STRIP;
				chao.material = Material::PLASTIC_GREEN();
				chao.indexVec = f1;
				
				meshList.push_back(chao);
			}
			
			// Chao varde inverso
			for (int i = 0; i < linhas - 1; i++){
				vector<unsigned int> f1;
				for (int j = 0; j < colunas; j++){
					f1.push_back((i) * colunas + j);
					f1.push_back((i+1) * colunas + j);
				}
				
				Mesh chao;
				chao.type = Mesh::TRIANGLE_STRIP;
				chao.material = Material::PLASTIC_GREEN();
				chao.indexVec = f1;
				
				meshList.push_back(chao);
			}
			
			
			ComputeVertexNormals();
			ComputeBoundingBox();
			ComputeRecursiveBoundingBox();
		}
		
		
		
		double randomizeAltura(){ // Cateto Oposto = 0.5 * Tangente do angulo (escolhido foi 30º) tg 30 = -6.5 => -6.5 * 101 /100
			int random = rand();
			return (random % (int)(0.5 * tan(M_PI/6)*101 ) / 100.0);
		}	
};

int main(int argc, char* argv[]){
    
    VART::ViewerGlutOGL::Init(&argc, argv);
    static VART::Scene scene;
    static VART::ViewerGlutOGL viewer;
    
    VART::Camera camera(VART::Point4D(20,7,0),VART::Point4D::ORIGIN(),VART::Point4D::Y());
    camera.SetFarPlaneDistance(50.0);	
    
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

	Casa c;
    scene.AddObject(&c);
    
    Jardin chao;
    scene.AddObject(&chao);
    
    viewer.SetTitle("TENTANDO NAO TIRAR 0");
    viewer.SetScene(scene);
    
    VART::ViewerGlutOGL::MainLoop(); 

    
    return 0;
}
    











