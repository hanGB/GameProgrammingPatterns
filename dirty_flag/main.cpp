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


	for (int i = 0; i < numMesh; ++i) {
		for (const auto& transform : transforms) {
			// rootNode의 차일드
			GraphNode* node = new GraphNode(meshes[i], transform);
			for (int j = 0; j < numMesh; ++j) {
				// node의 차일드
				node->AddChild(new GraphNode(meshes[j], transform));
			}
			rootNode->AddChild(node);
		}
	}

	rootNode->Render(Transform::Origin());

	delete rootNode;
	for (int i = 0; i < numMesh; ++i) {
		delete meshes[i];
	}
}
