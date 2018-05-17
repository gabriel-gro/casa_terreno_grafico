
#include <vart/scene.h>
#include <vart/light.h>
#include <vart/meshobject.h>
#include <vart/cylinder.h>
#include <vart/arrow.h>
#include <vart/mesh.h>
#include <vart/contrib/viewerglutogl.h>

#include <iostream>

using namespace std;
using namespace VART;

Material COLLOR_BROWN(Color(80,20,20));

class Pe: public MeshObject {
	private:
		Cylinder* pe;
		double alturaPe;
	public:
		Transform* trans;
		Transform* rot;
	
		Pe(){
			this->alturaPe = 3;
			pe = new Cylinder(this->alturaPe, 0.3);  // 1º - Hight | 2º - Radius
			this->pe->SetMaterial(COLLOR_BROWN);
			this->pe->SetPartsVisibility(Cylinder::ALL);
			
			rot = new Transform();			
			trans = new Transform();
			
			//~ rot->MakeIdentity();
			//~ trans->MakeIdentity();
			
			rot->AddChild(*pe);
			trans->AddChild(*rot);
			
		}
		Cylinder* getInstance(){
			return this->pe;
		}
		
};

class Cadeira: public MeshObject {
	private:
		Pe* pes;
	public:
		Cadeira(){
			pes = new Pe[4];
			
			for (int i = 0; i < 4; i++){
				pes[i].rot->MakeRotation(Point4D::X(), -(M_PI/2));
			}
			vector<Point4D> pontosTrans({
				Point4D(0, 0, 0),
				Point4D(2, 0, 0),
				Point4D(2, 0, 2),
				Point4D(0, 0, 2)
			});
			
			for (int i = 0; i < 4; i++){
				pes[i].trans->MakeTranslation(pontosTrans[i]);
			}
			
			for (int i = 0; i < 4; i++){
				this->AddChild(*pes[i].trans);
			}
			double f= 0.3; // Fator de correcao
			vector<double> vertices({
				0-f, 3.01, 0-f, 	// v0  ASSENTO Baixo
				0-f, 3.01, 2+f, 	// v1
				2+f, 3.01, 2+f,  	// v2
				2+f, 3.01, 0-f,		// v3
				
				0-f, 3.3, 0-f, 		// v4  ASSENTO Cima
				0-f, 3.3, 2+f, 		// v5
				2+f, 3.3, 2+f,  	// v6
				2+f, 3.3, 0-f,		// v7
				
				2+f, 6, 0-f,		// v8
				0-f, 6, 0-f,		// v9
				
				0-f, 6, 0,			// v10
				0-f, 3.3, 0,		// v11
				2+f, 3.3, 0,		// v12
				2+f, 6, 0		// v13
				
			});
			vertCoordVec = vertices;
			
			vector<unsigned int> vertAssento({
				
				0, 1, 2, 3,		// parte de cima assento
				3, 2, 1, 0,
				
				4, 5, 6, 7,		// parte de baixo assento
				7, 6, 5, 4,
				
				5, 1, 2, 6,		// Beiradas do assento
				1, 5, 4, 0,
				7, 3, 0, 4,
				3, 7, 6, 2,
				
				7, 4, 9, 8,			// Encosto costas
				10, 11, 12, 13, 	// encosto frente
				
				10, 9, 4, 11,	// Encosto Beiradas
				9, 10, 13, 8,
				12, 7, 8, 13,
				7, 12, 11, 4
			});
			
			Mesh assento;
			assento.type = Mesh::QUADS;
			assento.indexVec = vertAssento;
			assento.material = COLLOR_BROWN;
			
			meshList.push_back(assento);
			
			ComputeBoundingBox();
			ComputeRecursiveBoundingBox();
			ComputeVertexNormals();
		}
		
};

int main(int argc, char* argv[]){

    ViewerGlutOGL::Init(&argc, argv);
    static Scene scene;
    static ViewerGlutOGL viewer;
    
    VART::Camera camera(Point4D(10, 10, 10),Point4D::ORIGIN(),Point4D::Y());
    camera.SetFarPlaneDistance(50.0);	
    
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);
    
    // ------------------------------------------------------- ARROW's
    // Criando arrow's
    int tam = 3;
    Arrow arrowX(tam);
	Arrow arrowY(Point4D::ORIGIN(), Point4D::Y()*tam);
    Arrow arrowZ(Point4D::ORIGIN(), Point4D::Z()*tam);
    
    arrowX.SetMaterial(Material::PLASTIC_RED());
    arrowY.SetMaterial(Material::PLASTIC_GREEN());
    arrowZ.SetMaterial(Material::PLASTIC_BLUE());
    
    // adicionando as felhas de guia
    scene.AddObject(&arrowX);
	scene.AddObject(&arrowY);
	scene.AddObject(&arrowZ);
	// --------------------------------------------------------
	
	Cadeira minhaCadeira;
	scene.AddObject(&minhaCadeira);
    
    viewer.SetTitle("TENTANDO NAO TIRAR 0");
    viewer.SetScene(scene);
    
    VART::ViewerGlutOGL::MainLoop(); 

	
    return 0;
}

