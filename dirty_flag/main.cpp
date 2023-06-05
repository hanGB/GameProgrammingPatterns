#include "stdafx.h"
#include "graph_node.h"

int main()
{
	Mesh* meshes[MAX_MESH_COUNT];
	int numMesh = 0;

	Mesh* mesh = new Mesh(glm::vec3(1.0, 1.0, 1.0));
	meshes[numMesh] = mesh;
	numMesh++;
	mesh = new Mesh(glm::vec3(2.0, 2.0, 2.0));
	meshes[numMesh] = mesh;
	numMesh++;
	mesh = new Mesh(glm::vec3(3.0, 3.0, 3.0));
	meshes[numMesh] = mesh;
	numMesh++;

	GraphNode* rootNode = new GraphNode(nullptr);

	Transform transforms[3];
	transforms[0] = Transform();
	transforms[0].SetMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	transforms[1] = Transform();
	transforms[1].SetMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));
	transforms[2] = Transform();
	transforms[2].SetMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f)));


	GraphNode* nodes[MAX_NODE];
	int numNode = 0;
	for (int i = 0; i < numMesh; ++i) {
		for (const auto& transform : transforms) {
			// rootNode의 차일드
			int currentNodeIndex = numNode;
			nodes[numNode++] = new GraphNode(meshes[i], transform);
			for (int j = 0; j < numMesh; ++j) {
				// node의 차일드
				nodes[numNode] = new GraphNode(meshes[j], transform);
				nodes[currentNodeIndex]->AddChild(nodes[numNode]);
				numNode++;
			}
			rootNode->AddChild(nodes[currentNodeIndex]);
		}
	}

	rootNode->Render(Transform::Origin(), true);
	std::cout << "---------------------------------------------------------\n";
	rootNode->Render(Transform::Origin(), false);
	std::cout << "---------------------------------------------------------\n";
	nodes[0]->SetTransform(transforms[2]);
	nodes[numNode / 2]->SetTransform(transforms[0]);
	rootNode->Render(Transform::Origin(), false);

	delete rootNode;
	for (int i = 0; i < numMesh; ++i) {
		delete meshes[i];
	}
	for (int i = 0; i < numNode; ++i) {
		delete nodes[i];
	}
}
