#ifndef W_CORE_HIERARCHY_NODE_HPP
#define W_CORE_HIERARCHY_NODE_HPP

#include <algorithm>
#include <vector>

namespace warmonger {
namespace core {

/**
 * Utility mix-in class that allows building arbitrary hierarchy trees.
 *
 * This class is designed to be used as per the curiously recurring
 * templates pattern. That is, inheriting classes shall inherit from a template
 * instance instanciated by themselves.
 */
template <typename T>
class HierarchyNode
{
public:
    T* getHierarchyParent() const;
    void setHierarchyParent(T* parent);

    const std::vector<T*>& getHierarchyChildren() const;

    bool isHierarchyRoot() const;
    bool isHierarchyLeaf() const;

private:
    void addHierarchyChild(T* child);
    void removeHierarchyChild(T* child);

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
    if(this->parent != nullptr)
        this->parent->removeHierarchyChild(static_cast<T*>(this));

    this->parent = parent;

    if(this->parent != nullptr)
        this->parent->addHierarchyChild(static_cast<T*>(this));
}

template <typename T>
const std::vector<T*>& HierarchyNode<T>::getHierarchyChildren() const
{
    return children;
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

} // namespace core
} // namespace warmonger

#endif // W_CORE_HIERARCHY_NODE_HPP
