#ifndef W_CORE_HIERARCHY_NODE_HPP
#define W_CORE_HIERARCHY_NODE_HPP

#include <vector>

namespace warmonger {
namespace core {

/**
 * Utility mix-in class that allows building arbitrary hierarchy trees.
 */
template <typename T>
class HierarchyNode
{
public:
    T* getHierarchyParent() const;
    void setHierarchyParent(T* parent);

    const std::vector<T*>& getHierarchyChildren() const;

    void addHierarchyChild(T* child);
    void removeHierarchyChild(T* child);

    bool isHierarchyRoot() const;
    bool isHierarchyLeaf() const;

private:
    T* parent{nullptr};
    std::vector<T*> children;
};

template <typename T>
T* HierarchyNode<T>::getHierarchyParent() const
{
    return parent;
}

template <typename T>
void HierarchyNode<T>::setHierarchyParent(T* parent)
{
    this->parent = parent;
}

template <typename T>
const std::vector<T*>& HierarchyNode<T>::getHierarchyChildren() const
{
    return children;
}

template <typename T>
void HierarchyNode<T>::addHierarchyChild(T* child)
{
    this->children.push_back(child);
}

template <typename T>
void HierarchyNode<T>::removeHierarchyChild(T* child)
{
    const auto it = std::find(this->children.cbegin(), this->children.cend(), child);
    if (it != this->children.cend())
    {
        this->children.erase(it);
    }
}

template <typename T>
bool HierarchyNode<T>::isHierarchyRoot() const
{
    return this->parent == nullptr;
}

template <typename T>
bool HierarchyNode<T>::isHierarchyLeaf() const
{
    return this->children.empty();
}

} // namespace core
} // namespace warmonger

#endif // W_CORE_HIERARCHY_NODE_HPP
