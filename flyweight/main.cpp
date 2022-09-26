#include "stdafx.h"
#include "tree.h"

int main()
{
	TreeModel* treeModel = new TreeModel(1, 2, 3);

	std::vector<Tree*> trees;
	trees.reserve(5 * 5 * 5);
	int i = 0;
	for (int z = 0; z < 5; ++z) {
		for (int y = 0; y < 5; ++y) {
			for (int x = 0; x < 5; ++x) {
				Tree* tree = new Tree(i++, treeModel);
				tree->SetPosition(100.0f * x, 100.0f * y, 100.0f * z);
				trees.push_back(tree);
			}
		}
	}
	for (Tree* tree : trees) {
		tree->Render();
	}

	for (Tree* tree : trees) {
		delete tree;
	}
	delete treeModel;
}