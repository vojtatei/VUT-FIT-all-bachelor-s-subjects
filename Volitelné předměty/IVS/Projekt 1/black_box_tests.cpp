//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

namespace BlackBoxTesting
{

	using namespace ::testing;


	class TestingTree : public Test
	{

	protected:
		BinaryTree tree;

		int numberOfNodes = 13,
				nodes[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 42},
				numberOfLeafNodes = 14,
				leafNodeParents[8] = {1, 2, 6, 7, 9, 10, 12, 42};

	};


	class EmptyTree : public TestingTree
	{
	};


	class NonEmptyTree : public TestingTree
	{

	protected:

		void SetUp() override
		{
			for (int value : nodes) {
				tree.InsertNode(value);
			}
		}

	};

TEST_F(EmptyTree, InsertNode)
{
    BinaryTree tree;

    std::pair<bool, Node_t *> node = tree.InsertNode(42);
    ASSERT_NE(node.second, nullptr);
    EXPECT_EQ(node.second->key, 42);
    std::vector<Node_t *> AllNodes;
    tree.GetAllNodes(AllNodes);
    EXPECT_EQ(AllNodes.size(), 3);
}

TEST_F(EmptyTree, InsertNewNode)
{
	BinaryTree tree;
	std::pair<bool, Node_t *> node = tree.InsertNode(42);
	EXPECT_TRUE(node.first);
	EXPECT_EQ(42, node.second->key);


}
TEST_F(EmptyTree, InsertExistingNode)
{
	BinaryTree tree;
	tree.InsertNode(42);
	std::pair<bool, Node_t *> node = tree.InsertNode(42);
	EXPECT_FALSE(node.first);
	EXPECT_NE(nullptr, node.second);
	EXPECT_EQ(42, node.second->key);
}
TEST_F(EmptyTree, InsertNodes)
{
	std::vector<int> keys{2,8,4,56,34,28};
	std::vector<std::pair<bool, Node_t *>> NewNodes;
	tree.InsertNodes(keys, NewNodes);

	for(size_t i = 0; i < keys.size(); ++i)
	{
		EXPECT_TRUE(NewNodes[i].first);
		EXPECT_EQ(keys[i], NewNodes[i].second->key);
	}

	std::vector<std::pair<bool, Node_t *>> ExistingNodes;
	tree.InsertNodes(keys, ExistingNodes);

	for(size_t j = 0; j < keys.size(); ++j)
	{
		EXPECT_FALSE(ExistingNodes[j].first);
		EXPECT_EQ(keys[j], NewNodes[j].second->key);
	}
}
TEST_F(EmptyTree, DeleteNodeNonexistent) 
{
	EXPECT_FALSE(tree.DeleteNode(42));
	EXPECT_EQ(nullptr, tree.FindNode(42));
}
TEST_F(NonEmptyTree, DeleteNodeNonexistent) 
{
	bool deleted = tree.DeleteNode(99);
	EXPECT_FALSE(deleted);
}
TEST_F(NonEmptyTree, DeleteNode) {
    bool deleted = tree.DeleteNode(42);
    EXPECT_TRUE(deleted);

    Node_t *node = tree.FindNode(42);
    EXPECT_EQ(node, nullptr);
}
TEST_F(EmptyTree, FindNodeNull)
{
	EXPECT_EQ(nullptr, tree.FindNode(1));
}
TEST_F(NonEmptyTree, FindNode)
{
	Node_t *node = tree.FindNode(4);
	EXPECT_NE(nullptr,node);
	EXPECT_EQ(4,node->key);
}
TEST_F(NonEmptyTree,FindNodeNull)
{
	EXPECT_EQ(nullptr,tree.FindNode(99));
}

TEST_F(NonEmptyTree, LeafNodesAreBlack) {
  BinaryTree tree;
  std::vector<Node_t*> leaf_nodes;
  tree.GetLeafNodes(leaf_nodes);
  for (auto node : leaf_nodes) {
    EXPECT_EQ(node->color, BLACK);
  }
}

TEST_F(NonEmptyTree, RedNodeChildrenAreBlack) {
  BinaryTree tree;

  std::vector<Node_t*> all_nodes;
  tree.GetAllNodes(all_nodes);
  for (auto node : all_nodes) {
    if (node->color == RED) {
      if (node->pLeft) {
        EXPECT_EQ(node->pLeft->color, BLACK);
      }
      if (node->pRight) {
        EXPECT_EQ(node->pRight->color, BLACK);
      }
    }
  }
}

}

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

/*** Konec souboru black_box_tests.cpp ***/