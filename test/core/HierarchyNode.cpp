/**
 * \copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "core/HierarchyNode.hpp"
#include "test/catch.hpp"

using namespace warmonger;

struct Node : public core::HierarchyNode<Node>
{
};

TEST_CASE("Basic checks", "[HierarchyMember]")
{
    Node root, node0, node1, leaf0, leaf1;

    node0.setHierarchyParent(&root);
    node1.setHierarchyParent(&root);

    leaf0.setHierarchyParent(&node0);
    leaf1.setHierarchyParent(&node1);

    // root
    REQUIRE(root.isHierarchyRoot());
    REQUIRE(!root.isHierarchyLeaf());

    const std::vector<Node*> rootChildren{&node0, &node1};
    REQUIRE(root.getHierarchyChildren() == rootChildren);

    // node0
    REQUIRE(!node0.isHierarchyRoot());
    REQUIRE(!node0.isHierarchyLeaf());
    REQUIRE(node0.getHierarchyParent() == &root);

    const std::vector<Node*> node0Children{&leaf0};
    REQUIRE(node0.getHierarchyChildren() == node0Children);

    // node1
    REQUIRE(!node1.isHierarchyRoot());
    REQUIRE(!node1.isHierarchyLeaf());
    REQUIRE(node1.getHierarchyParent() == &root);

    const std::vector<Node*> node1Children{&leaf1};
    REQUIRE(node1.getHierarchyChildren() == node1Children);

    // leaf0
    REQUIRE(!leaf0.isHierarchyRoot());
    REQUIRE(leaf0.isHierarchyLeaf());
    REQUIRE(leaf0.getHierarchyParent() == &node0);
    REQUIRE(leaf0.getHierarchyChildren().empty());

    // leaf1
    REQUIRE(!leaf1.isHierarchyRoot());
    REQUIRE(leaf1.isHierarchyLeaf());
    REQUIRE(leaf1.getHierarchyParent() == &node1);
    REQUIRE(leaf1.getHierarchyChildren().empty());

    // move leaf0 to node1
    leaf0.setHierarchyParent(&node1);

    REQUIRE(leaf0.getHierarchyParent() == &node1);
    REQUIRE(node0.getHierarchyChildren().empty());
    REQUIRE(node0.isHierarchyLeaf());

    const std::vector<Node*> node1NewChildren{&leaf1, &leaf0};
    REQUIRE(node1.getHierarchyChildren() == node1NewChildren);
}
